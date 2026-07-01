/* Includes.
 * ------------------------------------------------------------------------------------------------
 */
#include "../host_port.h"

#if (CFG_HOST_PORT_IO_EN == 1)
#include "driver/gpio.h"
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
    esp_err_t status = ESP_OK;

    if (is_wkio) {
        return HOST_ERRCODE_UNSUPPORT;
    } else {
        gpio_config_t config = {
            .pin_bit_mask  = BIT(io),
            .mode          = GPIO_MODE_DISABLE,
            .pull_up_en    = GPIO_PULLUP_DISABLE,
            .intr_type     = GPIO_INTR_DISABLE,
#if SOC_GPIO_SUPPORT_PIN_HYS_FILTER
            .hys_ctrl_mode = GPIO_HYS_SOFT_DISABLE,
#endif
        };
        if (mode == HOST_IO_MODE_DISABLE) {
            // do noting
        } else if (mode == HOST_IO_MODE_OUTPUT) {
            config.mode = GPIO_MODE_OUTPUT;
        } else if (mode == HOST_IO_MODE_INPUT) {
            config.mode = GPIO_MODE_INPUT;
        } else if (mode == HOST_IO_MODE_INTERRUPT) {
            config.mode = GPIO_MODE_INPUT;
            if (pull == HOST_IO_PULL_DOWN) {
                config.intr_type = GPIO_INTR_POSEDGE;
            } else {
                config.intr_type = GPIO_INTR_NEGEDGE;
            }
        } else {
            HOST_LOG_ERR("%s: mode=%d is invalid!\n", __func__, mode);
            return HOST_ERRCODE_INVALID_PARAM;
        }
        if (pull == HOST_IO_PULL_NO) {
            // do noting
        } else if (pull == HOST_IO_PULL_UP) {
            config.pull_up_en = GPIO_PULLUP_ENABLE;
        } else if (pull == HOST_IO_PULL_DOWN) {
            config.pull_down_en = GPIO_PULLDOWN_ENABLE;
        } else {
            HOST_LOG_ERR("%s: pull=%d is invalid!\n", __func__, pull);
            return HOST_ERRCODE_INVALID_PARAM;
        }
        status = gpio_config(&config);
    }

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}


void host_io_write(uint32_t io, host_io_value_t value, bool is_wkio)
{
    esp_err_t status = ESP_OK;

    if (is_wkio) {
        return ;
    } else {
        status = gpio_set_level((gpio_num_t)io, (value == HOST_IO_VALUE_LOW) ? 0 : 1);
        if (status != ESP_OK) {
            HOST_LOG_ERR("%s: set IO%u to %s fail(%d)\n",
                __func__, io, (value == HOST_IO_VALUE_LOW) ? "LOW" : "HIGH", status);
            return ;
        }
    }
}


host_io_value_t host_io_read(uint32_t io, bool is_wkio)
{
    if (is_wkio) {
        return HOST_ERRCODE_UNSUPPORT;
    } else {
        return (gpio_get_level(io) == 1) ? HOST_IO_VALUE_HIGH : HOST_IO_VALUE_LOW;
    }
}


static void ensure_isr_service_installed(void)
{
    static bool installed = false;
    if (!installed) {
        gpio_install_isr_service(0);
        installed = true;
    }
}


int host_io_irq_cfg(uint32_t io, host_io_irq_trig_t trig, host_io_irq_callback_t pcallback, void *arg, bool is_wkio)
{
    esp_err_t status = ESP_OK;

    if (is_wkio) {
        return HOST_ERRCODE_UNSUPPORT;
    } else {
        host_io_pull_t pull;
        if (trig == HOST_IO_IRQ_EDGE_RISING) {
            pull = HOST_IO_PULL_DOWN;
        } else if (trig == HOST_IO_IRQ_EDGE_FALLING) {
            pull = HOST_IO_PULL_UP;
        } else if (trig == HOST_IO_IRQ_EDGE_BOTH) {
            pull = HOST_IO_PULL_DOWN;
        } else if (trig == HOST_IO_IRQ_LEVEL_HIGH) {
            pull = HOST_IO_PULL_DOWN;
        } else if (trig == HOST_IO_IRQ_LEVEL_LOW) {
            pull = HOST_IO_PULL_UP;
        } else {
            HOST_LOG_ERR("%s: trig=%d is invalid!\n", __func__, trig);
            return HOST_ERRCODE_INVALID_PARAM;
        }
        int ret = host_io_mode_set(io, HOST_IO_MODE_INTERRUPT, pull, is_wkio);
        if (ret != HOST_ERRCODE_SUCCESS) {
            HOST_LOG_ERR("%s: set mode fail(%d)\n", __func__, ret);
            return ret;
        }

        ensure_isr_service_installed();

        do {
            status = gpio_isr_handler_remove(io);
            if (status != ESP_OK) {
                HOST_LOG_ERR("%s: remove IO%d last isr handler fail(%d)\n", __func__, io, status);
                break;
            }

            status = gpio_isr_handler_add(io, pcallback, arg);
            if (status != ESP_OK) {
                HOST_LOG_ERR("%s: add IO%d isr handler fail(%d)\n", __func__, io, status);
                break;
            }

            status = gpio_intr_disable(io);
            if (status != ESP_OK) {
                HOST_LOG_ERR("%s: disable IO%d intr fail(%d)\n", __func__, io, status);
                break;
            }
        } while (0);
    }

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}


int host_io_irq_enable(uint32_t io, bool is_wkio)
{
    esp_err_t status = ESP_OK;

    if (is_wkio) {
        return HOST_ERRCODE_UNSUPPORT;
    } else {
        status = gpio_intr_enable(io);
    }

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}


int host_io_irq_disable(uint32_t io, bool is_wkio)
{
    esp_err_t status = ESP_OK;

    if (is_wkio) {
        return HOST_ERRCODE_UNSUPPORT;
    } else {
        status = gpio_intr_disable(io);
    }

    return (status == ESP_OK) ? HOST_ERRCODE_SUCCESS : HOST_ERRCODE_IO_ERROR;
}
#endif  /* CFG_HOST_PORT_IO_EN == 1 */
