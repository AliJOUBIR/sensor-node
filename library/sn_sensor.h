#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "sn_common.h"
#include "sensirion_config.h"


typedef struct{
    scd4x_t sensor;
    struct
    {
        bool co2;
        bool temperature;
        bool humidity;
        bool battery_level;
    }flag;
}sn_sensor_t;





sn_error_t sn_sensor_init(sn_sensor_t * const me);

sn_error_t sn_sensor_get_data(sn_sensor_t const * const me,sn_data_t *data);

sn_error_t sn_sensor_wakeup(sn_sensor_t const * const me);

sn_error_t sn_sensor_sleep(sn_sensor_t const * const me);
