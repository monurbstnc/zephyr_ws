#include "icm42670.h"


ICM42670_err_t icm42670_init(const struct i2c_dt_spec* dev_i2c)
{
    k_msleep(100);
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
    if (ret != 0 || reg_value != 0x67)
    {
        printk("Unexpected WHO_AM_I response: 0x%02x\n", reg_value);
        return ICM42670_ERR;
    }   


    // clearing reg_value 
    reg_value = 0;
    // ACCEL_MODE [1:0] : 11: Places accelerometer in Low Noise (LN) Mode
    reg_value |= (1UL<<0) | (1UL<<1);
    // ACCEL_MODE [3:2] : 11: Places gyroscope in Low Noise (LN) Mode
    reg_value |= (1UL<<3) | (1UL<<2);

    return ICM42670_OK;
}

static ICM42670_err_t icm42670_check_mclck(const struct i2c_dt_spec* dev_i2c)
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



