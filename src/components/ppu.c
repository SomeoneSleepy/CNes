#include "PPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ppu_power_on(ppu_t* ppu)
{
    ppu->PPUCTRL = 0x00;
    ppu->PPUMASK = 0x00;
    ppu->PPUSTATUS = 0x00;
    ppu->OAMADDR = 0x00;
    ppu->OAMDMA = 0x00;

    ppu->v = 0x0000;
    ppu->t = 0x0000;
    ppu->x = 0;
    ppu->w = 0;

    ppu->horizontal = 0;   // cycle count (0-340)
    ppu->vertical = 261;   // pre-render scanline (261)
    ppu->frame = 0;
}

uint8_t ppu_read_register(ppu_t* ppu, uint16_t loc)
{
    switch(loc)
    {
        case 0x2002:
            uint8_t status = ppu->PPUSTATUS;
            printf("status: %02X\n", ppu->PPUSTATUS);
            ppu->PPUSTATUS &= ~0x80;
            ppu->w = 0;
            return status;
        default:
            return 0;
    }
}

void ppu_write_register(ppu_t* ppu, uint16_t addr, uint8_t value)
{
    switch(addr) {
        case 0x2000:  // PPUCTRL
            ppu->PPUCTRL = value;
            ppu->t = (ppu->t & 0xF3FF) | ((value & 0x03) << 10);
            break;
    }
}

void ppu_tick(ppu_t* ppu) 
{
    // Advance horizontal (cycle)
    ppu->horizontal++;

    if (ppu->horizontal > 340) {
        ppu->horizontal = 0;
        ppu->vertical++;

        if (ppu->vertical > 261) {
            ppu->vertical = 0;
            ppu->frame++;
        }
    }

    // VBlank starts at scanline 241, cycle 1
    if (ppu->vertical == 241 && ppu->horizontal == 1) {
        ppu->PPUSTATUS |= 0x80;  // Set VBlank flag
        printf("VBlank set at frame %d\n", ppu->frame);
    }

    // VBlank ends at pre-render scanline 261, cycle 1
    if (ppu->vertical == 261 && ppu->horizontal == 1) {
        ppu->PPUSTATUS &= ~0x80; // Clear VBlank flag
        ppu->PPUSTATUS &= ~0x40; // Clear sprite 0 hit
        ppu->PPUSTATUS &= ~0x20; // Clear sprite overflow
        printf("VBlank cleared at frame %d\n", ppu->frame);
    }
}




