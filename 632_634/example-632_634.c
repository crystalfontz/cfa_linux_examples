// 632_634_COMMAND_LINE_TEST.cpp
// Ultra-simple 632 / 634 command-line communications example

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "typedefs.h"
#include "serial.h"

//============================================================================
int main(int argc, char* argv[])
  {
  printf("Ultra-simple CFA-632 / CFA-634 command-line communications example.\n");
  printf("Crystalfontz America, Inc. http://www.crystalfontz.com\n\n");
  printf("Usage:\n");
  printf("%s PORT BAUD\n",argv[0]);
  printf("PORT is something like \"/dev/ttyS0\" or \"/dev/usb/ttyUSB1\"\n");
  printf("BAUD is 1200, 2400, 4800, 9600 or 19200\n");
  printf("To clear the display, enter \"clear\" as an optional third parameter\n\n");
  

  //If only 0 or 1 parameter is entered, prompt for the missing parameter(s)
  if(argc < 3)
    {
      printf("\nMISSING A PARAMETER. Enter both PORT and BAUD.\n\n");
      return(0);
    }

  //Check for optional "clear" parameter and set flag if found
  int
    cleardisplay=0;
  if((argc > 3) && (!strcmp(argv[3],"clear"))) 
    cleardisplay=1;
 

  int
    baud;
  //default the baud to 19200
  if(!strcmp(argv[2],"1200"))
    baud=1200;
  else
    if(!strcmp(argv[2],"2400"))
      baud=2400;
    else
      if(!strcmp(argv[2],"4800"))
        baud=4800;
      else
        if(!strcmp(argv[2],"9600"))
          baud=9600;
        else
          baud=19200;

  if(Serial_Init(argv[1],baud))
    {
      printf("Could not open port \"%s\" at \"%d\" baud.\n",argv[1],baud);
      return(1);
    }
  else
    printf("\"%s\" opened at \"%d\" baud.\n\n",argv[1],baud);


  //We will try to be tricky, and send out data that will show up correctly
  //on both the 634 and 632.

  //First turn off "scroll" and "wrap"
  SendByte(20);
  SendByte(24);
 
  //Write out the bottom two lines
  //on the 634. The 632 will ignore
  //these lines.

  //Position to line 4, col 1
  SendByte(17);
  SendByte(0);
  SendByte(3);
  if(cleardisplay==1)
    {
                //12345678901234567890
      SendString("                    ");
    }
  else
    {
                //12345678901234567890
      SendString("This is Line 4 *****");
    }

  //Position to line 3, col 1
  SendByte(17);
  SendByte(0);
  SendByte(2);
  if(cleardisplay==1)
    {
                //12345678901234567890
      SendString("                    ");
    }
  else
    {
                //12345678901234567890
      SendString("This is Line 3 *****");
    }


  //Write out top two lines of the 
  //634 and both lines of the 632.

  //Position to line 2, col 1
  SendByte(17);
  SendByte(0);
  SendByte(1);
  if(cleardisplay==1)
    {
                //12345678901234567890
      SendString("                    ");
    }
  else
    {
                //12345678901234567890
      SendString("This is Line 2 *****");
    }

  //Position to line 1, col 1
  SendByte(17);
  SendByte(0);
  SendByte(0);
  if(cleardisplay==1)
    {
                //12345678901234567890
      SendString("                    ");
    }
  else
    {
                //12345678901234567890
      SendString("This is Line 1 *****");
    }

  
  if(cleardisplay==1) printf("Display Cleared.\n");
  printf("Done.\n\n");
  Uninit_Serial();
  return 0;
  }
//============================================================================
