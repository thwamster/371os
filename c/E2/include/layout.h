#pragma once

#include <stdint.h>

static const uint64_t KERNEL_START = 0x80200000;
static const uint64_t RAM_SIZE = 0x8000000;
static const uint64_t KERNEL_END = KERNEL_START + RAM_SIZE;

static volatile uint32_t * const SIFIVE_EXIT = (uint32_t *) 0x00100000;
static volatile uint32_t * const CLINT_BASE = (uint32_t *) 0x02000000;
static volatile uint32_t * const CLINT_MSIP = (uint32_t *) 0x02000000;
static volatile uint64_t * const MTIMECMP = (uint64_t *) 0x02004000;
static volatile uint64_t * const MTIME = (uint64_t *) 0x0200BFF8;
static volatile uint32_t * const PLIC_PRIORITY = (uint32_t *) 0x0C000000;
static volatile uint32_t * const PLIC_ENABLE = (uint32_t *) 0x0C002080;
static volatile uint32_t * const PLIC_THRESHOLD = (uint32_t *) 0xC201000;
static volatile uint32_t * const PLIC_CLAIM = (uint32_t *) 0xC201004;
static volatile uint8_t * const UART = (uint8_t *) 0x10000000;
static volatile uint8_t * const UART_IER = (uint8_t *) 0x10000001;
static volatile uint8_t * const UART_FCR = (uint8_t *) 0x10000002;
static volatile uint8_t * const UART_IIR = (uint8_t *) 0x10000002;
static volatile uint8_t * const UART_LCR = (uint8_t *) 0x10000003;
static volatile uint8_t * const UART_MCR = (uint8_t *) 0x10000004;
static volatile uint8_t * const UART_LSR = (uint8_t *) 0x10000005;
static volatile uint32_t * const VIRTIO_START = (uint32_t *) 0x10001000;
static volatile uint32_t * const VIRTIO_END = (uint32_t *) 0x10008000;
static volatile uint64_t * const PCIE_ECAM = (uint64_t *) 0x30000000;
static volatile uint32_t * const PCIE_MMIO = (uint32_t *) 0x40000000;