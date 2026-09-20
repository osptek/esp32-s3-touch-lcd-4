#pragma once

#include "driver/i2c.h"
#include "esp_io_expander.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PCA9557_I2C_ADDRESS    0x19
#define PCA9557_LCD_BACKLIGHT  IO_EXPANDER_PIN_NUM_6
#define PCA9557_LCD_CS         IO_EXPANDER_PIN_NUM_7

/**
 * @brief Create a PCA9557 handle on an already initialized legacy I2C bus.
 */
esp_err_t pca9557_new_i2c(i2c_port_t i2c_port, uint8_t address,
                          esp_io_expander_handle_t *out_handle);

#ifdef __cplusplus
}
#endif
