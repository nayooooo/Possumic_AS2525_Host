/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_COM_EN == 1)
#include "../include/hal_com.h"

#if (CFG_HOST_PORT_COM_SPI_EN == 1)
#include "driver/spi_master.h"
#endif  /* CFG_HOST_PORT_COM_SPI_EN == 1 */
#endif  /* CFG_HOST_PORT_COM_EN == 1 */


/* Private Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
static void * porting_port_spi_init(uint8_t id, uint32_t speed, void *arg);
static int porting_port_spi_deinit(void *bus);

static int porting_port_spi_open(void *bus, uint32_t cs_pin_L6_speed_H26);
static int porting_port_spi_close(void *bus, uint32_t cs_pin_L6_speed_H26);

static int porting_port_spi_write(void *bus, uint32_t cs_pin_L6_speed_H26, void *pbuff, uint32_t size);

static int porting_port_spi_read(void *bus, uint32_t cs_pin_L6_speed_H26, void *pbuff, uint32_t size);
#endif


/* Constants.
 * ------------------------------------------------------------------------------------------------
 */
#define USE_SPI_BUS_NUM        1
#define USE_SPI_DEVICE_NUM     1

#define DEFAULT_HOST           SPI2_HOST
#define DEFAULT_ID             2
#define DEFAULT_CLK_PIN        6
#define DEFAULT_MOSI_PIN       7
#define DEFAULT_MISO_PIN       2

#define GET_BUS_ID(bus)        ((((int)(bus)) == DEFAULT_HOST) ? 2 : -1)
#define BUS_ID_IS_VALID(id)    ((id) == DEFAULT_ID)
#define BUS_IS_VALID(bus)      (((int)(bus)) == DEFAULT_HOST)

// #define DEFAULT_DEV0_CS_PIN    10


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
static spi_device_handle_t s_spi_device_handle[USE_SPI_DEVICE_NUM] = {
    [0 ... USE_SPI_DEVICE_NUM-1] = NULL,
};
struct {
    int idx;
    uint8_t cs_pin;
    uint32_t speed;
} s_spi_device_mark[USE_SPI_DEVICE_NUM] = {
    [0 ... USE_SPI_DEVICE_NUM-1] = { -1, 0, 0 },
};
#define MARK_A_SPI_DEVICE(cs_pin, speed)            do {                        \
        host_os_critical_flag_t ___critical_flag___ = host_os_enter_critical(); \
        for (uint32_t ___i___ = 0; ___i___ < USE_SPI_DEVICE_NUM; ___i___++) {   \
            if (s_spi_device_mark[___i___].idx == -1) {                         \
                s_spi_device_mark[___i___].cs_pin = cs_pin;                     \
                s_spi_device_mark[___i___].speed  = speed;                      \
                s_spi_device_mark[___i___].idx    = ___i___;                    \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        host_os_exit_critical(___critical_flag___);                             \
    } while (0)
#define CLEAR_A_SPI_DEVICE(cs_pin)            do {                              \
        host_os_critical_flag_t ___critical_flag___ = host_os_enter_critical(); \
        for (uint32_t ___i___ = 0; ___i___ < USE_SPI_DEVICE_NUM; ___i___++) {   \
            if (s_spi_device_mark[___i___].cs_pin == cs_pin) {                  \
                s_spi_device_mark[___i___].cs_pin = 0;                          \
                s_spi_device_mark[___i___].speed  = 0;                          \
                s_spi_device_mark[___i___].idx    = -1;                         \
                break;                                                          \
            }                                                                   \
        }                                                                       \
        host_os_exit_critical(___critical_flag___);                             \
    } while (0)
static int SPI_DEVICE_HANDLE_IDX(uint8_t cs_pin)
{
    for (int i = 0; i < USE_SPI_DEVICE_NUM; i++) {
        if (s_spi_device_mark[i].cs_pin == cs_pin
            && s_spi_device_mark[i].idx >= 0
            && s_spi_device_mark[i].idx < USE_SPI_DEVICE_NUM) {
            return i;
        }
    }
    return -1;
}


/* Functions.
 * ------------------------------------------------------------------------------------------------
 */
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
static void porting_port_spi_pre_handle(spi_transaction_t *t)
{
}


static void * porting_port_spi_init(uint8_t id, uint32_t speed, void *arg)
{
    esp_err_t status = ESP_OK;
    spi_bus_config_t busCfg = {
        .mosi_io_num     = DEFAULT_MOSI_PIN,
        .miso_io_num     = DEFAULT_MISO_PIN,
        .sclk_io_num     = DEFAULT_CLK_PIN,
        .quadwp_io_num   = -1,
        .quadhd_io_num   = -1,
        .max_transfer_sz = 4000,
    };

    if (!BUS_ID_IS_VALID(id)) {
        return NULL;
    }

    do {
        status = spi_bus_initialize(DEFAULT_HOST, &busCfg, SPI_DMA_CH_AUTO);
        if (status != ESP_OK) {
            HOST_LOG_ERR("%s: init SPI%d bus fail(%d)\n", __func__, id, status);
            break;
        }
    } while (0);

    return (status == ESP_OK) ? (void *)DEFAULT_HOST : NULL;
}


static int porting_port_spi_deinit(void *bus)
{
    esp_err_t status = ESP_OK;

    if (!BUS_IS_VALID(bus)) {
        return HOST_ERRCODE_INVALID_HANDLE;
    }

    do {
        status = spi_bus_free((spi_host_device_t)bus);
        if (status != ESP_OK) {
            HOST_LOG_ERR("%s: free SPI%d bus fail(%d)\n", __func__, GET_BUS_ID(bus), status);
            break;
        }
    } while (0);

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}


static int porting_port_spi_open(void *bus, uint32_t cs_pin_L6_speed_H26)
{
    esp_err_t status = ESP_OK;
    uint8_t cs_pin = cs_pin_L6_speed_H26 & 0x3F;
    uint32_t speed = (cs_pin_L6_speed_H26 >> 6) & 0x3FFFFFF;
    spi_device_interface_config_t devCfg = {
        .clock_speed_hz = speed,
        .mode           = 0,
        .spics_io_num   = cs_pin,
        .queue_size     = 7,
        .pre_cb         = porting_port_spi_pre_handle,
    };

    if (!BUS_IS_VALID(bus)) {
        return HOST_ERRCODE_INVALID_HANDLE;
    }

    if (GET_BUS_ID(bus) == DEFAULT_ID) {
        do {
            MARK_A_SPI_DEVICE(cs_pin, speed);
            status = spi_bus_add_device(DEFAULT_HOST, &devCfg, &s_spi_device_handle[SPI_DEVICE_HANDLE_IDX(cs_pin)]);
            if (status != ESP_OK) {
                CLEAR_A_SPI_DEVICE(cs_pin);
                HOST_LOG_ERR("%s: SPI%d add device fail(%d)\n", __func__, GET_BUS_ID(bus), status);
                break;
            }
        } while (0);
    } else {
        return HOST_ERRCODE_INVALID_HANDLE;
    }

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}


static int porting_port_spi_close(void *bus, uint32_t cs_pin_L6_speed_H26)
{
    esp_err_t status = ESP_OK;
    uint8_t cs_pin = cs_pin_L6_speed_H26 & 0x3F;

    if (!BUS_IS_VALID(bus)) {
        return HOST_ERRCODE_INVALID_HANDLE;
    }

    if (GET_BUS_ID(bus) == DEFAULT_ID) {
        do {
            status = spi_bus_remove_device(s_spi_device_handle[SPI_DEVICE_HANDLE_IDX(cs_pin)]);
            if (status != ESP_OK) {
                HOST_LOG_ERR("%s: SPI%d remove device fail(%d)\n", __func__, GET_BUS_ID(bus), status);
                break;
            }
            s_spi_device_handle[SPI_DEVICE_HANDLE_IDX(cs_pin)] = NULL;
            CLEAR_A_SPI_DEVICE(cs_pin);
        } while (0);
    } else {
        return HOST_ERRCODE_INVALID_HANDLE;
    }

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}


static int porting_port_spi_write(void *bus, uint32_t cs_pin_L6_speed_H26, void *pbuff, uint32_t size)
{
    return 0;
}


static int porting_port_spi_read(void *bus, uint32_t cs_pin_L6_speed_H26, void *pbuff, uint32_t size)
{
    return 0;
}
#endif


host_com_ops_t *porting_port_spi_get_com_ops(uint8_t id)
{
#if ((CFG_HOST_PORT_COM_EN == 1) && (CFG_HOST_PORT_COM_SPI_EN == 1))
    if (id == DEFAULT_ID) {
        return (host_com_ops_t *)&spi_ops;
    } else {
        return NULL;
    }
#else
    return NULL;
#endif
}
