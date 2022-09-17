
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <nrfx_twis.h>
#include "main.h"

#define SLAVE_ADDR (0x28)
#define TWI_PIN_SDA 26
#define TWI_PIN_SCL 27
#define TWI_INT_PRIORITY 2
#define TWIS_INST 1
static uint16_t buffer_index;
static unsigned char twis_rx_buffer[14];
static unsigned char twis_tx_buffer[] = "This is a circular slave transmit buffer.";

const nrfx_twis_t m_twis = NRFX_TWIS_INSTANCE(TWIS_INST);
nrfx_twis_config_t i2c_config_s;
void initializing_TWIS(void);

static void twis_event_handler(nrfx_twis_evt_t const *const p_event)
{
	switch(p_event->type)
	{
		case NRFX_TWIS_EVT_READ_REQ:
		    if(p_event->data.buf_req)
			{
				(void) nrfx_twis_tx_prepare(&m_twis,twis_tx_buffer + buffer_index, 14);

              if((buffer_index += 14) >= sizeof(twis_tx_buffer)){
              buffer_index = 0;
				// nrfx_twis_tx_prepare(&m_twis,twis_tx_buffer,14);
			}
			}
			
			break;
		case NRFX_TWIS_EVT_READ_DONE:
			printk("\n nrf Read event done\n");
			break;
		case NRFX_TWIS_EVT_WRITE_REQ:
		    if(p_event->data.buf_req)
			{
				nrfx_twis_rx_prepare(&m_twis,twis_rx_buffer,14);
			}
			break;
		case NRFX_TWIS_EVT_WRITE_DONE:
		    printk("\nWrite Done\n");
			break;
		case NRFX_TWIS_EVT_READ_ERROR:
		    printk("\nTWIS READ ERROR\n");
			break;
		case NRFX_TWIS_EVT_WRITE_ERROR:
		    printk("\nTWIS WRITE ERROR\n");
			break;
		case NRFX_TWIS_EVT_GENERAL_ERROR:
		    printk("\nTWIS GENERAL ERROR\n");
			break;
		default:
		    printk("TWIS default\n");
			break;

	}
}


void initializing_TWIS()
{
	IRQ_CONNECT(DT_IRQN(DT_NODELABEL(i2c1)),
       DT_IRQ(DT_NODELABEL(i2c1), priority),
       nrfx_isr, nrfx_twis_1_irq_handler, 0);

	i2c_config_s.addr[0] = SLAVE_ADDR;
	i2c_config_s.addr[1] = 0;
	i2c_config_s.sda = TWI_PIN_SDA;
	i2c_config_s.scl = TWI_PIN_SCL;
	i2c_config_s.sda_pull = NRF_GPIO_PIN_PULLUP;
	i2c_config_s.scl_pull = NRF_GPIO_PIN_PULLUP;
	i2c_config_s.interrupt_priority = TWI_INT_PRIORITY;

	
	if(nrfx_twis_init(&m_twis, &i2c_config_s,twis_event_handler ) == NRFX_SUCCESS )
	{
		printk("\nnrf Twis initialized\n");
	}
	else
	{
		printk("\nnrf Twis not initialized\n");
	}
}


void main(void)
{
	printk("\n I2C Slave Demo Start....\n");
	initializing_TWIS();
	nrfx_twis_enable(&m_twis);
	
	
}
