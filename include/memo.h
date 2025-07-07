#ifndef MEMOH
#define MEMOH
#include <stdint.h>
#include "ppu.h"
#include "cpu.h"

#define MAX_ITEMS 2
#define ROM 1
#define LOW 0
#define HIGH 1
#define MAX_MEM_SIZE 0x10000
#define CPU_MEM_SIZE 0x10000
#define VRAM_SIZE 0x4000
#define BASE_GAME_SIZE 16384
#define BASE_TILES_SIZE 8192
#define RAM_START 0x000
#define RAM_END 0x1FFF
#define ROM_START 0x8000
#define VRAM_START 0x0000
#define NUMBER_SIZE 2

int load_data(uint8_t* memory, size_t starting_index, uint8_t* addition, size_t addition_size);
void memory_log(uint8_t* memory);

#endif