#include "led.h"
#include "log.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include <hardware/regs/addressmap.h>
#include <hardware/regs/sio.h>
#include <pico/stdio.h>
#include <pico/time.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "device.h"
#include "memory.h"

const uint BUTTON_PIN = 15;
const uint DEBOUNCE_MS = 50;

int line_length = 0;
char line[BUFSIZ];

bool get_button_debounce(uint pin) {
  bool state = gpio_get(pin);
  sleep_ms(DEBOUNCE_MS);
  return state && gpio_get(pin);
}

void cmd_enable(void) {
  led_set(true);
  LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_disable(void) {
  led_set(false);
  LOG_INF("led %s\n", led_is_on() ? "on" : "off");
}

void cmd_info(void) {
  device_info();
}

void cmd_version(void) {
  log_version();
}

void cmd_ping(void) {
  printf("pong\n");
}

void cmd_mem_info(void) {
  mem_info();
}

typedef void (*command_handler_t)(void);

struct command_t {
  const char *name;
  command_handler_t handler;
};

const struct command_t commands[] = {
  { "enable", cmd_enable},
  {"disable", cmd_disable},
  {"info", cmd_info},
  {"version", cmd_version},
  {"ping", cmd_ping},
  {"mem_info", cmd_mem_info}
};

void handle_command(const char* cmd) {
  size_t commands_len = sizeof(commands) / sizeof(commands[0]);
  for (size_t i = 0; i < commands_len; ++i) {
    if (strcmp(commands[i].name, cmd) == 0) {
      if (commands[i].handler != NULL) {
        commands[i].handler();
        return;
      }
    }
  }
  LOG_ERR("unknown command: %s\n", cmd);
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
