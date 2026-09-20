#include "pca9557.h"

#include <stdlib.h>

#include "esp_check.h"
#include "esp_log.h"

#define PCA9557_REG_INPUT      0x00
#define PCA9557_REG_OUTPUT     0x01
#define PCA9557_REG_POLARITY   0x02
#define PCA9557_REG_CONFIG     0x03
#define PCA9557_I2C_TIMEOUT_MS 100

typedef struct {
    esp_io_expander_t base;
    i2c_port_t i2c_port;
    uint8_t address;
    uint8_t output;
    uint8_t direction;
} pca9557_t;

static const char *TAG = "pca9557";

static esp_err_t pca9557_write_reg(pca9557_t *dev, uint8_t reg, uint8_t value)
{
    const uint8_t data[] = {reg, value};
    return i2c_master_write_to_device(dev->i2c_port, dev->address, data, sizeof(data),
                                      pdMS_TO_TICKS(PCA9557_I2C_TIMEOUT_MS));
}

static esp_err_t pca9557_read_reg(pca9557_t *dev, uint8_t reg, uint8_t *value)
{
    return i2c_master_write_read_device(dev->i2c_port, dev->address, &reg, 1, value, 1,
                                        pdMS_TO_TICKS(PCA9557_I2C_TIMEOUT_MS));
}

static esp_err_t pca9557_read_input(esp_io_expander_handle_t handle, uint32_t *value)
{
    uint8_t input = 0;
    esp_err_t ret = pca9557_read_reg((pca9557_t *)handle, PCA9557_REG_INPUT, &input);
    *value = input;
    return ret;
}

static esp_err_t pca9557_write_output(esp_io_expander_handle_t handle, uint32_t value)
{
    pca9557_t *dev = (pca9557_t *)handle;
    ESP_RETURN_ON_ERROR(pca9557_write_reg(dev, PCA9557_REG_OUTPUT, (uint8_t)value),
                        TAG, "write output register");
    dev->output = (uint8_t)value;
    return ESP_OK;
}

static esp_err_t pca9557_read_output(esp_io_expander_handle_t handle, uint32_t *value)
{
    *value = ((pca9557_t *)handle)->output;
    return ESP_OK;
}

static esp_err_t pca9557_write_direction(esp_io_expander_handle_t handle, uint32_t value)
{
    pca9557_t *dev = (pca9557_t *)handle;
    ESP_RETURN_ON_ERROR(pca9557_write_reg(dev, PCA9557_REG_CONFIG, (uint8_t)value),
                        TAG, "write direction register");
    dev->direction = (uint8_t)value;
    return ESP_OK;
}

static esp_err_t pca9557_read_direction(esp_io_expander_handle_t handle, uint32_t *value)
{
    *value = ((pca9557_t *)handle)->direction;
    return ESP_OK;
}

static esp_err_t pca9557_reset(esp_io_expander_handle_t handle)
{
    pca9557_t *dev = (pca9557_t *)handle;

    ESP_RETURN_ON_ERROR(pca9557_write_reg(dev, PCA9557_REG_OUTPUT, 0xFF),
                        TAG, "reset output register");
    ESP_RETURN_ON_ERROR(pca9557_write_reg(dev, PCA9557_REG_POLARITY, 0x00),
                        TAG, "reset polarity register");
    ESP_RETURN_ON_ERROR(pca9557_write_reg(dev, PCA9557_REG_CONFIG, 0xFF),
                        TAG, "reset direction register");

    dev->output = 0xFF;
    dev->direction = 0xFF;
    return ESP_OK;
}

static esp_err_t pca9557_del(esp_io_expander_handle_t handle)
{
    free(handle);
    return ESP_OK;
}

esp_err_t pca9557_new_i2c(i2c_port_t i2c_port, uint8_t address,
                          esp_io_expander_handle_t *out_handle)
{
    ESP_RETURN_ON_FALSE(out_handle, ESP_ERR_INVALID_ARG, TAG, "invalid output handle");

    pca9557_t *dev = calloc(1, sizeof(pca9557_t));
    ESP_RETURN_ON_FALSE(dev, ESP_ERR_NO_MEM, TAG, "no memory");

    dev->i2c_port = i2c_port;
    dev->address = address;
    dev->base.read_input_reg = pca9557_read_input;
    dev->base.write_output_reg = pca9557_write_output;
    dev->base.read_output_reg = pca9557_read_output;
    dev->base.write_direction_reg = pca9557_write_direction;
    dev->base.read_direction_reg = pca9557_read_direction;
    dev->base.reset = pca9557_reset;
    dev->base.del = pca9557_del;
    dev->base.config.io_count = 8;
    dev->base.config.flags.dir_out_bit_zero = 1;

    esp_err_t ret = pca9557_read_reg(dev, PCA9557_REG_OUTPUT, &dev->output);
    if (ret == ESP_OK) {
        ret = pca9557_read_reg(dev, PCA9557_REG_CONFIG, &dev->direction);
    }
    if (ret != ESP_OK) {
        free(dev);
        return ret;
    }

    *out_handle = &dev->base;
    ESP_LOGI(TAG, "PCA9557 initialized at I2C address 0x%02X", address);
    return ESP_OK;
}
