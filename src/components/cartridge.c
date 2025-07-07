#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "cartridge.h"
#include "memo.h"
#include "cpu.h"

int load_cartridge(char* cart_path, uint8_t* cpu_mem, uint8_t* vram)
{
    uint8_t prg_rom_size = 0, chr_rom_size = 0;
    int chr_res = 0, prg_res = 0;
    FILE* game = fopen(cart_path, "rb");

    if (!game)
    {
        printf("Invalid file location.\nPlease re-launch and type a valid path.");
        return 1;
    }

    fseek(game, 4, SEEK_SET);

    fread(&prg_rom_size, sizeof(uint8_t), 1, game);
    fread(&chr_rom_size, sizeof(uint8_t), 1, game);

    uint8_t* prg_rom = (uint8_t*) calloc(prg_rom_size * BASE_GAME_SIZE, sizeof(uint8_t));
    uint8_t* chr_rom = (uint8_t*) calloc(chr_rom_size * BASE_TILES_SIZE, sizeof(uint8_t));

    fseek(game, HEADER_END, SEEK_SET);

    prg_res = fread(prg_rom, sizeof(uint8_t), prg_rom_size * BASE_GAME_SIZE, game) != prg_rom_size * BASE_GAME_SIZE;
    chr_res = fread(chr_rom, sizeof(uint8_t), chr_rom_size * BASE_TILES_SIZE, game) != chr_rom_size * BASE_TILES_SIZE;
    
    if(prg_res || chr_res)
    {
        printf("Failed to load the program rom bytes from cartridge");
        return 1;
    }

    prg_res = load_data(cpu_mem, ROM_START, prg_rom, prg_rom_size * BASE_GAME_SIZE);
    chr_res = load_data(vram, VRAM_START, chr_rom, chr_rom_size * BASE_TILES_SIZE);

    if(prg_res || chr_res)
    {
        printf("Unable to load the game cartridge into memory");
        return 1;
    }
    fclose(game);
    return 0;
}
