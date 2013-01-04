// stubs for VxWorks interfaces
#ifndef _VXSTUBS_H_
#define _VXSTUBS_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <pthread.h>

extern "C" {

typedef int BOOL;

typedef int INT;
typedef signed char INT8;
typedef signed short INT16;
typedef signed long INT32;
typedef signed long long INT64;

typedef unsigned int UINT;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned long UINT32;
typedef unsigned long long UINT64;

typedef int (*FUNCPTR) ();

typedef int STATUS;
#define OK 0
#define ERROR -1

#define	sysClkRateGet() (1000 / 50)	// 50mS tick?
#define	NO_WAIT 0

typedef int TASK_ID;	// pthread_t
typedef void* SEM_ID;	// class vxsem *

#define SEM_Q_FIFO		 0x00	/* first in first out queue */
#define SEM_Q_PRIORITY		 0x01	/* priority sorted queue */
#define SEM_DELETE_SAFE		 0x04	/* owner delete safe (mutex opt.) */
#define SEM_INVERSION_SAFE	 0x08	/* no priority inversion (mutex opt.) */
#define SEM_EVENTSEND_ERR_NOTIFY 0x10	/* notify when eventRsrcSend fails */
#define SEM_INTERRUPTIBLE        0x20   /* interruptible on RTP signal */

#define	SEM_EMPTY	0
#define	SEM_FULL	1

// extern TASK_ID taskSpawn(char *name, int priority, int options, 
// 			 int stackSize, FUNCPTR entryPt, int arg1,  
// 			 int arg2, int arg3, int arg4, int arg5, 
// 			 int arg6, int arg7, int arg8, int arg9, 
// 			 int arg10);

extern STATUS taskDelete( TASK_ID task );

extern STATUS taskDelay( int delay );

// extern STATUS taskSuspend( TASK_ID task );	// kill with SIGSTOP?
// extern STATUS taskResume( TASK_ID task );	// kill with SIGCONT?
// extern BOOL taskIsReady( TASK_ID task );
// extern BOOL taskIsSuspended( TASK_ID task );

extern BOOL taskIdVerify( TASK_ID task );

// extern STATUS taskRestart( TASK_ID task );
// extern STATUS taskPrioritySet( TASK_ID task, INT32 newPriority );
// extern STATUS taskPriorityGet( TASK_ID task, INT32 * pPriority );
// extern TASK_ID taskNameToId( /*const*/ char *name );

extern TASK_ID taskIdSelf( void );
extern STATUS taskSafe( void );
extern STATUS taskUnsafe( void );
extern STATUS taskKill( TASK_ID task, int signal );

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

inline int errnoGet() {
    return errno;
}

inline STATUS errnoSet( int err ) {
    errno = err;
    return OK;
}

inline STATUS printErrno( int err ) {
    char buf[100];
    strerror_r(err, buf, sizeof buf);
    fprintf(stderr, "%s", buf);
    return OK;
}

inline int fioRead( int fd, char* buf, size_t n ) {
    return read(fd, buf, n);
}

inline int fioWrite( int fd, char* buf, size_t n ) {
    return write(fd, buf, n);
}

extern UINT32 hostGetByName( const char* );

#define	WAIT_NONE (0)
#define WAIT_FOREVER (-1)

extern SEM_ID semBCreate( int options, int initialState );
extern SEM_ID semMCreate( int options );
extern STATUS semTake( SEM_ID sem, int waitTime );
extern STATUS semGive( SEM_ID sem );
extern STATUS semFlush( SEM_ID sem );
extern STATUS semDelete( SEM_ID sem );

extern STATUS connectWithTimeout( int, struct sockaddr *, size_t, struct timeval * );

typedef void* SYMTAB_ID;
typedef int SYM_TYPE;

STATUS symFindByValue( SYMTAB_ID, UINT, char*, int*, SYM_TYPE* );

extern SYMTAB_ID statSymTbl;
extern SYMTAB_ID sysSymTbl;

#define	MAX_SYS_SYM_LEN	256

typedef void* MODULE_ID;

extern STATUS moduleNameFindBySymbolName( const char* symbol, char* module );
extern MODULE_ID moduleFindByName( const char* name );
extern STATUS unldByModuleId( MODULE_ID id, int options );

extern INT32 UserSwitchInput( INT32 nSwitch );
extern INT32 LedInput( INT32 led );
extern INT32 LedOutput( INT32 led, INT32 value );

}; // extern "C"

#endif // _VXSTUBS_H_
