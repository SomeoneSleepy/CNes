#include "PPU.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void ppu_power_on(ppu_t* ppu)
{
    ppu->PPUCTRL = 0x00;        // Default control register (no NMI, base nametable 0)
    ppu->PPUMASK = 0x00;        // Rendering disabled initially
    ppu->PPUSTATUS = 0x00;      // VBlank clear, sprite 0 hit cleared, some bits set as per reset behavior
    ppu->OAMADDR = 0x00;        // Sprite OAM address starts at 0
    ppu->OAMDMA = 0x00;         // No DMA initially
    ppu->write_toggle = 0;      // Reset write toggle for scroll and address

    ppu->v = 0x0000;            // Current VRAM address
    ppu->t = 0x0000;            // Temporary VRAM address
    ppu->x = 0;                 // Fine X scroll reset

    ppu->buffered_data = 0x00;  // Clear read buffer

    ppu->scanline = -1;         // Pre-render scanline start
    ppu->cycle = 0;             // Start at cycle 0
}

uint8_t ppu_read_register(ppu_t* ppu, uint16_t loc)
{
    switch(loc)
    {
        case 0x2000:
            return ppu->PPUCTRL;
        case 0x2001:
            return ppu->PPUMASK;
        case 0x2002:
            uint8_t status = ppu->PPUSTATUS;
            ppu->PPUSTATUS &= ~0x80;
            ppu->write_toggle = 0;
            return status;
        case 0x2003:
            return ppu->OAMADDR;
        case 0x2004:
            return ppu->OAMDATA;
        case 0x2005:
            return ppu->PPUSCROLL;
        case 0x2006:
            return ppu->PPUADDR;
        case 0x2007:
            return ppu->PPUDATA;
        case 0x4014:
            return ppu->OAMDMA;
        default:
            printf("invalid... 0x%04X\n", loc);
    }
}

void ppu_write_register(ppu_t* ppu, uint16_t addr, uint8_t value)
{
    switch(addr) {
        case 0x2000:  // PPUCTRL
            ppu->PPUCTRL = value;
            ppu->t = (ppu->t & 0xF3FF) | ((value & 0x03) << 10);
            break;

        case 0x2001:  // PPUMASK
            ppu->PPUMASK = value;
            break;
        case 0x2003:  // OAMADDR
            ppu->OAMADDR = value;
            break;

        case 0x2004:  // OAMDATA
            ppu->oam[ppu->OAMADDR] = value;
            ppu->OAMADDR = (ppu->OAMADDR + 1) & 0xFF;
            break;

        case 0x2005:  // PPUSCROLL
            if (ppu->write_toggle == 0) {
                ppu->t = (ppu->t & 0x7FE0) | (value >> 3);
                ppu->x = value & 0x07;
                ppu->write_toggle = 1;
            } else {
                ppu->t = (ppu->t & 0x0C1F) | ((value & 0xF8) << 2) | ((value & 0x07) << 12);
                ppu->write_toggle = 0;
            }
            break;

        case 0x2006:  // PPUADDR
            if (ppu->write_toggle == 0) {
                ppu->t = (ppu->t & 0x00FF) | ((value & 0x3F) << 8);
                ppu->write_toggle = 1;
            } else {
                ppu->t = (ppu->t & 0xFF00) | value;
                ppu->v = ppu->t;
                ppu->write_toggle = 0;
            }
            break;

        case 0x2007:  // PPUDATA
            // VRAM write skipped for now
            // You can add VRAM write logic later
            // Just increment VRAM address based on PPUCTRL bit 2:
            if (ppu->PPUCTRL & 0x04)
                ppu->v += 32;
            else
                ppu->v += 1;
            ppu->v &= 0x3FFF;
            break;

        case 0x4014:  // OAMDMA
            ppu->OAMDMA = value;
            break;
    }
}

void ppu_tick(ppu_t* ppu) 
{
    ppu->cycle++;
    if (ppu->cycle > 340) {
        ppu->cycle = 0;
        ppu->scanline++;
        if (ppu->scanline > 260) {
            ppu->scanline = -1; // pre-render line
            printf("Starting new frame\n");
        }
    }

    if (ppu->scanline == 241 && ppu->cycle == 1) {
        ppu->PPUSTATUS |= 0x80;
        printf("VBlank flag set at scanline %d, cycle %d\n", ppu->scanline, ppu->cycle);
    }

    if (ppu->scanline == -1 && ppu->cycle == 1) {
        ppu->PPUSTATUS &= ~0x80;
        ppu->PPUSTATUS &= ~0x40;
        printf("VBlank flag cleared at scanline %d, cycle %d\n", ppu->scanline, ppu->cycle);
    }
}


