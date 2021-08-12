/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#include "Q_adc.hpp"
#ifdef Q_ADC
#define ADC_DEVICE_NAME  DT_LABEL(DT_INST(0, nordic_nrf_saadc))
#define ADC_RESOLUTION  10
#define ADC_GAIN  ADC_GAIN_1_6
#define ADC_REFERENCE  ADC_REF_INTERNAL
#define ADC_ACQUISITION_TIME ADC_ACQ_TIME(ADC_ACQ_TIME_MICROSECONDS, 10)


#define LOG_LEVEL CONFIG_ADC_LOG_LEVEL
#define BUFFER_SIZE  6
int adc_ret;
uint32_t m_sample_buffer[BUFFER_SIZE];
//
const struct device *adc_dev;

void init_adc()
{
#ifdef Q_ADC

	printq("init ADC:%s\n", ADC_DEVICE_NAME);
	adc_dev = device_get_binding(ADC_DEVICE_NAME);
	if (adc_dev == NULL) {
		printq("Can't get ADC Device");
		return;
	}
	printq("init ADC OK!\n");

#endif
}
//ADC 1
const struct adc_channel_cfg adc_config_1 = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = 3,
	.differential = 0,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive = NRF_SAADC_INPUT_AIN1,
#endif
};
const struct adc_sequence adc_sequence_1 = {
	.channels = BIT(3),
	.buffer = m_sample_buffer,
	.buffer_size = sizeof(m_sample_buffer),
	.resolution = ADC_RESOLUTION,
	//	.calibrate = true,
};
//

//ADC 2
const struct adc_channel_cfg adc_config_2 = {
	.gain = ADC_GAIN,
	.reference = ADC_REFERENCE,
	.acquisition_time = ADC_ACQUISITION_TIME,
	.channel_id = 2,
	.differential = 0,
#if defined(CONFIG_ADC_CONFIGURABLE_INPUTS)
	.input_positive = NRF_SAADC_INPUT_AIN2,
#endif
};
const struct adc_sequence adc_sequence_2 = {
	.channels = BIT(2),
	.buffer = m_sample_buffer,
	.buffer_size = sizeof(m_sample_buffer),
	.resolution = ADC_RESOLUTION,
	//	.calibrate = true,
};

void init_adc_pin_1()
{
	adc_ret = adc_channel_setup(adc_dev, &adc_config_1);
	if (adc_ret != 0) {

		printq("init adc (1) Error:%d!\n", adc_ret);
		return;
	}
	printq("init adc(1) OK!\n");
	for (int i = 0; i < BUFFER_SIZE; i++) {
		m_sample_buffer[i] = 0;
	}
}

int read_adc_1()
{
	int adc_ret;
	adc_ret = adc_read(adc_dev, &adc_sequence_1);
	if (adc_ret != 0)
		printq("adc_read() failed with code %d\n", adc_ret);
	if (m_sample_buffer[0] > 1024 || m_sample_buffer[0] < 10)
		m_sample_buffer[0] = 0;

	return m_sample_buffer[0];
}


//

void init_adc_pin_2()
{
	adc_ret = adc_channel_setup(adc_dev, &adc_config_2);
	if (adc_ret != 0) {

		printq("init adc (2) Error:%d!\n", adc_ret);
		return;
	}
	printq("init adc(2) OK!\n");
	for (int i = 0; i < BUFFER_SIZE; i++) {
		m_sample_buffer[i] = 0;
	}
}

int read_adc_2()
{
	int adc_ret;
	adc_ret = adc_read(adc_dev, &adc_sequence_2);
	if (adc_ret != 0)
		printq("adc_read() failed with code %d\n", adc_ret);

	//	for (int i = 0; i < BUFFER_SIZE; i++) {
	//		printq("%d | ", m_sample_buffer[i] + 0xffff);
	if (m_sample_buffer[0] > 2024 || m_sample_buffer[0] < 20)
		m_sample_buffer[0] = 0;
	//	}

	return m_sample_buffer[0];
}
#endif