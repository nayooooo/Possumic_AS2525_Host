#include <stdio.h>

#include "Possumic_AS2525_HostDriver.h"

static DEV_HANDLE s_devHandle = DEV_HANDLE_INVALID;

static void C6_Report_Handle(uint32_t msg_id, uint8_t *data, uint32_t data_len, void *cb_arg)
{
    if (msg_id == 0xC6) {
        if (data != NULL && data_len > 0) {
            HOST_LOG_INF_HEX(data, ((data_len < 128) ? data_len : 128), "dev(%p) data len=%u", cb_arg, data_len);
        }
    }
}

void app_main(void)
{
    int status = HOST_ERRCODE_SUCCESS;
    DevHw_t devHwCfg = {
        .bus_speed        = 32000000,  // 32M
        .bus_type         = LLC_BUS_TYPE_SPI,
        .bus_id           = 2,
        .bus_param        = LLC_BUS_PARAM_SPI,
        .bus_event_method = LLC_BUS_EVENT_METHOD_ORDER,
        .upd_io           = 25,
        .rst_io           = 26,
        .notify_io        = 9,
        .notify_type      = LLC_NOTIFY_TYPE_EDGE,
        .upload_type      = LLC_UPLOAD_TYPE_PASSIVE,
        .param            = 10,
    };
    devHwCfg.param = (devHwCfg.param & 0x3F) | ((devHwCfg.bus_speed & 0x3FFFFFF) << 6);
    HifCfg_t hifCfg = {
        .cmd_buf_len      = 1024,
    };

    do {
        status = Host_Driver_Init();
        if (status != HOST_ERRCODE_SUCCESS) {
            HOST_LOG_ERR("init host driver fail(%d)\n", status);
            break;
        }
        HOST_LOG_INF("init host driver succ\n");

        status = Host_Device_Regist(&s_devHandle, &devHwCfg, &hifCfg);
        if (status != HOST_ERRCODE_SUCCESS) {
            HOST_LOG_ERR("regist a device fail(%d)\n", status);
            break;
        }
        HOST_LOG_INF("regist device(%p) succ\n", s_devHandle);

        status = Mmw_C6_GeneralData_Handle_Set(s_devHandle, C6_Report_Handle, s_devHandle);
        if (status != HOST_ERRCODE_SUCCESS) {
            HOST_LOG_ERR("set C6 handle fail(%d)\n", status);
            break;
        }
        HOST_LOG_INF("set C6 handle succ\n");

        status = Host_Device_Open(s_devHandle);
        if (status != HOST_ERRCODE_SUCCESS) {
            HOST_LOG_ERR("open device(%p) fail(%d)\n", s_devHandle, status);
            break;
        }
        HOST_LOG_INF("open device(%p) succ\n", s_devHandle);
    } while (0);

    if (status == HOST_ERRCODE_SUCCESS) {
        HOST_LOG_INF("test SUCCESS\n");
    } else {
        HOST_LOG_ERR("test FAIL\n");
    }
}