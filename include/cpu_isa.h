#ifndef VOID16_CPU_ISA_H
#define VOID16_CPU_ISA_H

#include <cpu.h>

#define INT_DIV_0 0

void isa_ld     (cpu_t* p, byte_t r, word_t d);
void isa_ldr    (cpu_t* p, byte_t l, byte_t r);
void isa_lde    (cpu_t* p, byte_t l, byte_t r);
void isa_ldm    (cpu_t* p, byte_t r, word_t d);
void isa_st     (cpu_t* p, word_t d, byte_t r);

void isa_add    (cpu_t* p, byte_t l, byte_t r);
void isa_sub    (cpu_t* p, byte_t l, byte_t r);
void isa_mul    (cpu_t* p, byte_t l, byte_t r);
void isa_imul   (cpu_t* p, byte_t l, byte_t r);
void isa_div    (cpu_t* p, byte_t l, byte_t r);
void isa_idiv   (cpu_t* p, byte_t l, byte_t r);

void isa_and    (cpu_t* p, byte_t l, byte_t r);
void isa_or     (cpu_t* p, byte_t l, byte_t r);
void isa_xor    (cpu_t* p, byte_t l, byte_t r);

void isa_shl    (cpu_t* p, byte_t l, byte_t r);
void isa_shr    (cpu_t* p, byte_t l, byte_t r);
void isa_sar    (cpu_t* p, byte_t l, byte_t r);

void isa_jz     (cpu_t* p, word_t d);
void isa_jnz    (cpu_t* p, word_t d);
void isa_js     (cpu_t* p, word_t d);
void isa_jns    (cpu_t* p, word_t d);
void isa_ja     (cpu_t* p, word_t d);
void isa_ja     (cpu_t* p, word_t d);

void isa_int    (cpu_t* p, word_t d);
void isa_cli    (cpu_t* p);
void isa_idl    (cpu_t* p);

#endif
