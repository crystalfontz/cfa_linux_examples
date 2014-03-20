//============================================================================
// 635 Linux Demonstration Code.
// serial,c: Ultra-simple 635 command-line communications example
// Copyright 2005, Crystalfontz America, Inc. Written by Brent A. Crosby
// www.crystalfontz.com, brent@crystalfontz.com
//============================================================================
#include  <stdlib.h>
#include  <termios.h>
#include  <unistd.h>
#include  <fcntl.h>
#include  <errno.h>
#include  <ctype.h>
#include  <stdio.h>
#include  "typedefs.h"
#include  "serial.h"

#define FALSE   0
#define TRUE    1

// com port handle
int handle;

// data buffering
#define RECEIVEBUFFERSIZE 4096
ubyte SerialReceiveBuffer[RECEIVEBUFFERSIZE];
dword ReceiveBufferHead;
dword ReceiveBufferTail;
dword ReceiveBufferTailPeek;

//------------------------------------------------------------------------------
int Serial_Init(char *devname, int baud_rate)
  {
  int
    brate;
  struct
    termios term;

  //open device
  handle = open(devname, O_RDWR | O_NOCTTY | O_NONBLOCK);

  if(handle <= 0)
    {
      	printf("Serial_Init:: Open() failed\n");
	return(1);
    }

  //get baud rate constant from numeric value
  switch (baud_rate)
  {
    case 19200:
      brate=B19200;
      break;
    case 115200:
      brate=B115200;
      break;
    default:
      printf("Serial_Init:: Invalid baud rate: %d (must be 19200 or 115200)\n", baud_rate);
      return(2);
  }
  //get device struct
  if(tcgetattr(handle, &term) != 0)
    {
    printf("Serial_Init:: tcgetattr() failed\n");
    return(3);
    }

  //input modes
  term.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|INPCK|ISTRIP|INLCR|IGNCR|ICRNL
                  |IXON|IXOFF);
  term.c_iflag |= IGNPAR;

  //output modes
  term.c_oflag &= ~(OPOST|ONLCR|OCRNL|ONOCR|ONLRET|OFILL
                  |OFDEL|NLDLY|CRDLY|TABDLY|BSDLY|VTDLY|FFDLY);

  //control modes
  term.c_cflag &= ~(CSIZE|PARENB|PARODD|HUPCL|CRTSCTS);
  term.c_cflag |= CREAD|CS8|CSTOPB|CLOCAL;

  //local modes
  term.c_lflag &= ~(ISIG|ICANON|IEXTEN|ECHO);
  term.c_lflag |= NOFLSH;

  //set baud rate
  cfsetospeed(&term, brate);
  cfsetispeed(&term, brate);

  //set new device settings
  if(tcsetattr(handle, TCSANOW, &term)  != 0)
    {
    printf("Serial_Init:: tcsetattr() failed\n");
    return(4);
    }

  ReceiveBufferHead=ReceiveBufferTail=0;

  printf("Serial_Init:: success\n");
  return(0);
  }
//------------------------------------------------------------------------------
void Uninit_Serial()
  {
  close(handle);
  handle=0;
  }
//------------------------------------------------------------------------------
void SendByte(unsigned char datum)
  {
  dword
    bytes_written;
  bytes_written=0;

  if(handle)
    {
    if((bytes_written=write(handle, &datum, 1)) != 1)
      printf("SendByte(): system call \"write()\" return error.\n  Asked for %d bytes to be written, but %d bytes reported as written.\n",
             1,(int)bytes_written);
    }
  else
    printf("SendByte(): \"handle\" is null\n");
  }
//------------------------------------------------------------------------------
void SendData(unsigned char *data,int length)
  {
  dword
    bytes_written;
  bytes_written=0;

  if(handle)
    {
    if((bytes_written=write(handle, data, length)) != length)
      printf("SendData(): system call \"write()\" return error.\n  Asked for %d bytes to be written, but %d bytes reported as written.\n",
             length,(int)bytes_written);
    }
  else
    printf("SendData(): \"handle\" is null\n");

  }
//------------------------------------------------------------------------------
void SendString(char *data)
  {
  while(*data)
    {
    usleep(500);
    SendByte(*data++);
    }
  }
//------------------------------------------------------------------------------
//Gets incoming data and puts it into SerialReceiveBuffer[].
void Sync_Read_Buffer(void)
  {
  ubyte
    Incoming[4096];
  long
    BytesRead;

  //  COMSTAT status;
  dword
    i;

  if(!handle)
    return;

  //get data
  BytesRead = read(handle, &Incoming, 4096);
  if(0<BytesRead)
  {
  //Read the incoming ubyte, store it.
  for(i=0; i < BytesRead; i++)
    {
    SerialReceiveBuffer[ReceiveBufferHead] = Incoming[i];

    //Increment the pointer and wrap if needed.
    ReceiveBufferHead++;
    if (RECEIVEBUFFERSIZE <= ReceiveBufferHead)
      ReceiveBufferHead=0;
      }
    }
  }
/*---------------------------------------------------------------------------*/
dword BytesAvail(void)
  {
  //LocalReceiveBufferHead and return_value are a signed variables.
  int
    LocalReceiveBufferHead;
  int
    return_value;

  //Get a register copy of the head pointer, since an interrupt that
  //modifies the head pointer could screw up the value. Convert it to
  //our signed local variable.
  LocalReceiveBufferHead = ReceiveBufferHead;
  if((return_value = (LocalReceiveBufferHead - (int)ReceiveBufferTail)) < 0)
    return_value+=RECEIVEBUFFERSIZE;

  return(return_value);
  }
/*---------------------------------------------------------------------------*/
ubyte GetByte(void)
  {
  dword
    LocalReceiveBufferTail;
  dword
    LocalReceiveBufferHead;
  ubyte
    return_byte=0;

  //Get a register copy of the tail pointer for speed and size.
  LocalReceiveBufferTail=ReceiveBufferTail;

  //Get a register copy of the head pointer, since an interrupt that
  //modifies the head pointer could screw up the value.
  LocalReceiveBufferHead=ReceiveBufferHead;


  //See if there are any more bytes available.
  if(LocalReceiveBufferTail!=LocalReceiveBufferHead)
    {
    //There is at least one more ubyte.
    return_byte=SerialReceiveBuffer[LocalReceiveBufferTail];

    //Increment the pointer and wrap if needed.
    LocalReceiveBufferTail++;
    if(RECEIVEBUFFERSIZE<=LocalReceiveBufferTail)
      LocalReceiveBufferTail=0;

    //Update the real ReceiveBufferHead with our register copy. Make sure
    //the ISR does not get serviced during the transfer.
    ReceiveBufferTail=LocalReceiveBufferTail;
    }

  return(return_byte);
  }
/*---------------------------------------------------------------------------*/
dword PeekBytesAvail(void)
  {
  //LocalReceiveBufferHead and return_value are a signed variables.
  int
    LocalReceiveBufferHead;
  int
    return_value;

  //Get a register copy of the head pointer, since an interrupt that
  //modifies the head pointer could screw up the value. Convert it to
  //our signed local variable.
  LocalReceiveBufferHead=ReceiveBufferHead;
  if((return_value=(LocalReceiveBufferHead-(int)ReceiveBufferTailPeek))<0)
    return_value+=RECEIVEBUFFERSIZE;
  return(return_value);
  }
/*---------------------------------------------------------------------------*/
void Sync_Peek_Pointer(void)
  {
  ReceiveBufferTailPeek=ReceiveBufferTail;
  }
/*---------------------------------------------------------------------------*/
void AcceptPeekedData(void)
  {
  ReceiveBufferTail=ReceiveBufferTailPeek;
  }
/*---------------------------------------------------------------------------*/
ubyte PeekByte(void)
  {
  int
    LocalReceiveBufferTailPeek;
  int
    LocalReceiveBufferHead;
  ubyte return_byte=0;

  //Get a register copy of the tail pointer for speed and size.
  LocalReceiveBufferTailPeek=ReceiveBufferTailPeek;

  //Get a register copy of the head pointer, since an interrupt that
  //modifies the head pointer could screw up the value.
  LocalReceiveBufferHead=ReceiveBufferHead;

  //See if there are any more bytes available.
  if(LocalReceiveBufferTailPeek!=LocalReceiveBufferHead)
    {
    //There is at least one more ubyte.
    return_byte=SerialReceiveBuffer[LocalReceiveBufferTailPeek];

    //Increment the pointer and wrap if needed.
    LocalReceiveBufferTailPeek++;
    if(RECEIVEBUFFERSIZE<=LocalReceiveBufferTailPeek)
      LocalReceiveBufferTailPeek=0;

    //Update the real ReceiveBufferHead with our register copy. Make sure
    //the ISR does not get serviced during the transfer.
    ReceiveBufferTailPeek=LocalReceiveBufferTailPeek;
    }
  return(return_byte);
  }
/*---------------------------------------------------------------------------*/
