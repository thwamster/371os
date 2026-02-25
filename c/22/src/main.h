#pragma once

#include <assert.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#define SIZE 0x80

int main();
void init();
size_t my_malloc(size_t s);
void set(int32_t n, size_t s);
int32_t get(size_t s);
void print();

static uint8_t BUS[SIZE];