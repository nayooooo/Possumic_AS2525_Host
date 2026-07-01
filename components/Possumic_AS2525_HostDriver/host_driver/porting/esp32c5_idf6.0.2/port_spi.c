/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_COM_EN == 1)
#include "../include/hal_com.h"

#if (CFG_HOST_PORT_COM_SPI_EN == 1)
#endif  /* CFG_HOST_PORT_COM_SPI_EN == 1 */
#endif  /* CFG_HOST_PORT_COM_EN == 1 */


/* Private Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
static void * porting_port_spi_init(uint8_t id, uint32_t speed, void *arg);
static int porting_port_spi_deinit(void *bus);

static int porting_port_spi_open(void *bus, uint32_t cs_pin);
static int porting_port_spi_close(void *bus, uint32_t cs_pin);

static int porting_port_spi_write(void *bus, uint32_t cs_pin, void *pbuff, uint32_t size);

static int porting_port_spi_read(void *bus, uint32_t cs_pin, void *pbuff, uint32_t size);
#endif


/* Constants.
 * ------------------------------------------------------------------------------------------------
 */
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
static const host_com_ops_t spi_ops = {
    .init                    = porting_port_spi_init,
    .deinit                  = porting_port_spi_deinit,
    .open                    = porting_port_spi_open,
    .close                   = porting_port_spi_close,
    .write                   = porting_port_spi_write,
    .read                    = porting_port_spi_read,
};
#endif


/* Private var.
 * ------------------------------------------------------------------------------------------------
 */
/* Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
static void * porting_port_spi_init(uint8_t id, uint32_t speed, void *arg)
{
    return NULL;
}


static int porting_port_spi_deinit(void *bus)
{
    return HOST_ERRCODE_UNSUPPORT;
}


static int porting_port_spi_open(void *bus, uint32_t cs_pin)
{
    return HOST_ERRCODE_UNSUPPORT;
}


static int porting_port_spi_close(void *bus, uint32_t cs_pin)
{
    return HOST_ERRCODE_UNSUPPORT;
}


static int porting_port_spi_write(void *bus, uint32_t cs_pin, void *pbuff, uint32_t size)
{
    return 0;
}


static int porting_port_spi_read(void *bus, uint32_t cs_pin, void *pbuff, uint32_t size)
{
    return 0;
}
#endif


host_com_ops_t *porting_port_spi_get_com_ops(uint8_t id)
{
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
    if (id == 0) {
        return (host_com_ops_t *)&spi_ops;
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}
