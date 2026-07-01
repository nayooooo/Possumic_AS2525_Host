#include <stdio.h>

#include "Possumic_AS2525_HostDriver.h"

static DEV_HANDLE s_devHandle = DEV_HANDLE_INVALID;

void app_main(void)
{
    int status = HOST_ERRCODE_SUCCESS;
    DevHw_t devHwCfg = {};
    HifCfg_t hifCfg = {};

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