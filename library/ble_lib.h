#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sn_common.h"

typedef struct{
    uint8_t temperature;
    uint8_t humidity;
    uint8_t co2;
    uint8_t battery_level;
}ble_lib_node_data_t;


void ble_lib_init(void);

void ble_lib_update_sensor_data(sn_data_t data);

void ble_lib_start_adv(void);

void ble_lib_stop_adv(void);