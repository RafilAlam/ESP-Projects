#include <stdio.h>
#include <driver/ledc.h>

#define BUZZPIN GPIO_NUM_16

void app_main(void)
{
  ledc_timer_config_t timer = {
    .speed_mode      = LEDC_HIGH_SPEED_MODE,
    .timer_num       = LEDC_TIMER_0,
    .duty_resolution = LEDC_TIMER_10_BIT,
    .freq_hz         = 2000,
    .clk_cfg         = LEDC_AUTO_CLK
  };
  ledc_timer_config(&timer);

  ledc_channel_config_t channel = {
    .gpio_num   = BUZZPIN,
    .speed_mode = LEDC_HIGH_SPEED_MODE,
    .channel    = LEDC_CHANNEL_0,
    .timer_sel  = LEDC_TIMER_0,
    .duty       = 512
  };
  ledc_channel_config(&channel);
  
}
