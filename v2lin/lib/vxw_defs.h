/****************************************************************************
 * Copyright (C) 2004, 2005, 2006 v2lin Team <http://v2lin.sf.net>
 * Copyright (C) 2000,2001  Monta Vista Software Inc.
 * 
 * This file is part of the v2lin Library.
 * VxWorks is a registered trademark of Wind River Systems, Inc.
 * 
 * Initial implementation Gary S. Robertson, 2000, 2001.
 * Contributed by Andrew Skiba, skibochka@sourceforge.net, 2004.
 * Contributed by Mike Kemelmakher, mike@ubxess.com, 2005.
 * Contributed by Constantine Shulyupin, conan.sh@gmail.com, 2006.
 * 
 * The v2lin library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * The v2lin Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 ****************************************************************************/
#ifndef _VXW_DEFS_H_
#define _VXW_DEFS_H_

__BEGIN_DECLS;
/*
**  Generic data types
*/
typedef unsigned int UINT;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef unsigned long long UINT64;

typedef int INT;
typedef char INT8;
typedef short INT16;
typedef long INT32;
typedef long long INT64;

typedef unsigned long INSTR;	// 32-bit word-aligned value

typedef int BOOL;
typedef int STATUS;

/*
**  Miscellaneous error codes
*/
#define TASK_ERRS                       0x00030000
#define MEM_ERRS                        0x00110000
#define MSGQ_ERRS                       0x00410000
#define OBJ_ERRS                        0x003d0000
#define SEM_ERRS                        0x00160000
#define SM_OBJ_ERRS                     0x00580000
#define S_memLib_NOT_ENOUGH_MEMORY      (MEM_ERRS + 1)
#define S_msgQLib_INVALID_MSG_LENGTH    (MSGQ_ERRS + 1)
#define S_objLib_OBJ_DELETED            (OBJ_ERRS + 3)
#define S_objLib_OBJ_ID_ERROR           (OBJ_ERRS + 1)
#define S_objLib_OBJ_TIMEOUT            (OBJ_ERRS + 4)
#define S_objLib_OBJ_UNAVAILABLE        (OBJ_ERRS + 2)
#define S_semLib_INVALID_OPERATION      (SEM_ERRS + 0x00000068)
#define S_smObjLib_NOT_INITIALIZED      (SM_OBJ_ERRS + 1)
#define S_taskLib_ILLEGAL_PRIORITY      (TASK_ERRS + 0x00000065)
#define S_taskLib_ILLEGAL_OPTIONS       (TASK_ERRS + 0x00000066)
#define S_taskLib_ILLEGAL_OPERATION     (TASK_ERRS + 0x00000067)

/*
**  Timeout options
*/
#define NO_WAIT                         0
#define WAIT_FOREVER                    -1
/*
**  Message Queue Option Flags
*/
#define MSG_PRI_NORMAL                  0x00
#define MSG_PRI_URGENT                  0x01
#define MSG_Q_FIFO                      0x00
#define MSG_Q_PRIORITY                  0x01
/*
**  Semaphore Option Flags
*/
#define SEM_EMPTY                       0x00
#define SEM_FULL                        0x01
#define SEM_Q_FIFO                      0x00
#define SEM_Q_PRIORITY                  0x01
#define SEM_DELETE_SAFE                 0x04
#define SEM_INVERSION_SAFE              0x08


#include <pthread.h>

__END_DECLS;

#endif // _VXW_DEFS_H_
