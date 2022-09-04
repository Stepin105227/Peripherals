
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <drivers/spi.h>
#include "main.h"

#define BUFF_SIZE 8
// port
const char *gpio0port = "GPIO_0";
const char *spi1port = "SPI_1";

// device structure
const struct device *m_dev_gpio0;
const struct device *m_dev_spi1;

// set the pin no for gpio
gpio_pin_t pwr3v3pin = 31;
gpio_flags_t pwrflag = GPIO_OUTPUT | GPIO_ACTIVE_HIGH;

// structure required for SPI
struct spi_config cfg_spi;		  // SPI configuration structure
struct spi_cs_control spi_cs_ctl; // This can be used to control a CS line via a GPIO line

static struct spi_buf tx_buf;
static struct spi_buf rx_buf;
static struct spi_buf_set tx_buf_set;
static struct spi_buf_set rx_buf_set;

char Tx_data[BUFF_SIZE] = {'D', 'U', 'S', 'H', 'Y', 'A', 'N', 'T'};
char Rx_data[BUFF_SIZE];

void main(void)
{
	uint8_t total_itr = 0;
	k_sleep(K_SECONDS(1));
	printk("\n SPI Demo Start....\n");
	enable_gpio();
	gpio_pin_set(m_dev_gpio0, pwr3v3pin, 1);
	k_sleep(K_MSEC(100));
	enable_spi();
	while (total_itr < 5)
	{
		uint8_t rt = 0;
		rt = spi_write_read(m_dev_spi1, &cfg_spi, Tx_data, Rx_data);
		if (rt != 0)
		{
			printk("\n SPI Write Read Operation Failed");
		}
		total_itr++;
	}
}

void enable_spi(void)
{
	cfg_spi.frequency = 250000;
	cfg_spi.operation = SPI_OP_MODE_MASTER;
	cfg_spi.operation |= SPI_WORD_SET(8);
	cfg_spi.operation |= (SPI_MODE_CPOL | SPI_MODE_CPHA);
	cfg_spi.operation |= SPI_TRANSFER_MSB;
	cfg_spi.slave = 0;
	cfg_spi.cs = &spi_cs_ctl;

	spi_cs_ctl.gpio_dev = m_dev_gpio0;
	spi_cs_ctl.gpio_pin = 23;
	spi_cs_ctl.gpio_dt_flags = GPIO_ACTIVE_LOW;
	spi_cs_ctl.delay = 0; // Delay in microseconds to wait before starting the transmission and before releasing the CS line.

	m_dev_spi1 = device_get_binding(spi1port);
	if (m_dev_spi1 == NULL)
	{
		printk("Device Binding for SPI_1 failed\n");
	}
	else
	{
		printk("Device Binding for SPI_1 success\n");
	}
}
void enable_gpio(void)
{
	int ret;
	m_dev_gpio0 = device_get_binding(gpio0port);
	if (m_dev_gpio0 == NULL)
	{
		printk("Device Binding for GPIO 0 failed\n");
	}
	else
	{
		printk("Device Binding for GPIO 0 success\n");
	}
	ret = gpio_pin_configure(m_dev_gpio0, pwr3v3pin, pwrflag);
	if (ret == 0)
	{
		printk("Configuration of GPIO pin success\n");
	}
	else
	{
		printk("Configuration of GPIO pin failed\n");
	}
}

int spi_write_read(struct device *dev, struct spi_config *config, char *Tx_SPI_data, char *Rx_SPI_data)
{
	uint8_t ret;
	tx_buf.buf = Tx_SPI_data;
	tx_buf.len = sizeof(Tx_data);
	tx_buf_set.buffers = &tx_buf;
	tx_buf_set.count = 1;
	rx_buf.buf = Rx_SPI_data;
	rx_buf.len = sizeof(Rx_data);
	rx_buf_set.buffers = &rx_buf;
	rx_buf_set.count = 1;
	ret = spi_transceive(dev, config, &tx_buf_set, &rx_buf_set);
	if (ret != 0)
	{
		printk("\n SPI Transmission Failed");
	}
	printk("\n Transmit : ");
	for (int i = 0; i < 8; i++)
	{
		printk("%c", Tx_data[i]);
	}
	printk("\n Receiver : ");
	for (int i = 0; i < 8; i++)
	{
		printk("%c", Rx_data[i]);
	}

	return ret;
}