/*
 * THIS FILE IS AUTOMATICALLY GENERATED AND MUST NOT BE EDITED MANUALLY!
 *
 * I2C-Generator: 0.2.0
 * Yaml Version: 0.1.0
 * Template Version: 0.2.1
 */
/*
 * Copyright (c) 2021, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "scd4x_i2c.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_i2c_hal.h"

#define SCD4X_I2C_ADDRESS 98

int16_t scd4x_start_periodic_measurement(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x21B1);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,1000);
    return NO_ERROR;
}

int16_t scd4x_read_measurement_ticks(scd4x_t  const * const me,uint16_t* co2, uint16_t* temperature,
                                     uint16_t* humidity) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0xEC05);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,1000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        return error;
    }
    *co2 = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    *temperature = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    *humidity = sensirion_common_bytes_to_uint16_t(&buffer[4]);
    return NO_ERROR;
}

int16_t scd4x_read_measurement(scd4x_t  const * const me,uint16_t* co2, int32_t* temperature_m_deg_c,
                               int32_t* humidity_m_percent_rh) {
    int16_t error;
    uint16_t temperature;
    uint16_t humidity;

    error = scd4x_read_measurement_ticks(me,co2, &temperature, &humidity);
    if (error) {
        return error;
    }
    *temperature_m_deg_c = ((21875 * (int32_t)temperature) >> 13) - 45000;
    *humidity_m_percent_rh = ((12500 * (int32_t)humidity) >> 13);
    return NO_ERROR;
}

int16_t scd4x_stop_periodic_measurement(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x3F86);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,500000);
    return NO_ERROR;
}

int16_t scd4x_get_temperature_offset_ticks(scd4x_t  const * const me,uint16_t* t_offset) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x2318);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,1000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *t_offset = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_get_temperature_offset(scd4x_t  const * const me,int32_t* t_offset_m_deg_c) {
    int16_t error;
    uint16_t t_offset;

    error = scd4x_get_temperature_offset_ticks(me,&t_offset);
    if (error) {
        return error;
    }
    *t_offset_m_deg_c = ((21875 * (int32_t)t_offset) >> 13);
    return NO_ERROR;
}

int16_t scd4x_set_temperature_offset_ticks(scd4x_t  const * const me,uint16_t t_offset) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x241D);

    offset = sensirion_i2c_add_uint16_t_to_buffer(me,&buffer[0], offset, t_offset);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,1000);
    return NO_ERROR;
}

int16_t scd4x_set_temperature_offset(scd4x_t  const * const me,int32_t t_offset_m_deg_c) {
    uint16_t t_offset = (uint16_t)((t_offset_m_deg_c * 12271) >> 15);
    return scd4x_set_temperature_offset_ticks(me,t_offset);
}

int16_t scd4x_get_sensor_altitude(scd4x_t  const * const me,uint16_t* sensor_altitude) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x2322);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,1000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *sensor_altitude = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_set_sensor_altitude(scd4x_t  const * const me,uint16_t sensor_altitude) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x2427);

    offset = sensirion_i2c_add_uint16_t_to_buffer(me,&buffer[0], offset,
                                                  sensor_altitude);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,1000);
    return NO_ERROR;
}

int16_t scd4x_set_ambient_pressure(scd4x_t  const * const me,uint16_t ambient_pressure) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0xE000);

    offset = sensirion_i2c_add_uint16_t_to_buffer(me,&buffer[0], offset,
                                                  ambient_pressure);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,1000);
    return NO_ERROR;
}

int16_t scd4x_perform_forced_recalibration(scd4x_t  const * const me,uint16_t target_co2_concentration,
                                           uint16_t* frc_correction) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x362F);

    offset = sensirion_i2c_add_uint16_t_to_buffer(me,&buffer[0], offset,
                                                  target_co2_concentration);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,400000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *frc_correction = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_get_automatic_self_calibration(scd4x_t  const * const me,uint16_t* asc_enabled) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x2313);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,1000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *asc_enabled = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_set_automatic_self_calibration(scd4x_t  const * const me,uint16_t asc_enabled) {
    int16_t error;
    uint8_t buffer[5];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x2416);

    offset =
        sensirion_i2c_add_uint16_t_to_buffer(me,&buffer[0], offset, asc_enabled);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,1000);
    return NO_ERROR;
}

int16_t scd4x_start_low_power_periodic_measurement(scd4x_t  const * const me) {
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x21AC);

    return sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
}

int16_t scd4x_get_data_ready_flag(scd4x_t  const * const me,bool* data_ready_flag) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    uint16_t local_data_ready = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0xE4B8);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,1000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    local_data_ready = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    *data_ready_flag = (local_data_ready & 0x07FF) != 0;
    return NO_ERROR;
}

int16_t scd4x_persist_settings(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x3615);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,800000);
    return NO_ERROR;
}

int16_t scd4x_get_serial_number(scd4x_t  const * const me,uint16_t* serial_0, uint16_t* serial_1,
                                uint16_t* serial_2) {
    int16_t error;
    uint8_t buffer[9];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x3682);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,1000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 6);
    if (error) {
        return error;
    }
    *serial_0 = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    *serial_1 = sensirion_common_bytes_to_uint16_t(&buffer[2]);
    *serial_2 = sensirion_common_bytes_to_uint16_t(&buffer[4]);
    return NO_ERROR;
}

int16_t scd4x_perform_self_test(scd4x_t  const * const me,uint16_t* sensor_status) {
    int16_t error;
    uint8_t buffer[3];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x3639);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }

    sensirion_i2c_hal_sleep_usec(me,10000000);

    error = sensirion_i2c_read_data_inplace(me,SCD4X_I2C_ADDRESS, &buffer[0], 2);
    if (error) {
        return error;
    }
    *sensor_status = sensirion_common_bytes_to_uint16_t(&buffer[0]);
    return NO_ERROR;
}

int16_t scd4x_perform_factory_reset(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x3632);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,800000);
    return NO_ERROR;
}

int16_t scd4x_reinit(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x3646);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,20000);
    return NO_ERROR;
}

int16_t scd4x_measure_single_shot(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x219D);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,5000000);
    return NO_ERROR;
}

int16_t scd4x_measure_single_shot_rht_only(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x2196);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,50000);
    return NO_ERROR;
}

int16_t scd4x_power_down(scd4x_t  const * const me) {
    int16_t error;
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x36E0);

    error = sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    if (error) {
        return error;
    }
    sensirion_i2c_hal_sleep_usec(me,1000);
    return NO_ERROR;
}

int16_t scd4x_wake_up(scd4x_t  const * const me) {
    uint8_t buffer[2];
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(me,&buffer[0], offset, 0x36F6);

    // Sensor does not acknowledge the wake-up call, error is ignored
    (void)sensirion_i2c_write_data(me,SCD4X_I2C_ADDRESS, &buffer[0], offset);
    sensirion_i2c_hal_sleep_usec(me,20000);
    return NO_ERROR;
}
