/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_OS_EN == 1)
#endif  /* CFG_HOST_PORT_OS_EN == 1 */


/* Types.
 * ------------------------------------------------------------------------------------------------
 */
/* Constants.
 * ------------------------------------------------------------------------------------------------
 */
/* Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if (CFG_HOST_PORT_OS_EN == 1)
host_os_critical_flag_t host_os_enter_critical(void)
{
    return 0;
}


void host_os_exit_critical(host_os_critical_flag_t flag)
{
}


host_os_time_t host_os_timestamp_us(void)
{
    return 0;
}


void * host_os_malloc(uint32_t size)
{
    return NULL;
}


void host_os_free(void *pmem)
{
}


host_os_thread_handle_t host_os_thread_create(
    const char *pname,
    uint32_t stack_size,
    host_os_thread_entry_t entry,
    void *arg, int32_t prio
)
{
    return NULL;
}


int host_os_thread_delete(host_os_thread_handle_t thread)
{
    return HOST_ERRCODE_UNSUPPORT;
}


host_os_sem_t host_os_sem_create(uint32_t initial_count, uint32_t limit)
{
    return NULL;
}


int host_os_sem_delete(host_os_sem_t psem)
{
    return HOST_ERRCODE_UNSUPPORT;
}


int host_os_sem_is_valid(host_os_sem_t psem)
{
    return 0;
}


int host_os_sem_take(host_os_sem_t psem, host_os_timeout_t timeout)
{
    return HOST_ERRCODE_UNSUPPORT;
}


int host_os_sem_give(host_os_sem_t psem)
{
    return HOST_ERRCODE_UNSUPPORT;
}


int host_os_delayms(uint32_t ms)
{
    return HOST_ERRCODE_UNSUPPORT;
}


int host_os_delayus(uint32_t us)
{
    return HOST_ERRCODE_UNSUPPORT;
}


#if (CFG_HOST_PORT_PM_EN == 1)
void host_os_pm_lock(void)
{
}

void host_os_pm_unlock(void)
{
}
#endif  /* CFG_HOST_PORT_PM_EN == 1 */
#endif  /* CFG_HOST_PORT_OS_EN == 1 */
