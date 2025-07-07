#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "opcodes.h"
#include "CPU.h"

opcode_info_t opcode_table[256] = {
    [0x10] = { BPL, ADDR_RELATIVE,    2, 2, true  }, // Branch if Positive
    [0x58] = { CLI, ADDR_NONE,        1, 1, true  }, // Clear Interrupt Disable
    [0x78] = { SEI, ADDR_NONE,        1, 1, true  }, // Set Interrupt Disable
    [0x9A] = { TXS, ADDR_NONE,        1, 1, true  }, // Transfer X to Stack Pointer
    [0x8D] = { STA, ADDR_ABSOLUTE,    4, 3, false }, // Store Accumulator
    [0xA0] = { LDY, ADDR_IMMEDIATE,   2, 2, false }, // Load Y Immediate
    [0xA1] = { LDA, ADDR_INDIRECT_X,  6, 2, false }, // Load A (Indirect,X)
    [0xA2] = { LDX, ADDR_IMMEDIATE,   2, 2, false }, // Load X Immediate
    [0xA4] = { LDY, ADDR_ZEROPAGE,    3, 2, false }, // Load Y Zero Page
    [0xA5] = { LDA, ADDR_ZEROPAGE,    3, 2, false }, // Load A Zero Page
    [0xA6] = { LDX, ADDR_ZEROPAGE,    3, 2, false }, // Load X Zero Page
    [0xA9] = { LDA, ADDR_IMMEDIATE,   2, 2, false }, // Load A Immediate
    [0xAC] = { LDY, ADDR_ABSOLUTE,    4, 3, false }, // Load Y Absolute
    [0xAD] = { LDA, ADDR_ABSOLUTE,    4, 3, false }, // Load A Absolute
    [0xAE] = { LDX, ADDR_ABSOLUTE,    4, 3, false }, // Load X Absolute
    [0xB1] = { LDA, ADDR_INDIRECT_Y,  5, 2, true  }, // Load A (Indirect),Y
    [0xB4] = { LDY, ADDR_ZEROPAGE_X,  4, 2, false }, // Load Y Zero Page,X
    [0xB6] = { LDX, ADDR_ZEROPAGE_Y,  4, 2, false }, // Load X Zero Page,Y
    [0xB8] = { CLV, ADDR_NONE,        1, 1, true  }, // Clear Overflow Flag
    [0xB9] = { LDA, ADDR_ABSOLUTE_Y,  4, 3, true  }, // Load A Absolute,Y
    [0xBC] = { LDY, ADDR_ABSOLUTE_X,  4, 3, false }, // Load Y Absolute,X
    [0xBD] = { LDA, ADDR_ABSOLUTE_X,  4, 3, true  }, // Load A Absolute,X
    [0xBE] = { LDX, ADDR_ABSOLUTE_Y,  4, 3, true  }, // Load X Absolute,Y
    [0xD8] = { CLD, ADDR_NONE,        1, 1, true  }, // Clear Decimal Mode
};

void handle_command(ppu_t* nes_ppu, cpu_t* nes_cpu, uint8_t value, uint16_t addr)
{
    printf("register value: 0x%02X\n", value);
    printf("address: 0x%04X\n", addr);
    switch(nes_cpu->opcode.instr)
    {
        case LDA:
            printf("LDA\n");
            nes_cpu->A = value;
            set_flags(nes_cpu, value);
            printf("A: 0x%02X\n", nes_cpu->A);
            print_flags(nes_cpu);
            break;
        case LDY:
            printf("LDY\n");
            nes_cpu->Y = value;
            set_flags(nes_cpu, value);
            printf("Y: 0x%02X\n", nes_cpu->Y);
            print_flags(nes_cpu);
            break;
        case STA:
            printf("STA\nA: %d\n", nes_cpu->A);
            cpu_write_mem(nes_ppu, nes_cpu, nes_cpu->mem, addr, nes_cpu->A);
            printf("Memory[%04X] = $%02X\n", addr, nes_cpu->A);
            break;
        case LDX:
            printf("LDX\n");
            nes_cpu->X = value;
            set_flags(nes_cpu, value);
            printf("X: 0x%02X\n", nes_cpu->X);
            print_flags(nes_cpu);
            break;
        case CLD:
            printf("CLD\n");
            clear_flag(nes_cpu, D);
            print_flags(nes_cpu);
            break;
        case SEI:
            printf("SEI\n");
            set_flag(nes_cpu, I);
            print_flags(nes_cpu);
            break;
        case TXS:
            printf("TXS\n");
            nes_cpu->SP = nes_cpu->X;
            printf("SP: %02X\n", nes_cpu->SP);
            break;
        case BPL:
            printf("BPL\n");
            print_flags(nes_cpu);
            if(is_flag_clear(nes_cpu, N))
            {
                nes_cpu->PC += (int8_t) value;
                printf("Branch taken: new PC = %04X\n", nes_cpu->PC);
            }
            else
            {
                printf("Branch not taken\n");
            }
            break;
        default:
            printf("invalid instruction: %02X\n", nes_cpu->opcode.instr);
            exit(1);
    }
}
