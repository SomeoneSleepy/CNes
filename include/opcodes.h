#pragma once

#include "PPU.h"

typedef struct cpu_t cpu_t;

typedef enum {
    ADDR_IMMEDIATE,      // e.g. LDA #$10
    ADDR_ABSOLUTE,       // e.g. LDA $1234
    ADDR_ABSOLUTE_X,     // e.g. LDA $1234,X
    ADDR_ABSOLUTE_Y,     // e.g. LDA $1234,Y
    ADDR_ZEROPAGE,       // e.g. LDA $10
    ADDR_ZEROPAGE_X,     // e.g. LDA $10,X
    ADDR_ZEROPAGE_Y,     // e.g. LDX $10,Y (rare)
    ADDR_INDIRECT,       // e.g. JMP ($1234)
    ADDR_INDIRECT_X,     // e.g. LDA ($10,X)
    ADDR_INDIRECT_Y,     // e.g. LDA ($10),Y
    ADDR_RELATIVE,       // e.g. BEQ, BNE, BPL, etc.
    ADDR_ACCUMULATOR,    // e.g. ASL A
    ADDR_IMPLIED,        // e.g. CLC, NOP
    ADDR_NONE            // Invalid/unsupported
} addr_mode_t;


typedef enum {
    LDA, LDX, LDY, STA, BPL, JMP, NOP, 
    TXS, SEI, CLV, CLI, CLD,
    INSTR_INVALID
} instr_t;

typedef struct {
    instr_t instr;
    addr_mode_t mode;
    uint8_t cycles;
    uint8_t bytes;
    int page_cross_penalty; // For indexed modes
} opcode_info_t;

extern opcode_info_t opcode_table[256];

void handle_command(ppu_t* nes_ppu, cpu_t* nes_cpu, uint8_t value, uint16_t addr);