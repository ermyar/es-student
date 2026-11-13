#pragma once

#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/regs/addressmap.h"
#include <boards/pico.h>

extern char __flash_binary_start;
void mem_info();
