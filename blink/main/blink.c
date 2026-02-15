#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <driver/gpio.h>

#define LEDPIN GPIO_NUM_16
void app_main(void)
{
  gpio_reset_pin(LEDPIN);
  gpio_set_direction(LEDPIN, GPIO_MODE_OUTPUT);

  uint32_t level = 0;
  while (1) {
    level = !level;
    gpio_set_level(LEDPIN, level);
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}
