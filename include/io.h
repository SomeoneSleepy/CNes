#ifndef IOH
#define IOH
#include <stdint.h>

typedef struct io_reg {
    // PULSE 1 CHANNEL
    uint8_t PULSE1_VOLUME;      // $4000 - Volume/envelope/length
    uint8_t PULSE1_SWEEP;       // $4001 - Sweep unit
    uint8_t PULSE1_TIMER_LOW;   // $4002 - Timer low
    uint8_t PULSE1_TIMER_HIGH;  // $4003 - Timer high and length counter

    // PULSE 2 CHANNEL
    uint8_t PULSE2_VOLUME;      // $4004
    uint8_t PULSE2_SWEEP;       // $4005
    uint8_t PULSE2_TIMER_LOW;   // $4006
    uint8_t PULSE2_TIMER_HIGH;  // $4007

    // TRIANGLE CHANNEL
    uint8_t TRIANGLE_LINEAR;    // $4008 - Linear counter
    uint8_t TRIANGLE_TIMER_LOW; // $400A
    uint8_t TRIANGLE_TIMER_HIGH;// $400B

    // NOISE CHANNEL
    uint8_t NOISE_VOLUME;       // $400C - Envelope/volume
    uint8_t NOISE_MODE;         // $400E - Mode and period
    uint8_t NOISE_LENGTH;       // $400F - Length counter

    // DMC CHANNEL
    uint8_t DMC_CONTROL;        // $4010 - Control (IRQ enable, loop)
    uint8_t DMC_LOAD;           // $4011 - Direct load
    uint8_t DMC_ADDR;           // $4012 - Sample address
    uint8_t DMC_LENGTH;         // $4013 - Sample length

    // STATUS AND CONTROL REGISTERS
    uint8_t STATUS;             // $4015 - Channel enable and status flags
    uint8_t FRAME_COUNTER;      // $4017 - Frame counter control

    // CONTROLLER PORTS (TYPICALLY $4016 AND $4017)
    uint8_t CONTROLLER1;        // $4016
    uint8_t CONTROLLER2;        // $4017

} io_reg;

#endif