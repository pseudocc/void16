#ifndef VOID16_CPU_H
#define VOID16_CPU_H

typedef unsigned char byte_t;
typedef unsigned short word_t;
typedef unsigned long long u64_t;

#define N_REGS 16
#define MEMSIZ 65536

typedef struct {
  u64_t clock;
  byte_t ram[MEMSIZ];
  word_t guard;

  word_t pb;
  word_t regs[N_REGS - 3];
  word_t zr;
  word_t pc;
  word_t sp;

  byte_t ZF : 1;
  byte_t SF : 1;
  byte_t IF : 1;

  byte_t cycles;
} cpu_t;

void cpu_init(cpu_t* p);
word_t cpu_tick(cpu_t* p);

#endif
