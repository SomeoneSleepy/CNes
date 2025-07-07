#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memo.h"
#include "opcodes.h"

void power_on(cpu_t* cpu, uint8_t* cpu_mem)
{
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->PC = cpu_mem[0xFFFC] | (cpu_mem[0xFFFD] << 8);
    cpu->SP = 0xFD;
    cpu->P = 0x24;
}

void set_flags(cpu_t* nes, uint8_t value)
{
    if(value == 0)
        nes->P |= Z;
    else
        nes->P &= ~Z;

    if (value & N)
        nes->P |= N;
    else
        nes->P &= ~N;
}

uint8_t fetch_next(cpu_t* cpu, uint8_t* mem)
{
    return mem[cpu->PC++];
}

uint16_t make_address(uint8_t low, uint8_t high)
{
    return (uint16_t)low | ((uint16_t) high << 8);
}

void print_flags(cpu_t* nes_cpu)
{
    printf("P: ");
    for (int i = 7; i >= 0; i--) 
    {
        printf("%c", (nes_cpu->P & (1 << i)) ? '1' : '0');
    }
    printf("\n");
}

void set_flag(cpu_t* nes_cpu, uint8_t bit)
{
    nes_cpu->P |= bit;
}

void clear_flag(cpu_t* cpu, uint8_t bit_mask)
{
    cpu->P &= ~bit_mask;
}

int is_flag_clear(cpu_t* cpu, uint8_t bit)
{
    return (cpu->P & bit) == 0;
}

int cpu_exec_instr(ppu_t* ppu, cpu_t* nes_cpu, uint8_t* mem, FILE* log)
{
    uint8_t value = 0;
    uint16_t addr = 0;
    char opcode_str[10] = { 0 };
    uint8_t ptr = 0, low = 0, high = 0;
    uint8_t opcode = fetch_next(nes_cpu, mem);

    printf("opcode: 0x%02X\n", opcode);
    fprintf(log, "opcode: 0x%02X\n", opcode);

    opcode_info_t command = opcode_table[opcode];
    switch (command.mode)
    {
        case ADDR_IMMEDIATE:
            value = fetch_next(nes_cpu, mem);
            break;
        case ADDR_ZEROPAGE:
            addr = fetch_next(nes_cpu, mem) & 0xFF;
            value = mem[addr];
            break;
        case ADDR_ZEROPAGE_X:
            addr = (fetch_next(nes_cpu, mem) + nes_cpu->X) & 0xFF;
            value = mem[addr];
            break;
        case ADDR_ZEROPAGE_Y:
            addr = (fetch_next(nes_cpu, mem) + nes_cpu->Y) & 0xFF;
            value = mem[addr];
            break;
        case ADDR_ABSOLUTE:
            low = fetch_next(nes_cpu, mem);
            high = fetch_next(nes_cpu, mem);
            printf("low: %02X, high: %02X\n", low, high);
            addr = make_address(low, high);
            value = cpu_read_mem(ppu, addr, mem);
            break;
        case ADDR_ABSOLUTE_X:
            low = fetch_next(nes_cpu, mem);
            high = fetch_next(nes_cpu, mem);
            printf("low: %02X, high: %02X\n", low, high);
            addr = make_address(low, high);
            value = cpu_read_mem(ppu, (addr + nes_cpu->X), mem);
            break;
        case ADDR_ABSOLUTE_Y:
            low = fetch_next(nes_cpu, mem);
            high = fetch_next(nes_cpu, mem);
            printf("low: %02X, high: %02X\n", low, high);
            addr = make_address(low, high);
            value = cpu_read_mem(ppu, (addr + nes_cpu->Y), mem);
            break;
        case ADDR_INDIRECT_X:
            ptr = fetch_next(nes_cpu, mem);
            low = mem[ptr];
            high = mem[(uint8_t)(ptr + 1)];
            addr = make_address(low, high) + nes_cpu->X;
            value = mem[addr];
            break;
        case ADDR_INDIRECT_Y:
            ptr = fetch_next(nes_cpu, mem);
            low = mem[ptr];
            high = mem[(uint8_t)(ptr + 1)];
            addr = make_address(low, high) + nes_cpu->Y;
            value = mem[addr];
            break;
        case ADDR_NONE:
            printf("No address mode\n");
            break;
        case ADDR_RELATIVE:
            value = fetch_next(nes_cpu, mem);
            break;
        default:
            printf("None.");
    }
    handle_commands(command, ppu, nes_cpu, mem, value, addr, log);
    return command.cycles;
}

void cpu_write_mem(ppu_t* ppu, cpu_t* cpu, uint8_t* mem, uint16_t loc, uint8_t value)
{
    printf("Writing the value: %02X - to address: $%04X\n", value, loc);
    if(loc <= 0x1FFF)
    {
        mem[loc % 0x0800] = value;
    }
    else if(loc >= 0x2000 && loc <= 0x3FFF)
    {
        ppu_write_register(ppu, 0x2000 + (loc % 8), value);
    }
    else if(loc == 0x4014)
    {
        ppu_write_register(ppu, loc, value);
    }
    else if(loc >= 0x8000)
    {
        printf("Error! the emulator tried to access the ROM field\n");
    }
    else
    {
        mem[loc] = value;
    }
}

uint8_t cpu_read_mem(ppu_t* ppu, uint16_t loc, uint8_t* mem)
{
    printf("Reading from memory in location: $%04X\n", loc);
    if(loc <= 0x1FFF)
    {
        return mem[loc % 0x0800];
    }
    else if(loc >= 0x2000 && loc <= 0x3FFF)
    {
        return ppu_read_register(ppu, 0x2000 + (loc % 8));
    }
    else if(loc == 0x4014)
    {
        return ppu_read_register(ppu, loc);
    }
    else
    {
        return mem[loc];
    }
}
