#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "sn_common.h"

typedef struct{
    void *i2c_instance;
    struct
    {
        bool co2;
        bool temperature;
        bool humidity;
        bool battery_level;
    }flag;
}sn_sensor_t;

void sn_sensor_init(sn_sensor_t *me);

bool sn_sensor_get_data(sn_sensor_t *me,sn_data_t *data);

void sn_sensor_wakeup(sn_sensor_t *me);

void sn_sensor_sleep(sn_sensor_t *me);
