#ifndef __ICM42670
#define __ICM42670

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define WHO_AM_I 0x75

typedef enum
{
    ICM42670_OK,
    ICM42670_ERR
} ICM42670_err_t;

typedef struct
{
    float accel_x;
    float accel_y;
    float accel_z;
} ICM42670_accelData;

typedef struct
{
    float gyro_x;
    float gyro_y;
    float gyro_z;
} ICM42670_gyroData;

typedef struct
{
    float temperature;
} ICM42670_tempData;


ICM42670_err_t icm42670_init(const struct i2c_dt_spec* dev_i2c);

#endif