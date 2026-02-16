#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/timers.h>
#include <driver/gpio.h>

#define LEDPIN GPIO_NUM_16
#define LOOPBLINK 0
#define TASKBLINK 1

#define MODE TASKBLINK

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
  
  
    
#endif
}
