//============================================================================
// 635 WinTest Code.
// Copyright 2002, Crystalfontz America, Inc. Written by Brent A. Crosby
// www.crystalfontz.com, brent@crystalfontz.com
//============================================================================
typedef unsigned char ubyte;
typedef signed char sbyte;
typedef unsigned short word;
typedef unsigned long dword;
typedef union
  {
  ubyte
    as_bytes[2];
  word
    as_word;
  } WORD_UNION;
//============================================================================
