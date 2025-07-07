#ifndef CARTRIDGEH
#define CARTRIDGEH

#include <stdint.h>
#include "cpu.h"

#define HEADER_START 0
#define HEADER_END 16


int load_cartridge(char* cart_path, uint8_t* cpu_mem, uint8_t* vram);

#endif