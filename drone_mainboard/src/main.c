#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <stdio.h>
#include <stdlib.h>
#include <zephyr/logging/log.h>

#ifdef CONFIG_ICM42670
#include "../icm42670/icm42670.h"
#endif

#define ICM42670_NODE DT_NODELABEL(icm42670_device)

// Reading device tree properties
static const int accel_hz = DT_PROP(ICM42670_NODE, accel_hz);
static const int gyro_hz = DT_PROP(ICM42670_NODE, gyro_hz);
static const int accel_fs = DT_PROP(ICM42670_NODE, accel_fs);
static const int gyro_fs = DT_PROP(ICM42670_NODE, gyro_fs);
const struct i2c_dt_spec icm42670_dev = I2C_DT_SPEC_GET(ICM42670_NODE);


/* The devicetree node identifier for the "led0" alias. */
#define LED1_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
int main()
{       
    int ret;
    if (!device_is_ready(icm42670_dev.bus)) {
        printk("ICM42670 device is not ready\n");
        return -1;
    }
    

	if (!gpio_is_ready_dt(&led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	gpio_pin_set_dt(&led,1);
    return 0;
}