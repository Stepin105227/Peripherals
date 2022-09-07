
#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/pwm.h>
#include "main.h"

#define PWM_CH0_PIN DT_PROP(DT_NODELABEL(pwm0), ch0_pin)
// port
const char *pwm0port = "PWM_0";

// device structure
const struct device *m_dev_pwm0;

// static valiable
static uint16_t duty_cycle_percent=75;
static uint8_t flag =0;
static uint32_t desired_frequency = 250000;
static uint32_t period_cycle =0;
static uint32_t pulse_cycle =0;

void main(void)
{
	
	k_sleep(K_SECONDS(1));
	printk("\n PWM Demo Start....\n");
	enable_pwm();
	if(duty_cycle_percent!=0)
	{
		duty_cycle_percent = (duty_cycle_percent * 65535)/100;
		// printk("\n Duty cycle is %ld",duty_cycle_percent);
	}

	configuration_set(NORMAL_POLARITY,desired_frequency, duty_cycle_percent);
	// printk("\ndesired_frequency : %d duty_cycle_percent : %d ",desired_frequency,duty_cycle_percent);
	generate_pwm();
	
}
/*******************************************************************
See header file for function definition
********************************************************************/
void generate_pwm()
{
	uint8_t ret;
	ret = pwm_pin_set_cycles(m_dev_pwm0,PWM_CH0_PIN,period_cycle,pulse_cycle,flag);
	if(ret ==0)
	{
		// printk("\n pwm_pin_set_cycles is working");
	}
	else
	{
		printk("\n pwm_pin_set_cycles is not working ");
	}
}


/*******************************************************************
See header file for function definition
********************************************************************/

void configuration_set(uint8_t polarity, uint32_t frequency, uint16_t duty_cycle)
{
	uint8_t ret;
	uint64_t cycle_per_second;
	flag = polarity;
	ret = pwm_get_cycles_per_sec(m_dev_pwm0, PWM_CH0_PIN, &cycle_per_second);
	if(ret ==0)
	{
		// printk("\n cycle_per_second %lld",cycle_per_second);
		// printk("\n desired_frequency %ld",desired_frequency);
		period_cycle = cycle_per_second/desired_frequency;
		// printk("\n Period_cycle %ld",period_cycle);
		// printk("\n pwm_get_cycles_per_sec is working properly ");
	}
	else
	{
		printk("\n pwm_get_cycles_per_sec is not working properly ");
	}

	pulse_cycle = (duty_cycle * period_cycle)>>16;
    // printk("\n Pulse_cycle is %ld",pulse_cycle);
}


/*******************************************************************
See header file for function definition
********************************************************************/

void enable_pwm(void)
{
	m_dev_pwm0 = device_get_binding(pwm0port);
	if (m_dev_pwm0 == NULL)
	{
		printk("Device Binding for PWM_0 failed\n");
	}
	else
	{
		printk("Device Binding for PWM_0 success\n");
	}
}
