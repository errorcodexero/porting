// stubs for VxWorks interfaces
#ifndef _VXSTUBS_H_
#define _VXSTUBS_H_

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>

#include "../v2lin/lib/vxw_defs.h"
#include "../v2lin/lib/vxw_hdrs.h"
//       "../v2lin/lib/loadLib.h"	// not implemented by v2lin
#include "../v2lin/lib/sysLib.h"
#include "../v2lin/lib/taskVarLib.h"
#include "../v2lin/lib/tickLib.h"
#include "../v2lin/lib/v2ldebug.h"
#include "../v2lin/lib/v2lpthread.h"

extern "C" {

// typedef int STATUS;
#define OK 0
#define ERROR -1

typedef int TASK_ID;

#define	VX_FP_TASK 1

#define sysClkRateGet() (1000 / V2PT_TICK)
#define	NO_WAIT 0

extern TASK_ID taskSpawn(char *name, int priority, int options, 
			 int stackSize, FUNCPTR entryPt, int arg1,  
			 int arg2, int arg3, int arg4, int arg5, 
			 int arg6, int arg7, int arg8, int arg9, 
			 int arg10);
extern STATUS taskDelete( TASK_ID task );
extern STATUS taskDelay( int delay );
extern STATUS taskSuspend( TASK_ID task );
extern STATUS taskResume( TASK_ID task );
extern BOOL taskIsReady( TASK_ID task );
extern BOOL taskIsSuspended( TASK_ID task );
extern BOOL taskIdVerify( TASK_ID task );
extern STATUS taskRestart( TASK_ID task );
extern STATUS taskPrioritySet( TASK_ID task, INT32 newPriority );
extern STATUS taskPriorityGet( TASK_ID task, INT32 * pPriority );
extern TASK_ID taskNameToId( /*const*/ char *name );
extern TASK_ID taskIdSelf( void );
extern STATUS taskSafe( void );
extern STATUS taskUnsafe( void );

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

#define	WAIT_NONE ((UINT32)0)

extern SEM_ID semBCreate( int options, int initialState );
extern SEM_ID semMCreate( int options );
extern STATUS semTake( SEM_ID sem, int waitTime );
extern STATUS semGive( SEM_ID sem );
extern STATUS semFlush( SEM_ID sem );
extern STATUS semDelete( SEM_ID sem );

extern STATUS taskKill( TASK_ID task, int signal );

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
