/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Q_GPIO.cpp
 * Author: mdarweash
 * 
 * Created on November 3, 2020, 3:09 PM
 */

#include "Q_GPIO.hpp"

#include "kernel.h"

extern const k_tid_t thread_led_blinker_id;
extern Q_GPIO q_led_0(PIN0, GPIO_OUTPUT_ACTIVE | FLAGS0);
extern Q_GPIO q_led_1(PIN1, GPIO_OUTPUT_LOW);

Q_GPIO::Q_GPIO(uint8_t pin, gpio_flags_t flags)
{
	this->flags = flags;
	this->pin = pin;

}

void Q_GPIO::init(const char * devName)
{
	//		this->dev = dev;
	this->dev = device_get_binding(devName);
	ret = gpio_pin_configure(dev, pin, flags);
	if (ret < 0) {
		printq("failed init PIN %s", devName);
		return;
	}
	printq("init PIN %s OK", devName);
}

void Q_GPIO::setPinActive()
{
	gpio_pin_set(dev, pin, true);
}

void Q_GPIO::setPinInActive()
{
	gpio_pin_set(dev, pin, false);
}

void Q_GPIO::setPin(uint8_t value)
{
	//		this->value = value;
	gpio_pin_set_raw(dev, pin, value);
}

uint8_t Q_GPIO::getPin()
{
	//		value = gpio_pin_get(dev, pin);
	return gpio_pin_get_raw(dev, pin);
}

void Q_GPIO::invert()
{
	gpio_pin_set(dev, pin, value);
	value = !value;
}

void Q_GPIO::set_blink(int blinks, int delay)
{
	this->blinks = blinks;
	this->delay = delay;
//	printq("waking up the thread\n");
	k_wakeup(thread_led_blinker_id);
//	printq("waking up the thread Done\n");
}

void Q_GPIO::blink()
{
	for (int i = 0; i < blinks; i++) {
		setPinActive();
		k_sleep((K_MSEC(delay)));
		setPinInActive();
		k_sleep((K_MSEC(delay)));
	}
	blinks = 0;
	delay = 0;
}

void thread_led_blinker()
{

	printq("thread_led_blinker started\n");

	//	printq("thread_led_blinker:%s\n",leds_thread);
	while (1) {
#ifdef Q_GPIO_INIT
		//		//		pin_1_10.invert();
		//		//		printq("INPUT now :%d\n" + pin_0_10.getPin());
		//		//        printq("inverting a led\n");
		//		q_led_snd_is_on = !q_led_snd_is_on;
		//		q_led_0.invert();
//		printq("ledBlinked| Started\n");
		q_led_0.blink();
		q_led_1.blink();
//		printq("ledBlinked| Sleeping...\n");
		k_sleep(K_FOREVER);
#endif
	}
}
K_THREAD_DEFINE(thread_led_blinker_id, 1024, thread_led_blinker, NULL, NULL, NULL,
	7, 0, 0);