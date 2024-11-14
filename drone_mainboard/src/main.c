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


#define REDLED_NODE DT_ALIAS(led2)
static const struct gpio_dt_spec red_led = GPIO_DT_SPEC_GET(REDLED_NODE, gpios);

#define GREENLED_NODE DT_ALIAS(led0)
static const struct gpio_dt_spec green_led = GPIO_DT_SPEC_GET(GREENLED_NODE, gpios);




int main()
{       
    int ret;
	if (!gpio_is_ready_dt(&red_led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&red_led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	gpio_pin_set_dt(&red_led,0);

	//

		if (!gpio_is_ready_dt(&green_led)) {
		return 0;
	}

	ret = gpio_pin_configure_dt(&green_led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0) {
		return 0;
	}

	gpio_pin_set_dt(&green_led,0);

    if (!device_is_ready(icm42670_dev.bus)) {
        printk("ICM42670 device is not ready\n");
		gpio_pin_set_dt(&red_led,1);
        return -1;
    }


	if(icm42670_init(&icm42670_dev) != ICM42670_OK)
	{
		gpio_pin_set_dt(&red_led,1);
		return -1;
	}

	gpio_pin_set_dt(&green_led,1);
	k_msleep(2000);
	gpio_pin_set_dt(&green_led,0);

    return 0;
}