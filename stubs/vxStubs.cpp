// stubs for unimplemented VxWorks functions
#include "stubs.h"

TASK_ID taskIdSelf( void ) {
    return (TASK_ID) pthread_self();
}

STATUS taskSafe( void ) {
    fprintf(stderr, "taskSafe not implemented, ignored\n");
    return OK;
}

STATUS taskUnsafe( void ) {
    fprintf(stderr, "taskUnsafe not implemented, ignored\n");
    return OK;
}

STATUS taskDelete( TASK_ID task )
{
    return (pthread_kill((pthread_t)task, SIGKILL) == 0
	    && pthread_join((pthread_t)task, NULL) == 0) ? OK : ERROR;
}

STATUS taskIdVerify( TASK_ID task )
{
    return (pthread_kill((pthread_t)task, 0) == 0) ? OK : ERROR;
}

STATUS taskDelay( int delay )
{
    if (delay == 0) delay++;
    uint64_t ns = delay * 1000000000UL / sysClkRateGet();
    struct timespec ts;
    ts.tv_sec = ns / 1000000000UL;
    ts.tv_nsec = ns % 1000000000UL;
    nanosleep(&ts, NULL);

    return OK;
}

#ifndef __CYGWIN__

#include <execinfo.h>

void wpi_selfTrace()
{
#define MAXFRAMES 20
    void *callers[MAXFRAMES];
    int n = backtrace(callers, MAXFRAMES);
    fprintf(stderr, "================ stack trace ================\n");
    fprintf(stderr, "TASK: %u\n", taskIdSelf());
    if (n > 1) {
	char **symbols = backtrace_symbols(callers, n);
	while (n > 1) {
	    fprintf(stderr, "%s\n", symbols[--n]);
	}
	free(symbols);
    }
    fprintf(stderr, "=============================================\n");
}

#else  // __CYGWIN__

void wpi_selfTrace() { abort(); }

#endif // __CYGWIN__

///////////////////////////////////////////////////////////////////////////////
//
// loadLib/unldLib stubs needed to resolve the following block of code
// in RobotBase.cpp:
// 
///////////////////////////////////////////////////////////////////////////////
//
//  // Find the startup code module.
//  char moduleName[256];
//  moduleNameFindBySymbolName("FRC_UserProgram_StartupLibraryInit", moduleName);
//  MODULE_ID startupModId = moduleFindByName(moduleName);
//  if (startupModId != NULL)
//  {
//	    // Remove the startup code.
//	    unldByModuleId(startupModId, 0);
//	    printf("!!!   Error: Default code was still running... "
//		   "It was unloaded for you... Please try again.\n");
//	    return;
//  }
//

extern "C" {

STATUS moduleNameFindBySymbolName( const char* symbol, char* module )
{
    assert(strcmp(symbol, "FRC_UserProgram_StartupLibraryInit") == 0);
    module[0] = '\0';
    return ERROR;
}

MODULE_ID moduleFindByName( const char* name )
{
    return NULL;
}

STATUS unldByModuleId( MODULE_ID id, int options )
{
    return ERROR;
}

}; // extern "C"

///////////////////////////////////////////////////////////////////////////////
//
// stubs for WPILib error reporting in ErrorBase.cpp and Utility.cpp
// 
///////////////////////////////////////////////////////////////////////////////

extern "C" {

SYMTAB_ID sysSymTbl = (void *) 0;;
SYMTAB_ID statSymTbl = (void *) 1;

#ifndef __CYGWIN__

size_t strlcpy( char* dst, const char *src, size_t size )
{
    size_t srclen = strlen(src);
    if (srclen < size) {
	memcpy(dst, src, srclen+1);
    } else {
	memcpy(dst, src, size-1);
	dst[size-1] = '\0';
    }
    return srclen+1;
}

#endif // __CYGWIN__


STATUS symFindByValue( SYMTAB_ID table, UINT value,
		       char* name, int* pValue, SYM_TYPE* pType )
{
    if (table != statSymTbl) {
	// return "symbol not found", caller will have to display raw address
	return ERROR;
    }

    // statSymTbl contains names for VxWorks subsystem error codes
    switch (value) {
    case S_memLib_NOT_ENOUGH_MEMORY:
	strlcpy(name, "S_memLib_NOT_ENOUGH_MEMORY", MAX_SYS_SYM_LEN);
	break;
    case S_msgQLib_INVALID_MSG_LENGTH:
	strlcpy(name, "S_msgQLib_INVALID_MSG_LENGTH", MAX_SYS_SYM_LEN);
	break;
    case S_objLib_OBJ_DELETED:
	strlcpy(name, "S_objLib_OBJ_DELETED", MAX_SYS_SYM_LEN);
	break;
    case S_objLib_OBJ_ID_ERROR:
	strlcpy(name, "S_objLib_OBJ_ID_ERROR", MAX_SYS_SYM_LEN);
	break;
    case S_objLib_OBJ_TIMEOUT:
	strlcpy(name, "S_objLib_OBJ_TIMEOUT", MAX_SYS_SYM_LEN);
	break;
    case S_objLib_OBJ_UNAVAILABLE:
	strlcpy(name, "S_objLib_OBJ_UNAVAILABLE", MAX_SYS_SYM_LEN);
	break;
    case S_semLib_INVALID_OPERATION:
	strlcpy(name, "S_semLib_INVALID_OPERATION", MAX_SYS_SYM_LEN);
	break;
    case S_smObjLib_NOT_INITIALIZED:
	strlcpy(name, "S_smObjLib_NOT_INITIALIZED", MAX_SYS_SYM_LEN);
	break;
    case S_taskLib_ILLEGAL_PRIORITY:
	strlcpy(name, "S_taskLib_ILLEGAL_PRIORITY", MAX_SYS_SYM_LEN);
	break;
    case S_taskLib_ILLEGAL_OPTIONS:
	strlcpy(name, "S_taskLib_ILLEGAL_OPTIONS", MAX_SYS_SYM_LEN);
	break;
    case S_taskLib_ILLEGAL_OPERATION:
	strlcpy(name, "S_taskLib_ILLEGAL_OPERATION", MAX_SYS_SYM_LEN);
	break;
    default:
	strerror_r(value, name, MAX_SYS_SYM_LEN);
	break;
    }
    *pValue = (int) value;
    *pType = 0;
    return OK;
}

}; // extern "C"
