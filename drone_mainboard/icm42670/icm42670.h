#ifndef __ICM42670
#define __ICM42670

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/i2c.h>

#define ICM42670_NODE DT_NODELABEL(icm42670_device)
#define ACCEL_SENS 4096

#define MCLK_RDY        0x00
#define GYRO_CONFIG0    0x20
#define ACCEL_CONFIG0   0x21
#define GYRO_CONFIG1    0x23
#define ACCEL_CONFIG1   0x24

#define ACCEL_DATA_X1   0x0B
#define ACCEL_DATA_X0   0x0C

#define ACCEL_DATA_Y1   0x0D
#define ACCEL_DATA_Y0   0x0E



#define ACCEL_DATA_Z1   0x0F
#define ACCEL_DATA_Z0   0x10


#define PWR_MGMT0       0x1F
#define WHO_AM_I        0x75


typedef enum
{
    ICM42670_OK,
    ICM42670_ERR,
    ICM42670_GYROCFG_ERR,
    ICM42670_GYROCFG_OK,
    ICM42670_ACCELCFG_ERR,
    ICM42670_ACCELCFG_OK,
    ICM42670_POWER_OK,
    ICM42670_POWER_ERR
}ICM42670_err_t;

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
void icm42670_get_acceldata(const struct i2c_dt_spec* dev_i2c,ICM42670_accelData* accelData);
void icm42670_get_gyrodata(const struct i2c_dt_spec* dev_i2c,ICM42670_gyroData* gyroData);

#endif