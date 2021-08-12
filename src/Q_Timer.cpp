/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Q_Timer.c
 * Author: mdarweash
 * 
 * Created on November 11, 2020, 7:32 PM
 */

#include "Q_Timer.hpp"

#ifdef COUNTER_RTC
#define TIMER DT_LABEL(DT_NODELABEL(timer1))
#define DELAY 10000000
#define ALARM_CHANNEL_ID 0
const struct device *counter_dev;
struct counter_alarm_cfg alarm_cfg;
int timer_ret;
#endif

void test_counter_interrupt_fn(const struct device *counter_dev,
	uint8_t chan_id, uint32_t ticks,
	void *user_data)
{
#ifdef COUNTER_RTC
	printq("alarm!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	k_sleep(K_MSEC(100));
	printq("I'm Ok\n");
	timer_ret = counter_set_channel_alarm(counter_dev, ALARM_CHANNEL_ID,
		&alarm_cfg);
	//	printq("USEC_PER_SEC=%d\n",USEC_PER_SEC);
	printq("Set alarm in %u sec (%u ticks)\n",
		(uint32_t) (counter_ticks_to_us(counter_dev,
		alarm_cfg.ticks) / USEC_PER_SEC),
		alarm_cfg.ticks);

	if (-EINVAL == timer_ret) {
		printq("Alarm settings invalid\n");
	} else if (-ENOTSUP == timer_ret) {
		printq("Alarm setting request not supported\n");
	} else if (timer_ret != 0) {
		printq("Error\n");
	}
	printq("new Alarm set!\n");
#endif
}

void init_times()
{
#ifdef COUNTER_RTC
	printq("Counter alarm sample\n\n");
	counter_dev = device_get_binding(TIMER);
	if (counter_dev == NULL) {
		printq("Device not found\n");
		return;
	}

	timer_ret = counter_start(counter_dev);
	if (timer_ret != 0) {
		printq("timer init problem%d\n", timer_ret);
	}
	alarm_cfg.flags = 0;
	alarm_cfg.ticks = counter_us_to_ticks(counter_dev, DELAY);
	alarm_cfg.callback = test_counter_interrupt_fn;
	alarm_cfg.user_data = &alarm_cfg;

	timer_ret = counter_set_channel_alarm(counter_dev, ALARM_CHANNEL_ID,
		&alarm_cfg);
	printq("Set alarm in %u sec (%u ticks)\n",
		(uint32_t) (counter_ticks_to_us(counter_dev,
		alarm_cfg.ticks) / USEC_PER_SEC),
		alarm_cfg.ticks);

	if (-EINVAL == timer_ret) {
		printq("Alarm settings invalid\n");
	} else if (-ENOTSUP == timer_ret) {
		printq("Alarm setting request not supported\n");
	} else if (timer_ret != 0) {
		printq("Error\n");
	}
#endif
}
