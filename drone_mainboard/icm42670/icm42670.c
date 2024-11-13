#include "icm42670.h"


ICM42670_err_t icm42670_init(const struct i2c_dt_spec* dev_i2c)
{
    uint8_t reg_value;
    int ret;

    // Check WHO_AM_I register
    ret = i2c_reg_read_byte_dt(dev_i2c, WHO_AM_I, &reg_value);
    if (ret != 0 || reg_value != 0x68)
    {
        printk("Unexpected WHO_AM_I response: 0x%02x\n", reg_value);
        return ICM42670_ERR;
    }

    return ICM42670_OK;
}

