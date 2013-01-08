#include "stubs.h"

// an object that can emulate either a VxWorks mutex or binary semaphore

class vxsem {
public:
    enum sem_type_t { SEM_MUTEX, SEM_BINARY } m_type;

private:
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    pthread_t m_owner;
    int m_count;

public:
    vxsem( sem_type_t type, bool taken = false ) :
	m_type( type ),
     // m_mutex( PTHREAD_MUTEX_INITIALIZER ),
     // m_cond( PTHREAD_COND_INITIALIZER ),
	m_owner( 0 ),
	m_count( 0 )
    {
	pthread_mutex_init( &m_mutex, NULL );
	pthread_cond_init( &m_cond, NULL ); 

	if (taken) {
	    m_count = 1;
	    m_owner = pthread_self();
	}
    }

    ~vxsem() {
	pthread_mutex_destroy( &m_mutex );
	pthread_cond_destroy( &m_cond );
    }

    STATUS take( int wait_time )
    {
	timespec timeout;

	if (wait_time >= 0) {
	    // TBD: replace sysClkRateGet() and expensive 64-bit division
	    // with something simpler...
	    clock_gettime(CLOCK_REALTIME, &timeout);
	    uint64_t ns = timeout.tv_sec * 1000000000UL + timeout.tv_nsec +
			  wait_time * 1000000000UL / sysClkRateGet();
	    timeout.tv_sec = ns / 1000000000UL;
	    timeout.tv_nsec = ns % 1000000000UL;
	}

	pthread_mutex_lock( &m_mutex );
	while ((m_count > 0) &&
	  !(m_type == SEM_MUTEX && pthread_equal(m_owner, pthread_self()))) {
	    if ((m_type == SEM_MUTEX) && pthread_kill(m_owner, 0) != 0) {
		fprintf(stderr, "mutex: absentee owner\n");
		m_count = 0;
		m_owner = (pthread_t) 0;
		break;
	    }
	    int result;
	    if (wait_time >= 0) {
		result = pthread_cond_timedwait(&m_cond, &m_mutex, &timeout);
	    } else {
		result = pthread_cond_wait(&m_cond, &m_mutex);
	    }
	    if (result != 0) {
		// assume ETIMEDOUT
		pthread_mutex_unlock( &m_mutex );
		return ERROR;
	    }
	}
	m_count++;
	m_owner = pthread_self();
	pthread_mutex_unlock( &m_mutex );
	return OK;
    }

    STATUS give()
    {
	pthread_mutex_lock( &m_mutex );
	if (m_type == SEM_MUTEX && !pthread_equal(m_owner, pthread_self())) {
	    fprintf(stderr, "mutex: given by non-owner\n");
	    pthread_mutex_unlock( &m_mutex );
	    return ERROR;
	}
	if (--m_count <= 0) {
	    if ((m_type == SEM_MUTEX) && (m_count < 0)) {
		fprintf(stderr, "mutex: more give() than take()!\n");
	    }
	    m_count = 0;
	    m_owner = (pthread_t) 0;
	    pthread_cond_signal( &m_cond );
	    pthread_mutex_unlock( &m_mutex );
	}
	return OK;
    }

    STATUS flush()
    {
	pthread_mutex_lock( &m_mutex );
	m_count = 0;
	m_owner = 0;
	pthread_cond_signal( &m_cond );
	pthread_mutex_unlock( &m_mutex );
	return OK;
    }

private:
    vxsem(const vxsem&);               \
    void operator=(const vxsem&);
};

extern "C" SEM_ID semMCreate( int options )
{
    return (SEM_ID) new vxsem( vxsem::SEM_MUTEX );
}

extern "C" SEM_ID semBCreate( int options, int full )
{
    return (SEM_ID) new vxsem( vxsem::SEM_BINARY, !full );
}

extern "C" STATUS semDelete( SEM_ID sem )
{
    delete (vxsem *)sem;
    return OK;
}

extern "C" STATUS semTake( SEM_ID sem, int wait_time )
{
    return ((vxsem *)sem)->take( wait_time );
}

extern "C" STATUS semGive( SEM_ID sem )
{
    return ((vxsem *)sem)->give();
}

extern "C" STATUS semFlush( SEM_ID sem )
{
    return ((vxsem *)sem)->flush();
}

