/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_STORE_EN == 1)
#endif /* CFG_HOST_PORT_STORE_EN == 1 */


/* Local Variable.
 * ------------------------------------------------------------------------------------------------
 */
#if (CFG_HOST_PORT_STORE_EN == 1)
#endif /* CFG_HOST_PORT_STORE_EN == 1 */


/* Local constants.
 * ------------------------------------------------------------------------------------------------
 */
/* Macro.
 * ------------------------------------------------------------------------------------------------
 */
/* Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if (CFG_HOST_PORT_STORE_EN == 1)
int host_store_open(void **hdl, void *path, uint32_t *len)
{
    return HOST_ERRCODE_UNSUPPORT;
}


void host_store_close(void *hdl)
{
}


int host_store_read(void *hdl, uint32_t offset, void *buff, uint32_t size)
{
    return HOST_ERRCODE_UNSUPPORT;
}
#endif /* CFG_HOST_PORT_STORE_EN == 1 */
