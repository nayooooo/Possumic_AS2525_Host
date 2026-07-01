/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_IO_EN == 1)
#endif  /* CFG_HOST_PORT_IO_EN == 1 */


/* Local Variable.
 * ------------------------------------------------------------------------------------------------
 */
#if (CFG_HOST_PORT_IO_EN == 1)
#endif  /* CFG_HOST_PORT_IO_EN == 1 */


/* Local Function Declaration.
 * ------------------------------------------------------------------------------------------------
 */
/* Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if (CFG_HOST_PORT_IO_EN == 1)
int host_io_mode_set(uint32_t io, host_io_mode_t mode, host_io_pull_t pull, bool is_wkio)
{
    return HOST_ERRCODE_UNSUPPORT;
}


void host_io_write(uint32_t io, host_io_value_t value, bool is_wkio)
{
}


host_io_value_t host_io_read(uint32_t io, bool is_wkio)
{
}


int host_io_irq_cfg(uint32_t io, host_io_irq_trig_t trig, host_io_irq_callback_t pcallback, void *arg, bool is_wkio)
{
    return HOST_ERRCODE_UNSUPPORT;
}


int host_io_irq_enable(uint32_t io, bool is_wkio)
{
    return HOST_ERRCODE_UNSUPPORT;
}


int host_io_irq_disable(uint32_t io, bool is_wkio)
{
    return HOST_ERRCODE_UNSUPPORT;
}
#endif  /* CFG_HOST_PORT_IO_EN == 1 */
