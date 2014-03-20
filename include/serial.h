//============================================================================
// 635 Linux Demonstration Code.
// serial,h: Ultra-simple 635 command-line communications example
// Copyright 2005, Crystalfontz America, Inc. Written by Brent A. Crosby
// www.crystalfontz.com, brent@crystalfontz.com
//============================================================================
int Serial_Init(char *devname, int baud_rate);
void Uninit_Serial();
void SendByte(unsigned char datum);
void Sync_Read_Buffer(void);
dword BytesAvail(void);
ubyte GetByte(void);
dword PeekBytesAvail(void);
void Sync_Peek_Pointer(void);
void AcceptPeekedData(void);
ubyte PeekByte(void);
void SendData(unsigned char *data,int length);
void SendString(char *data);
//============================================================================
