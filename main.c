

#include <stdbool.h>
#include <stdint.h>
#include "nordic_common.h"
#include "bsp.h"
#include "nrf_soc.h"
#include "nrf_sdh.h"
#include "nrf_sdh_ble.h"
#include "ble_advdata.h"

#include "nrf_pwr_mgmt.h"
#include "nrf_delay.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "ble_lib.h"
#include "sn_timer.h"
#include "sn_common.h"

#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"




#define SLEEP_PERIODE   (180000)    /* time of sleep in milliseconds */
#define ADV_PERIODE     (60000)     /* time of advertising in milliseconds */



/**@brief Function for initializing logging. */
static void log_init(void)
{
    ret_code_t err_code = NRF_LOG_INIT(NULL);
    APP_ERROR_CHECK(err_code);

    NRF_LOG_DEFAULT_BACKENDS_INIT();
}


/**@brief Function for initializing power management.
 */
static void power_management_init(void)
{
    ret_code_t err_code;
    err_code = nrf_pwr_mgmt_init();
    APP_ERROR_CHECK(err_code);
}

/**@brief Function for handling the idle state (main loop).
 *
 * @details If there is no pending log operation, then sleep until next the next event occurs.
 */
static void idle_state_handle(void)
{
    nrf_pwr_mgmt_run();
}

typedef enum
{
    DATA_STATE,                
    ADV_STATE,                
    SLEEP_STATE                    
} app_state_t;

typedef enum
{
    ENTRY,
    DO,
    EXIT
} intern_state_t;

static app_state_t state = DATA_STATE;
const uint8_t DATA_AVG_NUMBER = 50; 

void app_data_state_task()
{
    static sn_data_t data;
    static intern_state_t intern_state = ENTRY;
    static uint8_t sensor_data_avg_counter = 0;
    

    switch (intern_state)
    {
    case ENTRY:
        NRF_LOG_INFO("DATA STATE");
        // init i2c
        // init sensor
        data.humidity = 0xee;// set data to 0
        intern_state = DO;
        break;
    case DO:
        //read sensor data
        //calculate average data
        sensor_data_avg_counter++;
        data.temperature = sensor_data_avg_counter;
        data.humidity = 0xee;
        data.co2 = 0xdd;
        data.battery_level = 0xcc;
        if (sensor_data_avg_counter >= DATA_AVG_NUMBER)
        {
            sensor_data_avg_counter = 0;
            intern_state = EXIT;
        }
        break;
    case EXIT:
        // uninit sensor or go to sensor sleep model
        // uninit i2c
        NRF_LOG_INFO("DATA STATE EXIT");
        ble_lib_update_sensor_data(data);
        intern_state = ENTRY;
        state = ADV_STATE;
        break;    
    default:
        break;
    }
    
}

void app_adv_state_task()
{
    static intern_state_t intern_state = ENTRY;
    switch (intern_state)
    {
    case ENTRY:
        NRF_LOG_INFO("ADV STATE");
        ble_lib_start_adv();
        sn_timer_start_adv(ADV_PERIODE);  //1 min
        intern_state = DO;
        break;
    case DO:
        if (sn_timer_get_adv_timeout_flag() == true)
        {
            intern_state = EXIT;
        }
        break;
    case EXIT:
        NRF_LOG_INFO("ADV STATE EXIT");
        ble_lib_stop_adv();
        state = SLEEP_STATE;
        intern_state = ENTRY;
        break;    
    default:
        break;
    }
      
}

void app_sleep_state_task()
{
    sn_timer_start_sleep(SLEEP_PERIODE); 
    idle_state_handle();
    idle_state_handle();
    state = DATA_STATE;
    NRF_LOG_INFO("SLEEP STATE\r\n");
}


static void app_task(void)
{

    switch(state)
    {
        case DATA_STATE:
            app_data_state_task();
        break;
        case ADV_STATE:
            app_adv_state_task();
        break;
        case SLEEP_STATE:
            app_sleep_state_task();
        break;
    }

}

/**
 * @brief Function for application main entry.
 */
int main(void)
{
    // Initialize.
    log_init();
    sn_timer_init();
    power_management_init();
    ble_lib_init();


    
    sensirion_i2c_hal_init();

    // Clean up potential SCD40 states
    scd4x_wake_up();
    scd4x_stop_periodic_measurement();
    scd4x_reinit();

    
    // Start execution.
    NRF_LOG_INFO("Sensor node started.\r\n");


    for (;;)
    {
        app_task();
        while(NRF_LOG_PROCESS());
    }
}


/**
 * @}
 */
