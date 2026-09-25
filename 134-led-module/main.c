#include "led.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <hardware/regs/addressmap.h>
#include <hardware/regs/sio.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 50;

bool get_button_debounce(uint pin) {
  bool state = gpio_get(pin);
  sleep_ms(DEBOUNCE_MS);
  return state && gpio_get(pin);
}

void handle_command(int command) {
  switch (command) {
  case 'e':
    led_set(true);
    break;
  case 'd':
    led_set(false);
    break;
  default:
    printf("unknown command: %c\n", command);
  }
}

int main() {
  stdio_init_all();

  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN);

  led_init();

  bool previous = false;

  while (1) {
    bool current = get_button_debounce(BUTTON_PIN);

    if (previous && !current) {
      led_toggle();
      printf("led %s\n", led_is_on() ? "on" : "off");
    }

    previous = current;

    int command = getchar_timeout_us(0);

    if (command == PICO_ERROR_TIMEOUT) {
      continue;
    }

    handle_command(command);
  }
}
