/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_OS_EN == 1)
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include <stdlib.h>
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
    UBaseType_t saved = taskENTER_CRITICAL_FROM_ISR();
    return (host_os_critical_flag_t)saved;
}


void host_os_exit_critical(host_os_critical_flag_t flag)
{
    taskEXIT_CRITICAL_FROM_ISR((UBaseType_t)flag);
}


host_os_time_t host_os_timestamp_us(void)
{
    return (host_os_time_t)esp_timer_get_time();
}


void * host_os_malloc(uint32_t size)
{
    return malloc(size);
}


void host_os_free(void *pmem)
{
    free(pmem);
}


host_os_thread_handle_t host_os_thread_create(
    const char *pname,
    uint32_t stack_size,
    host_os_thread_entry_t entry,
    void *arg, int32_t prio
)
{
    TaskHandle_t handle = NULL;
    BaseType_t ret = xTaskCreate(
        entry,
        pname,
        stack_size / 4,
        arg,
        (UBaseType_t)prio,
        &handle
    );
    if (ret == pdPASS) {
        return (host_os_thread_handle_t)handle;
    }
    return NULL;
}


int host_os_thread_delete(host_os_thread_handle_t thread)
{
    return HOST_ERRCODE_UNSUPPORT;
}


host_os_sem_t host_os_sem_create(uint32_t initial_count, uint32_t limit)
{
    return (host_os_sem_t)xSemaphoreCreateCounting(limit, initial_count);
}


int host_os_sem_delete(host_os_sem_t psem)
{
    if (psem != NULL) {
        vSemaphoreDelete((SemaphoreHandle_t)psem);
        return HOST_ERRCODE_SUCCESS;
    } else {
        return HOST_ERRCODE_INVALID_HANDLE;
    }
}


int host_os_sem_is_valid(host_os_sem_t psem)
{
    return (psem != NULL) ? 1 : 0;
}


int host_os_sem_take(host_os_sem_t psem, host_os_timeout_t timeout)
{
    if (psem != NULL) {
        TickType_t ticks;
        if (timeout == 0) {
            ticks = 0;
        } else if (timeout == HOST_OS_TIMEOUT_FOREVER) {
            ticks = portMAX_DELAY;
        } else {
            ticks = pdMS_TO_TICKS(timeout);
        }
        BaseType_t ret = xSemaphoreTake((SemaphoreHandle_t)psem, ticks);
        return (ret == pdPASS) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_TIMEOUT;
    } else {
        return HOST_ERRCODE_INVALID_HANDLE;
    }
}


int host_os_sem_give(host_os_sem_t psem)
{
    if (psem != NULL) {
        BaseType_t ret = xSemaphoreGive((SemaphoreHandle_t)psem);
        return (ret == pdPASS) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
    } else {
        return HOST_ERRCODE_INVALID_HANDLE;
    }
}


int host_os_delayms(uint32_t ms)
{
    if (ms == 0) {
        taskYIELD();
    } else {
        TickType_t ticks;
        if (ms == HOST_OS_TIMEOUT_FOREVER) {
            ticks = portMAX_DELAY;
        } else {
            ticks = pdMS_TO_TICKS(ms);
        }
        vTaskDelay(ticks);
    }
    return 0;
}


int host_os_delayus(uint32_t us)
{
    if (us > 0) {
        esp_rom_delay_us(us);
    }
    return 0;
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
