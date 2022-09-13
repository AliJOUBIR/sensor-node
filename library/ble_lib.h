#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sn_common.h"



sn_error_t ble_lib_init(void);

sn_error_t ble_lib_update_sensor_data(sn_data_t data);

sn_error_t ble_lib_start_adv(void);

sn_error_t ble_lib_stop_adv(void);