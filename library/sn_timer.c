#include "sn_timer.h"
#include "app_timer.h"

APP_TIMER_DEF(m_adv_timer);
APP_TIMER_DEF(m_sleep_timer);


static bool adv_timer_timeout_flag = false; // TODO make it atomic


static void adv_timer_handler(void * p_context);
static void sleep_timer_handler(void * p_context);


void sn_timer_init(void)
{
    ret_code_t err_code = app_timer_init();
    APP_ERROR_CHECK(err_code);
    app_timer_create(&m_adv_timer,APP_TIMER_MODE_SINGLE_SHOT,adv_timer_handler);
    app_timer_create(&m_sleep_timer,APP_TIMER_MODE_SINGLE_SHOT,sleep_timer_handler);
}


void sn_timer_start_adv(uint32_t timeout_ms)
{
    app_timer_start(m_adv_timer,APP_TIMER_TICKS(timeout_ms),NULL);
}


void sn_timer_start_sleep(uint32_t timeout_ms)
{
    app_timer_start(m_sleep_timer,APP_TIMER_TICKS(timeout_ms),NULL);
}

bool sn_timer_get_adv_timeout_flag(void)
{
    bool flag_buffer = false;

    flag_buffer = adv_timer_timeout_flag;
    adv_timer_timeout_flag = false;

    return flag_buffer;
}






















static void adv_timer_handler(void * p_context)
{
  adv_timer_timeout_flag = true;
}

static void sleep_timer_handler(void * p_context)
{
    //DO nothing
}