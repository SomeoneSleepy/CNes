#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "cartridge.h"
#include "memo.h"
#include "cpu.h"


void gui_render(SDL_Renderer* renderer);

int main(int argc, char** argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    uint8_t cpu_mem[CPU_MEM_SIZE] = { 0 };
    uint8_t ppu_vram[VRAM_SIZE] = { 0 };
    cpu_t* nes_cpu = (cpu_t*) calloc(1, sizeof(cpu_t));
    ppu_t* nes_ppu = (ppu_t*) calloc(1, sizeof(ppu_t));
    nes_cpu->addr = (uint8_t*) calloc(2, sizeof(uint8_t));
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

    SDL_Window* window = SDL_CreateWindow("cNz Emulator",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          256, 260, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);                            
    SDL_Event event;

    nes_cpu->mem = cpu_mem;
    power_on(nes_cpu);
    ppu_power_on(nes_ppu);


    while(running)
    {
        cpu_tick(nes_cpu, nes_ppu);
        printf("opcode: 0x%02X, low: 0x%02X, high: 0x%02X\n", 
            nes_cpu->instr, nes_cpu->addr[LOW], nes_cpu->addr[HIGH]);
            
        for(i = 0; i < 3; i++)
        {
            ppu_tick(nes_ppu);
        }
        gui_render(renderer);
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    getchar();
    return 0;
}

void gui_render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
    SDL_RenderClear(renderer);

    // Draw your GUI here (menu bars, buttons, PPU frame, etc.)

    SDL_RenderPresent(renderer);
}