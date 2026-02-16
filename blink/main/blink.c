#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <driver/ledc.h>
#include <driver/gpio.h>

#define LEDPIN GPIO_NUM_16
#define LOOPBLINK 0
#define TASKBLINK 1
#define TASKFADEBLINK 2

#define MODE TASKFADEBLINK

void app_main(void)
{
  gpio_reset_pin(LEDPIN);
  gpio_set_direction(LEDPIN, GPIO_MODE_OUTPUT);

#if MODE == LOOPBLINK
  uint32_t level = 0;
  while(1) {
    level = !level;
    gpio_set_level(LEDPIN, level);
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
#elif MODE == TASKBLINK
  void Toggle(TimerHandle_t xTimer) {
    uint32_t level = (uint32_t) pvTimerGetTimerID(xTimer);
    level = !level;
    gpio_set_level(LEDPIN, level);
    vTimerSetTimerID(xTimer, (void *) level);
  }
  
  TimerHandle_t timer = xTimerCreate("Blinker", 500 / portTICK_PERIOD_MS, pdTRUE, (void *) 0, Toggle);
  if (timer!=NULL) {
    xTimerStart(timer, 0);
  }
#elif MODE == TASKFADEBLINK
  void Toggle(TimerHandle_t xTimer) {
    uint32_t level = (uint32_t) pvTimerGetTimerID(xTimer);
    level = !level;
    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 8191*level, 500, LEDC_FADE_NO_WAIT);
    vTimerSetTimerID(xTimer, (void *) level);
  }

  ledc_channel_config_t channel = {
    .gpio_num = LEDPIN,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = LEDC_CHANNEL_0,
    .intr_type = LEDC_INTR_DISABLE,
    .timer_sel = LEDC_TIMER_0,
    .duty = 0,
    .hpoint = 0,
  };
  ledc_channel_config(&channel);

  ledc_timer_config_t timer = {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_13_BIT,
    .timer_num = LEDC_TIMER_0,
    .freq_hz = 4000,
    .clk_cfg = LEDC_AUTO_CLK,
  };
  ledc_timer_config(&timer);

  ledc_fade_func_install(0);
  
  TimerHandle_t timerHandle = xTimerCreate("Blinker", 500 / portTICK_PERIOD_MS, pdTRUE, (void *) 0, Toggle);
  if (timerHandle!=NULL) {
    xTimerStart(timerHandle, 0);
  }
  
    
#endif
}
