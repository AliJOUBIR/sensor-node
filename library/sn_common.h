#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct{
    uint8_t temperature;
    uint8_t humidity;
    uint8_t co2;
    uint8_t battery_level;
}sn_data_t;