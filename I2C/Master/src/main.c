
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/i2c.h>
#include "main.h"

enum
{
	speed_100kHz = I2C_SPEED_STANDARD,
	speed_400kHz = I2C_SPEED_FAST,
	speed_1MHz = I2C_SPEED_FAST_PLUS
};

enum
{
	controller_mode_Master = I2C_MODE_MASTER,
	controller_mode_Slave = 0 
};

// port
const char *i2c0port = "I2C_0";

// device structure
const struct device *dev_i2c0;

// static valiable
uint8_t write_buf_data = 0x25;
uint8_t read_buf_data = 0x0;
uint8_t length = 1;
uint16_t Slave_address = 0x29;

static uint32_t i2c_cfg;

void i2c_configure_set(uint8_t i2c_speed, uint8_t controller_mode );
void write(uint8_t *write_buf, uint16_t slave_address, uint8_t massage_length);
void read(uint8_t *read_buf, uint16_t slave_address, uint8_t massage_length);

void main(void)
{
	k_sleep(K_SECONDS(1));
	printk("\n I2C Demo Start....\n");
	enable_i2c();
	i2c_configure_set(speed_100kHz,controller_mode_Master);
	write(write_buf_data, Slave_address, length);
	printk("\nTransmit Data :");
	printk("%d",write_buf_data);
    
	//k_sleep(K_MSEC(1));
    read(read_buf_data, Slave_address,length);
	printk("\nReceived Data :");
	printk("%d",read_buf_data);	
}
/*******************************************************************
See header file for function definition
********************************************************************/

void write(uint8_t *write_buf, uint16_t slave_address, uint8_t massage_length)
{
	int ret;
	struct i2c_msg msg;
	msg.buf = write_buf;
	msg.len = massage_length;
	msg.flags = I2C_MSG_WRITE | I2C_MSG_STOP;
	ret = i2c_transfer(dev_i2c0, &msg, massage_length, 0x29);
	if(ret == 0)
	{
		printk("\nWrite Successful");
	}
	else
	{
		printk("\nWrite Failed");
	}
}

/*******************************************************************
See header file for function definition
********************************************************************/

void read(uint8_t *read_buf, uint16_t slave_address, uint8_t massage_length)
{
	int ret;
	struct i2c_msg msg;
	msg.buf = read_buf;
	msg.len = massage_length;
	msg.flags = I2C_MSG_READ | I2C_MSG_STOP;
	ret = i2c_transfer(dev_i2c0, &msg, massage_length, slave_address);
	if(ret == 0)
	{
		printk("\nRead Successful");
	}
	else
	{
		printk("\nRead Failed");
	}
	
}

/*******************************************************************
See header file for function definition
********************************************************************/

void i2c_configure_set(uint8_t i2c_speed, uint8_t controller_mode_Master )
{
	int ret;
	i2c_cfg = I2C_SPEED_SET(i2c_speed) | controller_mode_Master;
	ret = i2c_configure(dev_i2c0, i2c_cfg);
	if(ret == 0)
	{
		printk("\nConfigure Success");
	}
	else
	{
		printk("\nConfigure Fail");
	}
}



/*******************************************************************
See header file for function definition
********************************************************************/

void enable_i2c(void)
{
	dev_i2c0 = device_get_binding(i2c0port);
	if (dev_i2c0 == NULL)
	{
		printk("Device Binding for I2C_0 failed\n");
	}
	else
	{
		printk("Device Binding for I2C_0 success\n");
	}
}
