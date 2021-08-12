/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Q_Logger.cpp
 * Author: mdarweash
 * 
 * Created on November 4, 2020, 12:28 PM
 */

#define Q_LOGGER_ON
#include "Q_Logger.h"

void printq(const char *fmt, ...)
{
#ifdef Q_LOGGER_ON
	va_list args;
	va_start(args, NULL);
	vprintk(fmt, args);
	va_end(args);
#endif
}