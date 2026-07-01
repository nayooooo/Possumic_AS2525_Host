/**
 **************************************************************************************************
 * @file    host_llc.h
 * @brief   host llc layer api header file.
 * @attention
 *          Copyright (c) 2026 Possumic Technology. all rights reserved.
 **************************************************************************************************
 */

/* Define to prevent recursive inclusion.
 * ------------------------------------------------------------------------------------------------
 */
#ifndef __HOST_LLC_H__
#define __HOST_LLC_H__

#ifdef __cplusplus
extern "C" {
#endif


/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../../porting/host_port.h"


/* Exported constants.
 * ------------------------------------------------------------------------------------------------
 */
/* Exported types.
 * ------------------------------------------------------------------------------------------------
 */
#define LLC_LOCAL
#define LLC_CALLBACK
// allways in interrupt function
#define LLC_LOCAL_IRQ
#define LLC_CALLBACK_IRQ


typedef void * LLC_HANDLE;


typedef void * (*LLC_Buffer_Get_CB)(uint32_t *size, uint32_t *timeout, void *arg);


/**
 * @brief LLC notify
 * 
 * @type LLC_NOTIFY_TYPE_EDGE,  param list: (NULL, 0, arg)
 *       LLC_NOTIFY_TYPE_LEVEL, param list: (NULL, 0, arg)
 *       LLC_NOTIFY_TYPE_DATA,  param list: (data, len, arg)
 * 
 * @function give data or notify the notify io is valid
 */
typedef void (*LLC_Notify_CB)(uint8_t *data, uint32_t len, void *arg);


/**
 * @brief device param
 * 
 * @type spi,  cs_pin, uint8_t
 *       i2c,  addr,   uint16_t
 *       uart, id,     uint8_t
 */

//typedef union {
//	uint8_t  uart_num;
//	uint8_t  spi_cs_pin;
//	uint16_t iic_addr;
//} DevParam;

typedef uint32_t DevParam;


typedef struct {
    uint32_t bus_speed;
#define LLC_BUS_TYPE_SPI                         0
#define LLC_BUS_TYPE_I2C                         1
#define LLC_BUS_TYPE_UART                        2
    uint8_t bus_type;
    uint8_t bus_id;
#define LLC_BUS_PARAM_SPI                        1
#define LLC_BUS_PARAM_DSPI                       2
#define LLC_BUS_PARAM_QSPI                       3
    uint8_t bus_param;
#define LLC_BUS_EVENT_METHOD_ORDER               0
#define LLC_BUS_EVENT_METHOD_PRIORITY            1
#define LLC_BUS_EVENT_METHOD_ORDER_BALANCE       2
#define LLC_BUS_EVENT_METHOD_PRIORITY_BALANCE    3
    uint8_t bus_event_method;
    uint8_t upd_io;
    uint8_t rst_io;
    uint8_t notify_io;
#define LLC_NOTIFY_TYPE_EDGE                     0
#define LLC_NOTIFY_TYPE_COM_ISR                  2
#define LLC_NOTIFY_TYPE_COM_IRQ_THREAD           3
    uint8_t notify_type;
#define LLC_UPLOAD_TYPE_ACTIVE                   1
#define LLC_UPLOAD_TYPE_PASSIVE                  2
#define LLC_UPLOAD_TYPE_ACTIVE_DELAY             3
    uint8_t upload_type;
    DevParam param;
} DevHw_t;


/* Exported Functions.
 * ------------------------------------------------------------------------------------------------
 */
void LLC_Print_Device_Tree(void);

int LLC_Init(void);
int LLC_Deinit(void);

/**
 * @brief regist a device and attach into bus, if bus not exist, will
 *        regist a bus first.
 * 
 * @retval LLC_HANDLE
 *     NULL  --> fail
 *     other --> succ
 */
LLC_HANDLE LLC_Device_Regist(DevHw_t *dev_cfg);
int LLC_Device_UnRegist(LLC_HANDLE dev);
int LLC_Device_Open(LLC_HANDLE dev);
int LLC_Device_Close(LLC_HANDLE dev);

DevHw_t *LLC_Device_Get_DevHwCfg(LLC_HANDLE dev);

int LLC_Device_Set_Bus_Param(LLC_HANDLE dev, uint8_t bus_param);
int LLC_Device_Set_Bus_Speed(LLC_HANDLE dev, uint32_t bus_speed);
int LLC_Device_Set_Device_Param(LLC_HANDLE dev, DevParam device_param);

int LLC_Device_Take_Bus(LLC_HANDLE dev, uint32_t timeout);
int LLC_Device_Release_Bus(LLC_HANDLE dev);
uint32_t LLC_Send(LLC_HANDLE dev, const uint8_t *data, uint32_t len, uint32_t timeout);
uint32_t LLC_Recv(LLC_HANDLE dev, uint8_t *buffer, uint32_t size, uint32_t timeout);
int LLC_Device_Recv_Finish(LLC_HANDLE dev);

int LLC_Notify_Type_Get(LLC_HANDLE dev, uint8_t *type);

int LLC_Buffer_Get_Handle_Regist(LLC_HANDLE dev, LLC_Buffer_Get_CB buffer_get, void *arg);
int LLC_Buffer_Get_Handle_Get(LLC_HANDLE dev, LLC_Buffer_Get_CB *buffer_get, void **arg);
/**
 * @param poll_period unit is us, not support now
 */
int LLC_Notify_Handle_Regist(LLC_HANDLE dev, LLC_Notify_CB cb, void *arg, uint32_t poll_period);
int LLC_Notify_Handle_Get(LLC_HANDLE dev, LLC_Notify_CB *cb, void **arg, uint32_t *poll_period);

int LLC_IO_Enable_Upd(LLC_HANDLE dev);
int LLC_IO_Disable_Upd(LLC_HANDLE dev);
int LLC_IO_Enable_Rst(LLC_HANDLE dev);
int LLC_IO_Disable_Rst(LLC_HANDLE dev);
int LLC_IO_Set_Upd(LLC_HANDLE dev, host_io_value_t level);
int LLC_IO_Set_Rst(LLC_HANDLE dev, host_io_value_t level);

host_io_value_t LLC_IO_Get_Notify(LLC_HANDLE dev);
int LLC_IO_Notify_Irq_Control(LLC_HANDLE dev, bool enable);
int LLC_IO_Notify_Irq_Update(LLC_HANDLE dev);


#ifdef __cplusplus
}
#endif

#endif /* __HOST_LLC_H__ */

