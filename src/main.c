#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "cartridge.h"
#include "memo.h"
#include "CPU.h"


int main(int argc, char** argv)
{
    uint8_t cpu_mem[CPU_MEM_SIZE] = { 0 };
    uint8_t ppu_vram[VRAM_SIZE] = { 0 };
    cpu_t* nes_cpu = (cpu_t*) calloc(1, sizeof(cpu_t));
    ppu_t* nes_ppu = (ppu_t*) calloc(1, sizeof(ppu_t));
    size_t rom_size = 0;

    FILE* opcodes_log = fopen("opcodes_log.txt", "w");

    int running = 1, i = 0;
    if (argc != MAX_ITEMS)
    {
        printf("wrong usage.\nuse emulator.exe <ROM path>\n");
        return 0;
    }
    if(!opcodes_log)
    {
        printf("couldn't write to opcodes log.");
        return 0;
    }

    if(load_cartridge(argv[ROM], cpu_mem, ppu_vram))
    {
        printf("please try again later.\n");
        return 0;
    }
    
    power_on(nes_cpu, cpu_mem);
    ppu_power_on(nes_ppu);

    while(running)
    {
        printf("--------------\n");
        fputs("--------------\n", opcodes_log);
        nes_cpu->cycles = cpu_exec_instr(nes_ppu, nes_cpu, cpu_mem, opcodes_log);
        printf("%d\n", nes_cpu->cycles);
        fflush(opcodes_log);
        for(i = 0; i < nes_cpu->cycles * 3; i++)
        {
            ppu_tick(nes_ppu);
        }

    }

    getchar();
    return 0;
}