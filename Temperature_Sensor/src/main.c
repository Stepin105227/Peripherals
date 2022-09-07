
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <stdio.h>
#include <string.h>
#include <drivers/i2c.h>
#include <drivers/gpio.h>
#include <drivers/sensor.h>
#include <drivers/uart.h>

// set the led sleep time
#define SLEEP_TIME_MS  500
#define TIME_PERIOD 2000
#define DURATION 1   // in minutes

// port
const char *gpio0port = "GPIO_0";
const char *uart0port = "UART_0";
const char *i2c0port = "I2C_0";

// device structure
const struct device *m_dev_gpio0;
const struct device *m_dev_uart0;
const struct device *m_dev_nrf5;
const struct device *m_dev_lm75;

// set the pin no for gpio
gpio_pin_t pwr3v3pin = 31;
gpio_flags_t pwrflag = GPIO_OUTPUT | GPIO_ACTIVE_HIGH;

// Config structure for uart
struct uart_config cfg_uart1;

// Sensor value structure
struct sensor_value temperature;
struct sensor_value nrf5_temperature;

void enable_uart(void);    					// enable uart and gpio
void enable_gpio(void);
//void init_lm75(void);      					// initialize temp sensor with LM75 and nrf5
void init_nrf5(void);
//int32_t read_temperature(void);            // read temprature value from sensor and return temp value
int32_t read_temperature_nrf5(void);

static struct k_timer timer;
static uint8_t count =0;
static void timer_expire(struct k_timer *timer)
{
	count++;
	printk("\n Time Expire..");
	if(count == 1)
	{
		k_timer_stop(timer);
	}
}

static void timer_stop(struct k_timer *timer)
{
	printk("\n Timer stop..");
}
void main(void)
{
	k_sleep(K_SECONDS(1));
	printk("\n Temparature sensor reading using LM75....\n");
	enable_gpio();
	gpio_pin_set(m_dev_gpio0, pwr3v3pin, 1);
	k_sleep(K_MSEC(100));
	enable_uart();
	//init_lm75();
	init_nrf5();
	k_timer_init(&timer, timer_expire,timer_stop);
	k_timer_start(&timer,K_SECONDS(60),K_SECONDS(5));
	while(count == 0)
	{
		read_temperature_nrf5();
	}
	
}

void enable_uart(void)
{
    int ret;
	cfg_uart1.baudrate = 115200;
	cfg_uart1.parity = UART_CFG_PARITY_NONE;
	cfg_uart1.stop_bits = UART_CFG_STOP_BITS_1;
	cfg_uart1.data_bits = UART_CFG_DATA_BITS_8;
	cfg_uart1.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;

	m_dev_uart0 = device_get_binding(uart0port);
	if(m_dev_gpio0 == NULL)
	{
		printk("Device Binding for UART 0 failed\n");
	}
	else
	{
		printk("Device Binding for UART 0 success\n");
	}
	ret = uart_configure(m_dev_uart0, &cfg_uart1);
	if(ret ==0)
	{
		printk("Configuration of UART 0 success\n");
	}
	else
	{
		printk("Configuration of UART 0 failed\n");
	}

}
void enable_gpio(void)
{
	int ret;
	m_dev_gpio0 = device_get_binding(gpio0port);
	if(m_dev_gpio0 == NULL)
	{
		printk("Device Binding for GPIO 0 failed\n");
	}
	else
	{
		printk("Device Binding for GPIO 0 success\n");
	}
	ret = gpio_pin_configure(m_dev_gpio0,pwr3v3pin,pwrflag);
	if(ret ==0)
	{
		printk("Configuration of GPIO pin success\n");
	}
	else
	{
		printk("Configuration of GPIO pin failed\n");
	}

}

void init_nrf5(void)
{
	m_dev_nrf5 = device_get_binding("TEMP_0");
	if(m_dev_nrf5 == NULL)
	{
		printk("Device Binding for nrf5 failed\n");
	}
	else
	{
		printk("Device Binding for nrf5 success\n");
	}
}

int32_t read_temperature_nrf5(void)
{
	int sample_fatch_nrf5;
	int  reading_frm_sensor_nrf;
	sample_fatch_nrf5 = sensor_sample_fetch(m_dev_nrf5);
	if(sample_fatch_nrf5 != 0)
	{
		printk(" Nrf5 sensor unable to fatch\n");
	}
	else
	{
		reading_frm_sensor_nrf = sensor_channel_get(m_dev_nrf5,SENSOR_CHAN_DIE_TEMP, &nrf5_temperature);
		if(reading_frm_sensor_nrf == 0)
		{
			printk("\nNrf5_Temprature is : %d.%d \t",nrf5_temperature.val1,nrf5_temperature.val2);
			if((26> nrf5_temperature.val1) && (nrf5_temperature.val1 >22))
			{
				//temp_flag =0;
			}
			else
			{
				//temp_flag = 1;
			}
		}
		else
		{
			printk("\nNrf5 failed to read Temprature");
		}
		k_sleep(K_MSEC(SLEEP_TIME_MS));

	}

}

