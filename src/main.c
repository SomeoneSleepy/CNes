#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "cartridge.h"
#include "memo.h"
#include "cpu.h"


int main(int argc, char** argv)
{
    uint8_t cpu_mem[CPU_MEM_SIZE] = { 0 };
    uint8_t ppu_vram[VRAM_SIZE] = { 0 };
    cpu_t* nes_cpu = (cpu_t*) calloc(1, sizeof(cpu_t));
    ppu_t* nes_ppu = (ppu_t*) calloc(1, sizeof(ppu_t));
    size_t rom_size = 0;

    int running = 1, i = 0;
    if (argc != MAX_ITEMS)
    {
        printf("wrong usage.\nuse emulator.exe <ROM path>\n");
        return 0;
    }

    if(load_cartridge(argv[ROM], cpu_mem, ppu_vram))
    {
        printf("please try again later.\n");
        return 0;
    }

    nes_cpu->mem = cpu_mem;
    power_on(nes_cpu);
    ppu_power_on(nes_ppu);

    while(running)
    {
        printf("--------------\n");
        cpu_tick(nes_cpu, nes_ppu);
        printf("opcode: 0x%02X, low: 0x%02X, high: 0x%02X\n", 
           nes_cpu->instr, nes_cpu->low, nes_cpu->high);
        for(i = 0; i < 3; i++)
        {
            printf("ppu\n");
            // ppu_tick(nes_ppu);
        }
        getchar();
    }

    getchar();
    return 0;
}