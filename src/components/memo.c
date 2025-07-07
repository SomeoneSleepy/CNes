#include "memo.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int load_data(uint8_t* memory, size_t starting_index, uint8_t* addition, size_t addition_size)
{
    int result = 0;
    size_t i = 0;
    if (starting_index + addition_size > MAX_MEM_SIZE)
    {
        printf("Overflow");
        result = 1;
    }

    for (i = 0; i < addition_size; i++) {
        memory[starting_index + i] = addition[i];
    }
    return result;
}

void memory_log(uint8_t* memory)
{
    char number [NUMBER_SIZE] = { 0 };
    int i = 0;
    FILE* log_file = fopen("mem_log.txt", "w");
    if(!log_file)
    {
        printf("Failed to create log text file.");
        return;
    }

    for(i = 0; i < CPU_MEM_SIZE; i++)
    {
        fprintf(log_file, "%d ", memory[i]);
    }

    fputc('\n', log_file);
    fclose(log_file);
}
