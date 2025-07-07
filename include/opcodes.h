#pragma once

<<<<<<< HEAD
#include "PPU.h"

typedef struct cpu_t cpu_t;
=======
#include "cpu.h"
>>>>>>> 1d15b808b5b1990397be2648fa1c9ebd3608521e

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

<<<<<<< HEAD
void handle_command(ppu_t* nes_ppu, cpu_t* nes_cpu, uint8_t value, uint16_t addr);
=======
void handle_commands(opcode_info_t operation, ppu_t* nes_ppu, cpu_t* nes_cpu, uint8_t* mem, uint8_t value, uint16_t addr, FILE* log);

#endif
>>>>>>> 1d15b808b5b1990397be2648fa1c9ebd3608521e
