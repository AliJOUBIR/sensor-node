#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "sn_common.h"


/**@brief Function for initializing the sensor node timers
 */
sn_error_t sn_timer_init(void);

/**@brief Function for starting the advertising timer
 */
sn_error_t sn_timer_start_adv(uint32_t timeout_ms);

/**@brief Function for starting the sleep timer
 */
sn_error_t sn_timer_start_sleep(uint32_t timeout_ms);

/**@brief Function for getting advertising timer timeout flag
 */
bool sn_timer_get_adv_timeout_flag(void);