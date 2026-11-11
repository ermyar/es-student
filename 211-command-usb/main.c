#include "led.h"
#include "log.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <hardware/regs/addressmap.h>
#include <hardware/regs/sio.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "device.h"

const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 50;

const char* cmd_enable = "enable";
const char* cmd_disable = "disable";
const char* cmd_info = "info";
const char* cmd_version = "version";

int line_length = 0;
char line[BUFSIZ];

bool get_button_debounce(uint pin) {
  bool state = gpio_get(pin);
  sleep_ms(DEBOUNCE_MS);
  return state && gpio_get(pin);
}

void handle_command(const char* command) {
  if (strcmp(command, cmd_enable) == 0) {
    led_set(true);
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    return;
  }
  if (strcmp(command, cmd_disable) == 0) {
    led_set(false);
    LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    return;
  }
  if (strcmp(command, cmd_info) == 0) {
    device_info();
    return;
  }
  if (strcmp(command, cmd_version) == 0) {
    log_version();
    return;
  }

  LOG_ERR("unknown command: %s\n", command);
}

void read_line() {
  int command = getchar_timeout_us(0);

  if (command == PICO_ERROR_TIMEOUT) {
    return;
  }

  LOG_DBG("got %c\n", command);
  if (command == '\n' || command == '\r') {
    putchar('\n');
    line[line_length] = '\0';
    handle_command(line);
    line_length = 0;
    return;
  }


  if (line_length + 1 < BUFSIZ) {
    line[line_length] = command;
    line_length++;
    putchar(command);
  }
}

int main() {
  stdio_init_all();

  gpio_init(BUTTON_PIN);
  gpio_set_dir(BUTTON_PIN, GPIO_IN);
  gpio_pull_up(BUTTON_PIN);

  led_init();

  bool previous = false;
  int len = 0;

  while (1) {
    bool current = get_button_debounce(BUTTON_PIN);

    if (previous && !current) {
      led_toggle();
      LOG_INF("led %s\n", led_is_on() ? "on" : "off");
    }

    previous = current;

    read_line();
  }
}
