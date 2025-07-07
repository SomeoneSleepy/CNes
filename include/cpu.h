#ifndef CPUH
#define CPUH
#include "io.h"
#include "ppu.h"
#include <stdint.h>
#include <stdio.h>

enum flags {
    C = 0x01,  // bit 0
    Z = 0x02,  // bit 1
    I = 0x04,  // bit 2
    D = 0x08,  // bit 3
    B = 0x10,  // bit 4
    U = 0x20,  // bit 5 (unused/reserved, often set to 1)
    V = 0x40,  // bit 6
    N = 0x80   // bit 7
};

typedef struct cpu_t
{
    uint8_t A;      // Accumulator
    uint8_t X;      // X register
    uint8_t Y;      // Y register
    uint16_t PC;    // Program Counter
    uint8_t SP;     // Stack Pointer
    uint8_t P;      // Processor Status (packed flags byte)
    uint8_t cycles;
} cpu_t;

void power_on(cpu_t* cpu, uint8_t* cpu_mem);
void set_flags(cpu_t* nes, uint8_t value);
void clear_flag(cpu_t* cpu, uint8_t bit_mask);
void set_flag(cpu_t* nes_cpu, uint8_t bit);
int is_flag_clear(cpu_t* cpu, uint8_t bit);
uint8_t fetch_next(cpu_t* cpu, uint8_t* mem);
uint16_t make_address(uint8_t low, uint8_t high);
int cpu_exec_instr(ppu_t* ppu, cpu_t* nes_cpu, uint8_t* mem, FILE* log);
void print_flags(cpu_t* nes_cpu);
void cpu_write_mem(ppu_t* ppu, cpu_t* cpu, uint8_t* mem, uint16_t loc, uint8_t value);
uint8_t cpu_read_mem(ppu_t* ppu, uint16_t loc, uint8_t* mem);

#endif