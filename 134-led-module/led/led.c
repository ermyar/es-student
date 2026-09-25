#include "led.h"
#include "hardware/gpio.h"
#include <stdio.h>

const uint LED_PIN = 25;

static bool led_state = false;

void led_init() {
  gpio_init(LED_PIN);
  gpio_set_dir(LED_PIN, GPIO_OUT);
}

void led_set(bool led) {
  gpio_put(LED_PIN, led);
  led_state = led;
  printf("led %s\n", led ? "on" : "off");
}

void led_toggle() {
  led_set(!led_state);
}

bool led_is_on() {
  return led_state;
}
