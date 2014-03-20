// 631_COMMAND_LINE_TEST.cpp
// Ultra-simple 631 command-line communications example

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "typedefs.h"
#include "serial.h"
#include "cf_packet.h"
#include "show_packet.h"

//============================================================================
int main(int argc, char* argv[])
  {
  printf("Ultra-simple CFA-631 command-line communications example.\n");
  printf("Crystalfontz America, Inc. http://www.crystalfontz.com\n\n");
  printf("Usage:\n");
  printf("%s PORT BAUD\n",argv[0]);
  printf("PORT is something like \"/dev/ttyS0\" or \"/dev/usb/ttyUSB0\"\n");
  printf("BAUD is 19200 or 115200\n");
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
  if((argc > 3) && (!strcmp(argv[3],"clear"))) cleardisplay=1;


  int
    baud;
  //default the baud to 19200
  if(strcmp(argv[2],"115200"))
    baud=19200;
  else
    baud=115200;

  if(Serial_Init(argv[1],baud))
    {
    printf("Could not open port \"%s\" at \"%d\" baud.\n",argv[1],baud);
    return(1);
    }
  else
    printf("\"%s\" opened at \"%d\" baud.\n\n",argv[1],baud);

//Outgoing command packets. Either clear the screen
//or send our line information
//**********************************************
  if(cleardisplay)
  {
    outgoing_response.command = 6;
    outgoing_response.data_length = 0;
    send_packet();
  }
  else
  {
    //Send line 1 to the 631 using command 31
    outgoing_response.command = 31;
    outgoing_response.data[0]=0; //col
    outgoing_response.data[1]=0; //row
    memcpy(&outgoing_response.data[2],"* >This is line 1< *",20);
    outgoing_response.data_length = 22;
    send_packet();

  //CFA-631 / CFA-633 communications protocol only allows
  //one outstanding packet at a time. Wait for the response
  //packet from the CFA-631 / CFA-633 before sending another
  //packet.
  int
    k;
  int

    timed_out;
  timed_out = 1; //default timed_out is true
  for(k=0;k<=10000;k++)
    if(check_for_packet())
      {
      ShowReceivedPacket();
      timed_out = 0; //set timed_out to false
      break;
      }
  if(timed_out)
    printf("Timed out waiting for a response.\n");

//Send line 2 to the 631 using command 31
    outgoing_response.command = 31;
    outgoing_response.data_length = 22;
    outgoing_response.data[0]=0; //col
    outgoing_response.data[1]=1; //row
    memcpy(&outgoing_response.data[2],"* >This is line 2< *",20);
    send_packet();

  //CFA-631 / CFA-633 communications protocol only allows
  //one outstanding packet at a time. Wait for the response
  //packet from the CFA-631 / CFA-633 before sending another
  //packet.
  timed_out = 1; //reset timed_out to default false value
  for(k=0;k<=10000;k++)
    if(check_for_packet())
      {
      ShowReceivedPacket();
      timed_out = 0; //set timed_out to false
      break;
      }
  if(timed_out)
    printf("Timed out waiting for a response.\n");
  }
//**********************************************

  printf("Ctrl-C to exit.\n");
  printf("Updated display, now waiting for packets\n\n");
 
  while(!cleardisplay) 
   {
     usleep(100000);  // 1/10 second
     if(check_for_packet())
       ShowReceivedPacket();
   }

  if(cleardisplay) printf("Display Cleared.\n");
  printf("Done.\n\n");
  Uninit_Serial();
  return 0;

  }
//============================================================================
