#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct{
    uint16_t co2;
    int32_t temperature;
    int32_t humidity;
    uint8_t battery_level;
}sn_data_t;


typedef enum{
    SN_SUCCESS,
    SN_ERROR
}sn_error_t;
