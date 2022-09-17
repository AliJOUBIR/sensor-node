#include "sn_sensor.h"
#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c_hal.h"







sn_error_t sn_sensor_init(sn_sensor_t * const me)
{
    sensirion_i2c_hal_init(&me->sensor.i2c);
    return SN_SUCCESS;
}

sn_error_t sn_sensor_get_data(sn_sensor_t const * const me,sn_data_t * const data)
{
    uint16_t co2;
    int32_t temperature;
    int32_t humidity;
    bool data_ready_flag = false;
    uint32_t error = SN_SUCCESS;

    error = scd4x_get_data_ready_flag(&me->sensor,&data_ready_flag);

    if ((data_ready_flag == true) && (error == false))
    {
      error = scd4x_read_measurement(&me->sensor,&co2, &temperature, &humidity);
      data->co2 = co2;
      data->temperature = temperature;
      data->humidity = humidity;
    }

    return error;
}

sn_error_t sn_sensor_wakeup(sn_sensor_t const * const me)
{
    uint32_t error = SN_SUCCESS;
    scd4x_wake_up(&me->sensor);
    scd4x_stop_periodic_measurement(&me->sensor);
    scd4x_reinit(&me->sensor);

    error = scd4x_start_periodic_measurement(&me->sensor);

    return error;
}

sn_error_t sn_sensor_sleep(sn_sensor_t const * const me)
{
    scd4x_stop_periodic_measurement(&me->sensor);
    scd4x_power_down(&me->sensor);
    return SN_SUCCESS;
}