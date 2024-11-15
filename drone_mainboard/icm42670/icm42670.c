#include "icm42670.h"

static ICM42670_err_t icm42670_check_mclck(const struct i2c_dt_spec *dev_i2c)
{

    uint8_t reg_value;
    int ret;

    // Check mclk
    /*
    1: Indicates internal clock is currently running
    */
    ret = i2c_reg_read_byte_dt(dev_i2c, MCLK_RDY, &reg_value);

    if (ret != 0 || reg_value != 0x01)
    {
        printk("MCLK IS NOT READY: 0x%02x\n", reg_value);
        return ICM42670_ERR;
    }

    return ICM42670_OK;
}

/*
Setting gyro configurations from the app.overlay file
*/
static ICM42670_err_t icm42670_set_gyrocfg(const struct i2c_dt_spec *dev_i2c)
{
    /*
    If they are static that means they are live entire life of the process and can be changed from where they defined.
    Since we are setting once we should declare them as local variables
    */
    const int gyro_hz = DT_PROP(ICM42670_NODE, gyro_hz);
    const int gyro_fs = DT_PROP(ICM42670_NODE, gyro_fs);

    uint8_t reg_value = 0;
    //Setting gyro hz
    switch (gyro_hz)
    {
        case 12: // 12.5 Hz
            reg_value |= (1UL << 3) | (1UL << 2);
            break;
        case 25: //25 Hz
            reg_value |= (1UL << 3) | (1UL << 1) | (1UL << 0);
            break;
        case 50:
            reg_value |= (1UL << 3) | (1UL << 1);
            break;
        case 100:
            reg_value |= (1UL << 3) | (1UL << 0);
            break;
        case 200:
            reg_value |= (1UL << 3);
            break;
        case 400:
            reg_value |= (1UL << 2) | (1UL << 1) | (1UL << 0);
            break;
        case 800:
            reg_value |= (1UL << 2) | (1UL << 1);
            break;
        case 1600:
            reg_value |= (1UL << 2) | (1UL << 0);
            break;

        default:

            break;
    }

    // gyro_fs setting
    switch(gyro_fs)
    {
        case 250:
            reg_value |= (1UL<<5) | (1UL<<6);
            break;
        case 500:
            reg_value |= (1UL<<6);
            break;
        case 1000:
            reg_value |= (1UL<<5);
            break;
        case 2000:
            reg_value |= (0UL<<6) | (0UL<<5);
            break;
    }

    if(i2c_reg_write_byte_dt(dev_i2c,ACCEL_CONFIG0,reg_value)!= 0)
    {
        return ICM42670_GYROCFG_ERR;
    }

    //Clearing reg value
    reg_value = 0;
    //Setting BW to 73 Hz
    reg_value |= (1UL<<0) | (1UL<<1);
    if(i2c_reg_write_byte_dt(dev_i2c,GYRO_CONFIG1,reg_value)!= 0)
    {
        return ICM42670_GYROCFG_ERR;
    }
    return ICM42670_GYROCFG_OK;
    
}

//Setting Acceleration Config from app.overlay file
static ICM42670_err_t icm42670_set_accelcfg(const struct i2c_dt_spec *dev_i2c)
{
    /*
    If they are static that means they are live entire life of the process and can be changed from where they defined.
    Since we are setting once we should declare them as local variables
    */
const int accel_fs = DT_PROP(ICM42670_NODE, accel_fs);
const int accel_hz = DT_PROP(ICM42670_NODE, accel_hz);

    uint8_t reg_value = 0;
    //Setting acceleration hz
    switch (accel_hz)
    {
        case 12: // 12.5 Hz
            reg_value |= (1UL << 3) | (1UL << 2);
            break;
        case 25: //25 Hz
            reg_value |= (1UL << 3) | (1UL << 1) | (1UL << 0);
            break;
        case 50:
            reg_value |= (1UL << 3) | (1UL << 1);
            break;
        case 100:
            reg_value |= (1UL << 3) | (1UL << 0);
            break;
        case 200:
            reg_value |= (1UL << 3);
            break;
        case 400:
            reg_value |= (1UL << 2) | (1UL << 1) | (1UL << 0);
            break;
        case 800:
            reg_value |= (1UL << 2) | (1UL << 1);
            break;
        case 1600:
            reg_value |= (1UL << 2) | (1UL << 0);
            break;

        default:

            break;
    }


    switch(accel_fs)
    {
        case 2:
            reg_value |= (1UL<<6) | (1UL<<5);
            break;
        case 4:
            reg_value |= (1UL<<6);
            break;
        case 8:
            reg_value |= (1UL<<5);
            break;
        case 16:
            reg_value |= (0UL<<6) | (0UL<<5);
            break;
    }

    if(i2c_reg_write_byte_dt(dev_i2c,ACCEL_CONFIG0,reg_value)!= 0)
    {
        return ICM42670_ACCELCFG_ERR;
    }

    reg_value = 0;
    reg_value |= (1UL<<0) | (1UL<<1);

    if(i2c_reg_write_byte_dt(dev_i2c,ACCEL_CONFIG1,reg_value)!= 0)
    {
        return ICM42670_ACCELCFG_ERR;
    }

    return ICM42670_ACCELCFG_OK;
    
}



static ICM42670_err_t icm42670_power_on(const struct i2c_dt_spec *dev_i2c)
{
    uint8_t reg_value = 0;
    k_busy_wait(200);
    //ACCEL_MODE[1:0] : 11: Places accelerometer in Low Noise (LN) Mode
    reg_value |= (1UL<<0) | (1UL<<1);
    //GYRO_MODE [3:2] : 11: Places gyroscope in Low Noise (LN) Mode
    reg_value |= (1UL<<2) | (1UL<<3);
    if(i2c_reg_write_byte_dt(dev_i2c,PWR_MGMT0,reg_value)!= 0)
    {
        return ICM42670_POWER_ERR;
    }
    //When transitioning from OFF to any of the other modes, do not issue any register writes for 200 µs.
    k_busy_wait(200);
    return ICM42670_POWER_OK;
}



ICM42670_err_t icm42670_init(const struct i2c_dt_spec *dev_i2c)
{
    k_msleep(200);
    uint8_t reg_value;
    int ret;
    if(icm42670_check_mclck(dev_i2c) != ICM42670_OK)
    {
        return ICM42670_ERR;
    }

    // Check WHO_AM_I register
    ret = i2c_reg_read_byte_dt(dev_i2c, WHO_AM_I, &reg_value);
    /*
    This register is used to verify the identity of the device. The contents of WHOAMI is an 8-bit device ID. The default
    value of the register is 0x67. This is different from the I 2C address of the device as seen on the slave I2C controller
    by the applications processor.
    */
    if(ret != 0 || reg_value != 0x67)
    {
        return ICM42670_ERR;
    }

    //Setting Gyro Configurations from the devicetree
    if(icm42670_set_gyrocfg(dev_i2c) != ICM42670_GYROCFG_OK )
    {
        return ICM42670_ERR;
    }

  
    // Setting Acceleration configurations from the devicetree
    if(icm42670_set_accelcfg(dev_i2c) != ICM42670_ACCELCFG_OK )
    {
        return ICM42670_ERR;
    }

    //Powering the device
    if(icm42670_power_on(dev_i2c) != ICM42670_POWER_OK )
    {
        return ICM42670_ERR;
    }

    reg_value = 0;
    
    

    return ICM42670_OK;
}




