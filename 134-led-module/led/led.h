#ifndef LED_H
#define LED_H

#include "pico/stdlib.h"
#include <stdbool.h>

void led_init();
void led_set(bool);
void led_toggle();
bool led_is_on();

#endif


