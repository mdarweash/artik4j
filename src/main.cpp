/* main.c - Application main entry point */

/*
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */
//https://jimmywongiot.com/2019/12/18/step-by-step-on-nordic-connect-sdk-for-development/
#include <zephyr/types.h>
#include <stddef.h>

#include <sys/util.h>
#include <stdio.h>
#include <stdlib.h>

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <usb/usb_device.h>
#include <drivers/uart.h>
#include <logging/log.h>
#include <math.h>

#include "kernel.h"
#include "Q_GPIO.hpp"
#include "Q_Bluetooth.hpp"
#include "Q_Logger.h"
#include "Q_Timer.hpp"
#include "Q_ULP.hpp"
#include "Q_adc.hpp"
#include "Q_Globals.hpp"
// ------------CONFIGURATION---------------
//#define USB_UART

#ifdef CONFIG_USB
#endif
#define LED_INIT

//#define Q_I2C

#if  defined BLE5_ADV || defined BLE4_ADV || defined BLE5_SCANNER || defined BLE4_SCANNER
#endif


#ifdef Q_ADC
#include "Q_adc.hpp"
int adc_1 = -1;
int adc_2 = -1;
#endif





#ifdef Q_I2C

//#if DT_NODE_HAS_STATUS(DT_ALIAS(i2c_0), okay)
//#define I2C_DEV_NAME DT_LABEL(DT_ALIAS(i2c_0))
//#elif DT_NODE_HAS_STATUS(DT_ALIAS(i2c_1), okay)
//#define I2C_DEV_NAME DT_LABEL(DT_ALIAS(i2c_1))
//#elif DT_NODE_HAS_STATUS(DT_ALIAS(i2c_2), okay)
//#define I2C_DEV_NAME DT_LABEL(DT_ALIAS(i2c_2))
//#else
//#error "Please set the correct I2C device"
//#endif
const struct device *i2c_dev;

#include <drivers/i2c.h>

#endif
#ifdef USB_UART
const struct device *devUART;
uint32_t dtr = 0U;
#endif

int ret;
int i = 0;

const struct device *dev0;
const struct device *dev1;

bool q_led_rcv_is_on = false;
bool q_led_snd_is_on = true;



#ifdef Q_GPIO_INIT

void init_gpio()
{
#ifdef Q_GPIO_INIT
	printq("Init Pins\n");
	q_led_1.init(LED1);
	printq("Led 1 Ok\n");
	q_led_0.init(LED0);
	printq("Led 0 Ok\n");
#endif
}

#endif

void init_led()
{
#ifdef LED_INIT

	dev0 = device_get_binding(LED0);
	dev1 = device_get_binding(LED1);
	if (dev0 == NULL) {
		printq("failed init LED0");
		return;
	}
	if (dev1 == NULL) {
		printq("failed init LED1");
		return;
	}

	ret = gpio_pin_configure(dev0, PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
	if (ret < 0) {
		printq("failed init PIN0");
		return;
	}

	ret = gpio_pin_configure(dev1, PIN1, GPIO_OUTPUT_ACTIVE | FLAGS1);
	if (ret < 0) {
		printq("failed init PIN1");

		return;
	}
	gpio_pin_set(dev1, PIN1, (int) q_led_rcv_is_on);
	gpio_pin_set(dev0, PIN0, (int) q_led_snd_is_on);
#endif 
}

void init_i2c()
{
#ifdef Q_I2C
	i2c_dev = device_get_binding(I2C_DEV);
	if (!i2c_dev) {
		printq("I2C: Device driver not found.\n");
		return;
	}
#endif
}

#ifdef Q_I2C

#endif

void init_usb_uart()
{
#ifdef USB_UART
	devUART = device_get_binding("CDC_ACM_0");
	if (!devUART) {
		printq("CDC ACM device not found");
		return;
	}
	printq("CDC ACM Ready.");
	//
	ret = usb_enable(NULL);
	if (ret != 0) {
		printq("Failed to enable USB");

		return;
	}
	printq("Usb Enabled.");
#endif
}

void init_system()
{
	//--------------------- USB_UART ---------------------

	init_usb_uart();

	//--------------------- GPIO  ---------------------
#ifdef Q_GPIO_INIT
	init_gpio();
#endif

	//    --------------------- LEDS | GPIO---------------------
	init_led();

	//--------------------- Bluetooth Subsystem ---------------------
	init_bluetooth();

	//--------------------- Bluetooth 4 Scanner---------------------
	ble4_scanner();

	//--------------------- Bluetooth 5 Scanner---------------------


	//--------------------- Bluetooth 4 Advertising---------------------
	ble4_adv();

	//--------------------- Bluetooth 5 Advertising---------------------

	//---------------------           I2C          ---------------------
	init_i2c();


#ifdef Q_ADC
	//---------------------           ADC          ---------------------
	init_adc();
	init_adc_pin_1();
	init_adc_pin_2();
#endif
}

void readAdc()
{
#ifdef Q_ADC
	printq("reading ADC1\n");

	adc_1 = read_adc_1();
	printq("ADC1 ==%d\n", adc_1);
	adc_2 = read_adc_2();
	printq("ADC2 =%d\n", adc_2);
#endif
}

void main_thread()
{
	while (!ready) {
		k_sleep(K_SECONDS(1));
	}
	printq("mainThread started..\n");

	do {
		//	    printq("sleeping for %d\n",k_uptime_get_32());
		k_sleep(K_MSEC(500));
#ifdef Q_ADC
		readAdc();
#endif

#if  defined BLE5_ADV || defined BLE4_ADV
#endif
	} while (1);
}

void thread_ble()
{
	while (!ready) {
		k_sleep(K_SECONDS(1));
	}
	printq("thread_blue started..\n");
	start_ble5_scanner();
	while (1) {

	}
}

void main(void)
{

	init_system();
	ready = true;

	//Enter main thread in sleep
	k_cpu_idle();

}
//
//K_THREAD_DEFINE(main_thread_id, 1024, main_thread, NULL, NULL, NULL,
//	7, 0, 0);

K_THREAD_DEFINE(thread_ble_id, 1024, thread_ble, NULL, NULL, NULL,
	7, 0, 0);