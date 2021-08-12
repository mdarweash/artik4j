#ifndef Q_BLUETOOTH_H
#define Q_BLUETOOTH_H
#endif
#ifdef Q_BLUETOOTH_H
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/addr.h>
#include <bluetooth/gap.h>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <sys/printk.h>
//#include "Q_Globals.hpp"
#include "Q_Logger.h"
//#define BLE4_ADV
#define BLE5_ADV
//#define BLE4_SCANNER
#define BLE5_SCANNER
//#define LOG_BT_HEX
//#define BT_SCANNER_LOGGER
static void adv_recv(const struct bt_le_scan_recv_info *info, struct net_buf_simple *buf);

void print_macaddress(const bt_addr_le_t* bt_add);

void macaddress_to_string(const bt_addr_le_t * bt_add);

bool isMacEqual(const uint8_t mac1[], const uint8_t mac2[]);

void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
	struct net_buf_simple * buf);

void ble4_scanner();

void start_ble5_scanner();
void stop_ble5_scanner();
void ble4_adv();

void start_ble5_adv();
void stop_ble5_adv();
void init_bluetooth();
#endif