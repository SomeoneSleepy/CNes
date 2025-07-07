#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"
#include "memo.h"
#include "opcodes.h"

void power_on(cpu_t* cpu)
{
    cpu->A = 0;
    cpu->X = 0;
    cpu->Y = 0;
    cpu->PC = cpu->mem[0xFFFC] | (cpu->mem[0xFFFD] << 8);
    cpu->SP = 0xFD;
    cpu->P = 0x24;
    cpu->handling_cmd = false;
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

uint8_t fetch_next(cpu_t* cpu)
{
    return cpu->mem[cpu->PC++];
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

void cpu_tick(cpu_t* nes_cpu, ppu_t* nes_ppu)
{
    if(!nes_cpu->handling_cmd)
    {
        nes_cpu->instr = fetch_next(nes_cpu);
        nes_cpu->opcode = opcode_table[nes_cpu->instr];
        nes_cpu->byte = nes_cpu->opcode.bytes - 1;
        nes_cpu->handling_cmd = true;
        return;
    }
    switch(nes_cpu->byte)
    {
        case 0:
            cpu_exec_instr(nes_cpu, nes_ppu);
            nes_cpu->handling_cmd = false;
            return;
        case 1:
            nes_cpu->low = fetch_next(nes_cpu);
            break;
        case 2:
            nes_cpu->high = fetch_next(nes_cpu);
            break;
        default:
            printf("None. %d\n", nes_cpu->byte);
    }
    nes_cpu->byte -= 1;
}

void cpu_exec_instr(cpu_t* nes_cpu, ppu_t* nes_ppu)
{
    uint8_t value = 0, msb = 0, lsb = 0;
    uint16_t addr = 0;
    
    
    switch (nes_cpu->opcode.mode)
    {
        case ADDR_IMMEDIATE:
            value = nes_cpu->low;
            break;
        case ADDR_ZEROPAGE:
            addr = nes_cpu->low & 0xFF;
            value = nes_cpu->mem[addr];
            break;
        case ADDR_ZEROPAGE_X:
            addr = (nes_cpu->low + nes_cpu->X) & 0xFF;
            value = nes_cpu->mem[addr];
            break;
        case ADDR_ZEROPAGE_Y:
            addr = (nes_cpu->low + nes_cpu->Y) & 0xFF;
            value = nes_cpu->mem[addr];
            break;
        case ADDR_ABSOLUTE:
            printf("low: %02X, high: %02X\n", nes_cpu->low, nes_cpu->high);
            addr = make_address(nes_cpu->low, nes_cpu->high);
            value = cpu_read_mem(nes_ppu, addr, nes_cpu->mem);
            break;
        case ADDR_ABSOLUTE_X:
            printf("low: %02X, high: %02X\n", nes_cpu->low, nes_cpu->high);
            addr = make_address(nes_cpu->low, nes_cpu->high);
            value = cpu_read_mem(nes_ppu, (addr + nes_cpu->X), nes_cpu->mem);
            break;
        case ADDR_ABSOLUTE_Y:
            printf("low: %02X, high: %02X\n", nes_cpu->low, nes_cpu->high);
            addr = make_address(nes_cpu->low, nes_cpu->high);
            value = cpu_read_mem(nes_ppu, (addr + nes_cpu->Y), nes_cpu->mem);
            break;
        case ADDR_INDIRECT_X:
            lsb = nes_cpu->mem[nes_cpu->low];
            msb = nes_cpu->mem[(uint8_t)(nes_cpu->low + 1)];
            addr = make_address(lsb, msb) + nes_cpu->X;
            value = nes_cpu->mem[addr];
            break;
        case ADDR_INDIRECT_Y:
            lsb = nes_cpu->mem[nes_cpu->low];
            msb = nes_cpu->mem[(uint8_t)(nes_cpu->low + 1)];
            addr = make_address(lsb, msb) + nes_cpu->Y;
            value = nes_cpu->mem[addr];
            break;
        case ADDR_RELATIVE:
            value = nes_cpu->low;
            break;
        case ADDR_NONE:
            printf("No address mode\n");
            break;
        default:
            printf("None.");
    }
    handle_command(nes_ppu, nes_cpu, value, addr);
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
