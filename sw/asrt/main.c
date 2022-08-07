#include <stdio.h>

#include "irq.h"
#include "stdint.h"

// DMA vars
static volatile char* const TERMINAL_ADDR = (char* const)0x20000000;

static volatile uint32_t* const DMA_SRC_ADDR = (uint32_t* const)0x70000000;
static volatile uint32_t* const DMA_DST_ADDR = (uint32_t* const)0x70000004;
static volatile uint32_t* const DMA_LEN_ADDR = (uint32_t* const)0x70000008;
static volatile uint32_t* const DMA_OP_ADDR = (uint32_t* const)0x7000000C;
static volatile uint32_t* const DMA_STAT_ADDR = (uint32_t* const)0x70000010;

static const uint32_t DMA_OP_NOP = 0;
static const uint32_t DMA_OP_MEMCPY = 1;

_Bool dma_completed = 0;

void dma_irq_handler() {
	dma_completed = 1;
}

void init() {
	register_interrupt_handler(4, dma_irq_handler);
}

// Sensor vars
static volatile char* const SENSOR_INPUT_ADDR = (char* const)0x50000000;
static volatile uint32_t* const SENSOR_SCALER_REG_ADDR = (uint32_t* const)0x50000080;
static volatile uint32_t* const SENSOR_FILTER_REG_ADDR = (uint32_t* const)0x50000084;

void sensor_irq_handler() {
	*TERMINAL_ADDR = 'S';
	*TERMINAL_ADDR = '1';
	*TERMINAL_ADDR = ':';
	*TERMINAL_ADDR = ' ';
	for (int i = 0; i < 64; ++i) {
		*TERMINAL_ADDR = *(SENSOR_INPUT_ADDR + i) % 92 + 32;
	}
	*TERMINAL_ADDR = '\n';
}

// Sensor2 vars
static volatile char* const SENSOR2_INPUT_ADDR = (char* const)0x50002000;
static volatile uint32_t* const SENSOR2_SCALER_REG_ADDR = (uint32_t* const)0x50002080;
static volatile uint32_t* const SENSOR2_FILTER_REG_ADDR = (uint32_t* const)0x50002084;

void sensor2_irq_handler() {
	*TERMINAL_ADDR = 'S';
	*TERMINAL_ADDR = '2';
	*TERMINAL_ADDR = ':';
	*TERMINAL_ADDR = ' ';
	for (int i = 0; i < 64; ++i) {
		*TERMINAL_ADDR = *(SENSOR2_INPUT_ADDR + i) % 92 + 32;
	}
	*TERMINAL_ADDR = '\n';
}

// Main program
int main() {
	// Program sensor 1
	register_interrupt_handler(2, sensor_irq_handler);
	*SENSOR_SCALER_REG_ADDR = 5;
	*SENSOR_FILTER_REG_ADDR = 2;

	// Program sensor 2
	register_interrupt_handler(5, sensor2_irq_handler);
	*SENSOR2_SCALER_REG_ADDR = 5;
	*SENSOR2_FILTER_REG_ADDR = 2;

	// DMA execution
	for (int i = 0; i < ITER; ++i) {
		uint8_t src[32] = {[0 ... 31] = 65 + (i%26)};
		uint8_t dst[32] = {0};

		dma_completed = 0;
		*DMA_SRC_ADDR = (uint32_t)(&src[0]);
		*DMA_DST_ADDR = (uint32_t)(&dst[0]);
		*DMA_LEN_ADDR = 32;
		*DMA_OP_ADDR = DMA_OP_MEMCPY;

		while (!dma_completed) {
			asm volatile("wfi");
		}

		*TERMINAL_ADDR = 'D';
		*TERMINAL_ADDR = '0';
		*TERMINAL_ADDR = ':';
		*TERMINAL_ADDR = ' ';
		for (int i = 0; i < 32; ++i) {
			*TERMINAL_ADDR = dst[i];
		}
		*TERMINAL_ADDR = '\n';

		for (int i = 0; i < 5; ++i) {
			asm volatile("wfi");
		}
	};

	return 0;
}
