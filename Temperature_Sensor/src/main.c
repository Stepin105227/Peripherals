
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











// #include <zephyr.h>
// #include <device.h>
// #include <devicetree.h>
// #include <stdio.h>
// #include <string.h>
// #include <drivers/i2c.h>
// #include <drivers/gpio.h>
// #include <drivers/sensor.h>
// #include <drivers/uart.h>

// // set the led sleep time
// #define SLEEP_TIME_MS  500
// #define TIME_PERIOD 2000
// #define DURATION 1   // in minutes

// // port
// const char *gpio0port = "GPIO_0";
// const char *uart0port = "UART_0";
// const char *i2c0port = "I2C_0";

// // device structure
// const struct device *m_dev_gpio0;
// const struct device *m_dev_uart0;
// const struct device *m_dev_nrf5;
// const struct device *m_dev_lm75;

// // set the pin no for gpio
// gpio_pin_t pwr3v3pin = 31;
// gpio_flags_t pwrflag = GPIO_OUTPUT | GPIO_ACTIVE_HIGH;

// // Config structure for uart
// struct uart_config cfg_uart1;

// // Sensor value structure
// struct sensor_value temperature;
// struct sensor_value nrf5_temperature;

// void enable_uart(void);    					// enable uart and gpio
// void enable_gpio(void);
// //void init_lm75(void);      					// initialize temp sensor with LM75 and nrf5
// void init_nrf5(void);
// //int32_t read_temperature(void);            // read temprature value from sensor and return temp value
// int32_t read_temperature_nrf5(void);
// static uint32_t start_time = 0;
// static uint32_t stop_time = 0;
// static uint16_t total_ittration =0;
// static bool temp_flag = 1;
// // static struct k_timer out_timer;
// // static void timer_callback(struct k_timer *timer)
// // {
// // 	read_temperature_nrf5();
// // 	//k_timer_stop(timer);
// // }

// // K_SEM_DEFINE(i2s_transfer, 0, 10);

// // void timer_ms_handler(struct k_timer *dummy)
// // {
// // 	k_sem_give(&i2s_transfer);
// // 	read_temperature_nrf5();
// // }
// // K_TIMER_DEFINE(my_timer, timer_ms_handler, NULL);
// void main(void)
// {
// 	k_sleep(K_SECONDS(1));
// 	printk("\n Temparature sensor reading using LM75\n");
// 	enable_gpio();
// 	gpio_pin_set(m_dev_gpio0, pwr3v3pin, 1);
// 	k_sleep(K_MSEC(100));
// 	enable_uart();
// 	//init_lm75();
// 	init_nrf5();
// 	total_ittration = (DURATION * 60000)/TIME_PERIOD;
// 	printk("\n Starting time in mili secound : %d ",k_uptime_get_32());
//     start_time = k_uptime_get_32();
// 	while(total_ittration > 0)
// 	{
// 		uint32_t current_time = k_uptime_get_32();
// 		read_temperature_nrf5();
// 		while(k_uptime_get_32() <= current_time + TIME_PERIOD);
// 		total_ittration--;
// 	}
// 	stop_time = k_uptime_get_32();
// 	printk("\n Total time has taken mili secound : %d ",stop_time - start_time);
// 	printk("\n Ending time in mili secound : %d ",k_uptime_get_32());
// 	printk("\n ==========================================");
// 	printk("\n========= Test Case PASS ===================");
// 	printk("\n ==========================================");
//    // read_temperature_nrf5();
// 	// k_timer_init(&out_timer, timer_callback,NULL);
// 	// k_timer_start(&out_timer,K_SECONDS(1),K_SECONDS(5));
// 	//while(1) {
// 		// k_sem_take(&i2s_transfer, K_NO_WAIT );
// 		// read_temperature_nrf5();
// 	//}
// 	// while(1)
// 	// {
// 	// 	//read_temperature();
//     //     read_temperature_nrf5();
// 	// 	k_sleep(K_MSEC(SLEEP_TIME_MS));
// 	// }
// }

// void enable_uart(void)
// {
//     int ret;
// 	cfg_uart1.baudrate = 115200;
// 	cfg_uart1.parity = UART_CFG_PARITY_NONE;
// 	cfg_uart1.stop_bits = UART_CFG_STOP_BITS_1;
// 	cfg_uart1.data_bits = UART_CFG_DATA_BITS_8;
// 	cfg_uart1.flow_ctrl = UART_CFG_FLOW_CTRL_NONE;

// 	m_dev_uart0 = device_get_binding(uart0port);
// 	if(m_dev_gpio0 == NULL)
// 	{
// 		printk("Device Binding for UART 0 failed\n");
// 	}
// 	else
// 	{
// 		printk("Device Binding for UART 0 success\n");
// 	}
// 	ret = uart_configure(m_dev_uart0, &cfg_uart1);
// 	if(ret ==0)
// 	{
// 		printk("Configuration of UART 0 success\n");
// 	}
// 	else
// 	{
// 		printk("Configuration of UART 0 failed\n");
// 	}

// }
// void enable_gpio(void)
// {
// 	int ret;
// 	m_dev_gpio0 = device_get_binding(gpio0port);
// 	if(m_dev_gpio0 == NULL)
// 	{
// 		printk("Device Binding for GPIO 0 failed\n");
// 	}
// 	else
// 	{
// 		printk("Device Binding for GPIO 0 success\n");
// 	}
// 	ret = gpio_pin_configure(m_dev_gpio0,pwr3v3pin,pwrflag);
// 	if(ret ==0)
// 	{
// 		printk("Configuration of GPIO pin success\n");
// 	}
// 	else
// 	{
// 		printk("Configuration of GPIO pin failed\n");
// 	}

// }
// // void init_lm75(void)
// // {
// // 	//m_dev_lm75 = device_get_binding("LM75");
// // 	m_dev_lm75 = device_get_binding(DT_LABEL(DT_INST(0, ti_lm75)));
// // 	if(m_dev_lm75 == NULL)
// // 	{
// // 		printk("Device Binding for LM_75 failed\n");
// // 	}
// // 	else
// // 	{
// // 		printk("Device Binding for LM_75 success\n");
// // 	}
// // }
// void init_nrf5(void)
// {
// 	m_dev_nrf5 = device_get_binding("TEMP_0");
// 	if(m_dev_nrf5 == NULL)
// 	{
// 		printk("Device Binding for nrf5 failed\n");
// 	}
// 	else
// 	{
// 		printk("Device Binding for nrf5 success\n");
// 	}
// }
// // int32_t read_temperature(void)
// // {
// // 	int sample_fatch;
// // 	int  reading_frm_sensor;
// // 	sample_fatch = sensor_sample_fetch(m_dev_lm75);
// // 	if(sample_fatch != 0)
// // 	{
// // 		printk(" Lm 75 sensor unable to fatch\n");
// // 	}
// // 	else
// // 	{
// // 		reading_frm_sensor = sensor_channel_get(m_dev_lm75,SENSOR_CHAN_AMBIENT_TEMP, &temperature);
// // 		if(reading_frm_sensor == 0)
// // 		{
// // 			printk("\nLM75_Temprature is : %d \t",temperature);
// // 		}
// // 		else
// // 		{
// // 			printk("\nLM75 failed to read Temprature");
// // 		}
// // 		k_sleep(K_MSEC(SLEEP_TIME_MS));

// // 	}
   
// // }
// int32_t read_temperature_nrf5(void)
// {
// 	int sample_fatch_nrf5;
// 	int  reading_frm_sensor_nrf;
// 	sample_fatch_nrf5 = sensor_sample_fetch(m_dev_nrf5);
// 	if(sample_fatch_nrf5 != 0)
// 	{
// 		printk(" Nrf5 sensor unable to fatch\n");
// 	}
// 	else
// 	{
// 		reading_frm_sensor_nrf = sensor_channel_get(m_dev_nrf5,SENSOR_CHAN_DIE_TEMP, &nrf5_temperature);
// 		if(reading_frm_sensor_nrf == 0)
// 		{
// 			printk("\nNrf5_Temprature is : %d.%d \t",nrf5_temperature.val1,nrf5_temperature.val2);
// 			if((26> nrf5_temperature.val1) && (nrf5_temperature.val1 >22))
// 			{
// 				temp_flag =0;
// 			}
// 			else
// 			{
// 				temp_flag = 1;
// 			}
// 		}
// 		else
// 		{
// 			printk("\nNrf5 failed to read Temprature");
// 		}
// 		k_sleep(K_MSEC(SLEEP_TIME_MS));

// 	}

// }




