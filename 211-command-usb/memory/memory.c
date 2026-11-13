#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/regs/addressmap.h"
#include <boards/pico.h>

extern char __flash_binary_start;
extern char __flash_binary_end;
extern char __boot2_start__;
extern char __boot2_end__;
extern char __etext;
extern char __data_start__;
extern char __data_end__;
extern char __bss_start__;
extern char __bss_end__;
extern char __HeapLimit;
extern char __StackBottom;
extern char __StackTop;

static void row(const char *name, uintptr_t start, uintptr_t end) {
  printf("%-10s 0x%08x 0x%08x %8u\n", name, (unsigned)start, (unsigned)end, (unsigned)(end - start));
}


void mem_info() {
  printf("area       start      end        size    \n");
  row("flash", XIP_BASE, XIP_BASE+PICO_FLASH_SIZE_BYTES);
  row("sram", SRAM_BASE, SRAM_END);
  row("rom", ROM_BASE, ROM_BASE + (1<<14));
  row("image", (uintptr_t)&__flash_binary_start, (uintptr_t)&__flash_binary_end);
  row("free", (uintptr_t)&__flash_binary_end, XIP_BASE+PICO_FLASH_SIZE_BYTES);
  row("boot2", (uintptr_t)&__boot2_start__, (uintptr_t)&__boot2_end__);
  row("text", (uintptr_t)&__boot2_end__, (uintptr_t)&__etext);
  row("data flash", (uintptr_t)&__etext, (uintptr_t)(&__etext) + (uintptr_t)(&__data_end__) - (uintptr_t)(&__data_start__));
  row("data ram", (uintptr_t)&__data_start__, (uintptr_t)&__data_end__);
  row("bss", (uintptr_t)&__bss_start__, (uintptr_t)&__bss_end__);
  row("heap", (uintptr_t)&__bss_end__, (uintptr_t)&__HeapLimit);
  row("stack", (uintptr_t)&__StackBottom, (uintptr_t)&__StackTop);

  printf("total\n");
  printf("flash image %8u = boot2 %u + text %u + data %u\n", (unsigned)((uintptr_t)(&__flash_binary_end) - (uintptr_t)(&__flash_binary_start)), (unsigned)((uintptr_t)(&__boot2_end__) - (uintptr_t)(&__boot2_start__)), (unsigned)((uintptr_t)(&__etext) - (uintptr_t)(&__boot2_end__)), (unsigned)((uintptr_t)(&__data_end__) - (uintptr_t)(&__data_start__)));
  printf("flash free  %8u of %u\n", (unsigned)(XIP_BASE+PICO_FLASH_SIZE_BYTES - (uintptr_t)(&__flash_binary_end)), PICO_FLASH_SIZE_BYTES);
  printf("ram used    %8u = data %u + bss %u\n", (unsigned)((uintptr_t)(&__bss_end__) - (uintptr_t)(&__data_start__)), (unsigned)((uintptr_t)(&__data_end__) - (uintptr_t)(&__data_start__)), (unsigned)((uintptr_t)(&__bss_end__) - (uintptr_t)(&__bss_start__)));
  printf("ram free    %8u for heap and %u for stack\n", (unsigned)((uintptr_t)(&__HeapLimit) - (uintptr_t)(&__bss_end__)), (unsigned)((uintptr_t)(&__StackTop) - (uintptr_t)(&__StackBottom)));
}
