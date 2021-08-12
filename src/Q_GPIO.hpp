/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Q_GPIO.hpp
 * Author: mdarweash
 *
 * Created on November 3, 2020, 3:09 PM
 */

#ifndef Q_GPIO_HPP
#define Q_GPIO_HPP
#define Q_GPIO_INIT

#include <zephyr.h>
#include <device.h>
#include <devicetree.h>
#include <drivers/gpio.h>
#include <sys/printk.h>
#include "Q_Logger.h"
#include "Q_Globals.hpp"
#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0 DT_GPIO_LABEL(LED0_NODE, gpios)
#define PIN0 DT_GPIO_PIN(LED0_NODE, gpios)
#define FLAGS0 DT_GPIO_FLAGS(LED0_NODE, gpios)
#else

/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED0 ""
#define PIN0 0
#define FLAGS0 0
#endif
#if DT_NODE_HAS_STATUS(LED1_NODE, okay)
#define LED1 DT_GPIO_LABEL(LED1_NODE, gpios)
#define PIN1 DT_GPIO_PIN(LED1_NODE, gpios)
#define FLAGS1 DT_GPIO_FLAGS(LED1_NODE, gpios)
#else
/* A build error here means your board isn't set up to blink an LED. */
#error "Unsupported board: led0 devicetree alias is not defined"
#define LED1 ""
#define PIN1 0
#define FLAGS1 0
#endif

class Q_GPIO {
	int ret;
	static const struct device *gpioDev0;
	static const struct device *gpioDev1;
	int blinks = 0;
	int delay = 0;
public:
	uint8_t pin;
	const struct device *dev;
	uint8_t value = false;
	gpio_flags_t flags;

	Q_GPIO(uint8_t pin, gpio_flags_t flags);

	void init(const char * devName);

	void setPinActive();

	void setPinInActive();

	void setPin(uint8_t value);

	uint8_t getPin();

	void invert();

	void set_blink(int blinks, int delay);

	void blink();
};
extern Q_GPIO q_led_0;
extern Q_GPIO q_led_1;
#endif /* Q_GPIO_HPP */

