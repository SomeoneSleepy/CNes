#ifndef PPUH
#define PPUH

#include <stdint.h>

typedef struct ppu_t {
    uint8_t PPUCTRL;     // $2000
    uint8_t PPUMASK;     // $2001
    uint8_t PPUSTATUS;   // $2002 (read-only)
    uint8_t OAMADDR;     // $2003
    uint8_t OAMDATA;     // $2004
    uint8_t PPUSCROLL;     // $2005
    uint8_t PPUADDR;     // $2006
    uint8_t PPUDATA;     // $2007
    uint8_t OAMDMA;      // $4014 (DMA trigger)
    
    uint16_t v;          // current VRAM address
    uint16_t t;          // temporary VRAM address
    uint8_t  x;          // fine X scroll
    uint8_t w;
    uint16_t horizontal;
    uint16_t vertical;
    uint16_t frame;
} ppu_t;


void ppu_power_on(ppu_t* ppu);
uint8_t ppu_read_register(ppu_t* ppu, uint16_t loc);
void ppu_write_register(ppu_t* ppu, uint16_t addr, uint8_t value);
void ppu_tick(ppu_t* ppu);

#endif