/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#include "Q_Bluetooth.hpp"
#include "Q_GPIO.hpp"

static uint8_t mfg_data[] = {0xff, 0xff, 0x73};
static uint8_t temp_data[] = {0x09, 0x18, 0x33};
//char macAddress[] = "xx:xx:xx:xx:xx:xx";
uint8_t sidCounter = 0;
char* macAddress;
uint8_t ignoreMac[] = {0xCD, 0xE4, 0xEF, 0x2D, 0x7C, 0x00};
uint8_t QoudraMac1[] = {0x4d, 0xb9, 0x1f, 0x5e, 0x6C, 0xCA};
uint8_t QoudraMac2[] = {0xf5, 0x20, 0x81, 0x1e, 0x05, 0xF9};
uint8_t QoudraMac3[] = {0x15, 0x44, 0x6b, 0x02, 0xd7, 0xF1};
uint8_t QoudraMac4[] = {0x86, 0xE7, 0xAB, 0x61, 0xA9, 0xD4};
uint8_t QoudraMac5[] = {0xb2, 0x6B, 0x6C, 0x62, 0xF5, 0xDF};
uint8_t SkymindPocMac[] = {0x49, 0xf0, 0x9f, 0xb7, 0xac, 0xeb};
const struct bt_data ad[] = {
  BT_DATA_BYTES(BT_DATA_FLAGS, (BT_LE_AD_GENERAL | BT_LE_AD_NO_BREDR)),
  BT_DATA_BYTES(BT_DATA_UUID16_ALL, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06),
};

int bt_ret;



#ifdef BLE4_ADV
static const struct bt_data ad[] = {
  BT_DATA(BT_DATA_MANUFACTURER_DATA, mfg_data, 3),
  BT_DATA(BT_DATA_NAME_COMPLETE, "Qoudra@Skymind.My", 17),
  BT_DATA(BT_DATA_SVC_DATA16, temp_data, 3)
};
#endif
#ifdef BLE5_ADV

struct bt_le_ext_adv *adv;
#endif

#ifdef BLE5_SCANNER 

const struct bt_le_scan_param scan_param = {
  .type = BT_HCI_LE_SCAN_PASSIVE,
  .options = BT_LE_SCAN_OPT_CODED,
  .interval = 0x0010,
  .window = 0x0010,
};
#endif

#ifdef BLE4_SCANNER
struct bt_le_scan_param scan_param = {
  .type = BT_HCI_LE_SCAN_PASSIVE,
  .options = BT_LE_SCAN_OPT_NONE,
  .interval = 0x0010,
  .window = 0x0010,
};
#endif

#if defined BLE4_ADV || defined BLE5_ADV
struct bt_le_adv_param *adv_param =
  BT_LE_ADV_PARAM(BT_LE_ADV_OPT_USE_TX_POWER | BT_LE_ADV_OPT_USE_IDENTITY,
  BT_GAP_ADV_FAST_INT_MIN_2,
  BT_GAP_ADV_FAST_INT_MAX_2,
  NULL);

#endif

void print_macaddress(const bt_addr_le_t* bt_add)
{
  for (int i = 0; i < 6; i++) {
    printq("%X", bt_add->a.val[5 - i]);
    //    macAddress[i * 2 + i ] = temp[0];
    //    if (bt_add->a.val[i] == 0) {
    //      printq("0");
    //    }
    if (i != 5) {
      printq(":");
    }
  }
}

void macaddress_to_string(const bt_addr_le_t * bt_add)
{
  char temp[2];
  for (int i = 0; i < 6; i++) {
    sprintf(temp, "%X", bt_add->a.val[5 - i]);
    macAddress[i * 2 + i ] = temp[0];
    if (bt_add->a.val[i] == 0) {
      macAddress[i * 2 + i + 1] = temp[0];
    } else {
      macAddress[i * 2 + i + 1] = temp[1];
    }
  }
  //  bt_addr_le_to_str(bt_add,macAddress,10);
}

bool isMacEqual(const uint8_t mac1[], const uint8_t mac2[])
{
  if (sizeof(mac2) == 0)return false;
  for (int i = 0; i < 6; i++) {
    if (mac1[i] != mac2[i])
      return false;
  }
  return true;
}

void scan_cb(const bt_addr_le_t *addr, int8_t rssi, uint8_t adv_type,
  struct net_buf_simple * buf)
{
  if (isMacEqual(QoudraMac1, addr->a.val) || isMacEqual(QoudraMac2, addr->a.val) || isMacEqual(QoudraMac3, addr->a.val) || isMacEqual(QoudraMac4, addr->a.val) || isMacEqual(SkymindPocMac, addr->a.val) || isMacEqual(QoudraMac5, addr->a.val)) {
#ifdef Q_GPIO_INIT
    q_led_1.set_blink(1, 50);
#endif
    printq("{");

    printq("\"macAdress\":");
    printq("\"");
    print_macaddress(addr);
    printq("\",");

    printq("\"rssi\":%d,", rssi);
    printq("\"advType\":%d,", adv_type);
    printq("\"data\":\"");
    for (int i = 0; i < buf->len; i++) {
      printq("0x%X ", buf->data[i]);
    }
    printq("\"");
    printq("};\n");
  }
}

void ble4_scanner()
{
#ifdef BLE4_SCANNER
  ret = bt_le_scan_start(&scan_param, scan_cb);
  if (bt_ret) {
    printq("Starting scanning failed (ret %d)\n", bt_ret);

    return;
  }
  printq("BLE4 Scanning started!\n");
#endif

}

void start_ble5_scanner()
{
#ifdef BLE5_SCANNER
  bt_ret = bt_le_scan_start(&scan_param, scan_cb);
  if (bt_ret) {
    printq("Starting scanning failed (ret %d)\n", bt_ret);

    return;
  }
  printq("BLE5 Scanning started!\n");
#endif
}

void stop_ble5_scanner()
{
  printq("ble5 scanning stopped\n");
  bt_le_scan_stop();
}


void ble4_adv()
{
#ifdef BLE4_ADV

  ret = bt_le_adv_start(adv_param, ad, ARRAY_SIZE(ad),
    NULL, 0);
  if (bt_ret) {
    printq("Advertising failed to start (ret %d)\n", bt_ret);

    return;
  }
  printq("BLE4 Adv started!\n");
#endif
}

void start_ble5_adv()
{
#ifdef BLE5_ADV
  printq("init BLE5 Adv\n");
  //    adv_param->options = BT_LE_ADV_OPT_CONNECTABLE;

  adv_param->options = BT_LE_ADV_OPT_EXT_ADV;
  adv_param->options |= BT_LE_ADV_OPT_CODED;
  adv_param->options |= BT_LE_ADV_OPT_USE_IDENTITY;
  adv_param->id = BT_ID_DEFAULT;
  adv_param->sid = sidCounter++;
  adv_param->interval_min = BT_GAP_ADV_FAST_INT_MIN_2;
  adv_param->interval_max = BT_GAP_ADV_FAST_INT_MAX_2;
  printq("BLE5 packet created\n");
  bt_le_ext_adv_delete(adv);
  bt_ret = bt_le_ext_adv_create(adv_param, NULL, &adv);
  if (bt_ret) {
    printq("Advertising failed to create (ret %d)\n", bt_ret);
    return;
  }
  printq("BLE5 packet converted\n");

  bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
  bt_ret = bt_le_ext_adv_start(adv, NULL);
  if (bt_ret) {
    printq("Advertising failed to start (ret %d)\n", bt_ret);

    return;
  }
  printq("BLE5 Adv started!\n");
#endif
}

void stop_ble5_adv()
{
  printq("ble5 adv stopped\n");
  bt_le_ext_adv_stop(adv);
}

void init_bluetooth()
{
#if defined BLE5_ADV || defined BLE4_ADV || defined BLE5_SCANNER || defined BLE4_SCANNER 
  printq("Starting Bluetooth Subsystem\n");
  bt_ret = bt_enable(NULL);
  if (bt_ret) {
    printq("Bluetooth init failed (ret %d)\n", bt_ret);

    return;
  }
  printq("Bluetooth initialized.");
  bt_addr_le_t addr;
  size_t size(6);
  bt_id_get(&addr, &size);
  print_macaddress(&addr);
  printq("\n");

#endif
}