#cfa_linux_examples

Linux-compatible(*) command-line example programs with C source code. Supports Crystalfontz Intelligent LCD Modules using USB and serial interface including CFA533, CFA631, CFA632, CFA633, CFA634, CFA635.

CFA735 and CFA835 can be demonstrated using the CFA635 example.


#Basic Installation

These are generic installation instructions: (Note that gcc v3.0 or later, make and their supporting libraries are required for this utility to work.)

#Extracting the bundle
* Extract the files contained within the cfa_linux_examples.tar.gz archive to the desired location.
* To extract the files use the command: "tar -xzvf cfa_linux_examples.tar.gz"

#Compiling from source
* 'cd' into the created folder and use 'make' to compile and create the example binaries.

#Executing the programs
* The example utilities run with the command line options of:

````
~$ ./example-XXX PORT BAUD
   PORT is something like "/dev/ttyS0" or "/dev/usb/ttyUSB0"
   BAUD is 19200 or 115200
   To clear the display, enter "clear" as an optional third parameter
````

* The screen output should show something similar to the following:

````
----------------------
-  >This is line 1<  -
-  >This is line 2<  -
----------------------
````

#Notes on Baud Rate
   This utility may be used to test the 631, 632, 633, 634, and 635 displays.

   BAUD rates for the modules are as follows:
      631 USB: 115200
      632 USB: 19200
      633 USB/Serial: 19200 (default) or 115200 (FW v1.4 and beyond)
      634 USB: 19200
      635 USB: 115200

   If the above baud rate does not give any output on the LCD, try 19200 or 115200
   as earlier/later models of a USB LCD may use a different baud rate to those shown
   above.

#Make options
   If you modify the existing code and wish to do a fresh make, run
   'make clean' and then 'make' again. 

#Source Code
Source code for the example utilities are also contained in this archive.
See the extracted sub-directories for the C source code.

(*) This source code (with a few minor modifications) should also work under other operating systems.
If you would like to have us (Crystalfontz) help you make these work under a different OS, please email us.

#Contact Info
````
    Crystalfontz America, Incorporated
    12412 East Saltese Ave
    Spokane Valley, WA 99216-0357
    http://www.crystalfontz.com
    sales@crystalfontz.com
    voice (509) 892-1200 
    US toll-free (888) 206-9720
    fax (509) 892-1203 
````
