#ifndef Q_ADC
//#define Q_ADC
#endif

#ifdef Q_ADC

#include <hal/nrf_saadc.h>
#include <drivers/adc.h>
#include <zephyr.h>
#include <sys/printk.h>
#include "Q_Globals.hpp"
#include "Q_Logger.h"
void init_adc();

void init_adc_pin_1();
int read_adc_1();
void init_adc_pin_2();
int read_adc_2();
#endif