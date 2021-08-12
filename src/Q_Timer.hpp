/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Q_Timer.h
 * Author: mdarweash
 *
 * Created on November 11, 2020, 7:32 PM
 */
#ifndef Q_TIMER_H
#define Q_TIMER_H

#define COUNTER_RTC

#include <zephyr.h>
#include <device.h>
#include <drivers/counter.h>
#include <sys/printk.h>
#include "Q_Globals.hpp"
#include "Q_Logger.h"
#include <power/power.h>
void init_RTC();
void test_counter_interrupt_fn(const struct device *counter_dev,
	uint8_t chan_id, uint32_t ticks,
	void *user_data);
#endif /* Q_TIMER_H */