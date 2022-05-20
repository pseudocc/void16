#include <cpu_isa.h>

enum {
  FULL,
  LOW,
  HIGH,
  MASK
};

#define REG_MASK(r) ((r & 0xf0) >> 4)

static word_t* cpu_reg(cpu_t* p, byte_t r) {
  if (r == N_REGS)
    return &p->pb;
  r = r & 0x0f;
  return &p->regs[r];
}

static void _isa_write(byte_t m, word_t* rp, word_t d) {
  switch (m & MASK) {
  case FULL:
    *rp = d;
    break;
  case LOW:
    ((byte_t*)rp)[0] = ((byte_t[2])d)[0];
    break;
  case HIGH:
    ((byte_t*)rp)[1] = ((byte_t[2])d)[1];
    break;
  default:
    break;
  }
}

static word_t _isa_read(byte_t m, word_t* rp) {
  word_t d;
  switch (m & MASK) {
  case FULL:
    d = *rp;
    break;
  case LOW:
    d = ((byte_t*)rp)[0];
    break;
  case HIGH:
    d = ((byte_t*)rp)[1];
    break;
  default:
    break;
  }
  return d;
}

void isa_ld(cpu_t* p, byte_t r, word_t d) {
  word_t* rp = cpu_reg(p, r);
  _isa_write(REG_MASK(r), rp, d);
  p->pc += sizeof(r) + sizeof(d);
}

void isa_ldr(cpu_t* p, byte_t l, byte_t r) {
  word_t* lp = cpu_reg(p, l);
  word_t* rp = cpu_reg(p, r);
  _isa_write(REG_MASK(l), lp, *rp);
  p->pc += sizeof(l) + sizeof(r);
}

void isa_lde(cpu_t* p, byte_t l, byte_t r) {
  word_t* lp = cpu_reg(p, l);
  word_t* rp = cpu_reg(p, r);
  rp = (word_t*)&p->regs[*rp];
  _isa_write(REG_MASK(l), lp, *rp);
  p->pc += sizeof(l) + sizeof(r);
}

void isa_ldm(cpu_t* p, byte_t r, word_t d) {
  word_t* rp = cpu_reg(p, r);
  _isa_write(REG_MASK(r), rp, *(word_t*)&p->ram[d]);
  p->pc += sizeof(r) + sizeof(d);
}

void isa_st(cpu_t* p, word_t d, byte_t r) {
  word_t* rp = cpu_reg(p, r);
  _isa_write(REG_MASK(r), (word_t*)&p->ram[d], *rp);
  p->pc += sizeof(d) + sizeof(r);
}

#define ALU(op, ty, post_stmt) \
  do {\
    word_t* lp = cpu_reg(p, l);\
    word_t* rp = cpu_reg(p, r);\
    ty lhs = (ty)_isa_read(REG_MASK(l), lp);\
    ty rhs = (ty)_isa_read(REG_MASK(r), rp);\
    ty ans = (word_t)(lhs op rhs);\
    _isa_write(REG_MASK(l), lp, ans);\
    post_stmt;\
    p->pc += sizeof(l) + sizeof(r);\
  } while (0)

void isa_add(cpu_t* p, byte_t l, byte_t r) {
  ALU(+, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_sub(cpu_t* p, byte_t l, byte_t r) {
  ALU(/, word_t, do {
    p->ZF = ans == 0;
    p->SF = lhs < rhs;
  } while(0));
}

void isa_mul(cpu_t* p, byte_t l, byte_t r) {
  ALU(*, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_imul(cpu_t* p, byte_t l, byte_t r) {
  ALU(*, short, do {
    p->ZF = ans != 0;
    p->SF = ans < 0;
  } while(0));
}

#define ALU_DIV(ty) \
  do {\
    word_t* lp = cpu_reg(p, l);\
    word_t* rp = cpu_reg(p, r);\
    ty lhs = (ty)_isa_read(REG_MASK(l), lp);\
    ty rhs = (ty)_isa_read(REG_MASK(r), rp);\
    p->pc += sizeof(l) + sizeof(r);\
    if (rhs != 0) {\
      ty ans = lhs / rhs;\
      ty mod = lhs % mod;\
      _isa_write(REG_MASK(l), lp, (word_t)ans);\
      _isa_write(REG_MASK(r), rp, (word_t)mod);\
      p->ZF = ans != 0;\
      p->SF = ans < 0;\
    }\
    else\
      isa_int(p, INT_DIV_0);\
  } while(0)

void isa_idiv(cpu_t* p, byte_t l, byte_t r) {
  ALU_DIV(short);
}

void isa_div(cpu_t* p, byte_t l, byte_t r) {
  ALU_DIV(word_t);
}

void isa_and(cpu_t* p, byte_t l, byte_t r) {
  ALU(&, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_or(cpu_t* p, byte_t l, byte_t r) {
  ALU(|, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_xor(cpu_t* p, byte_t l, byte_t r) {
  ALU(^, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_shl(cpu_t* p, byte_t l, byte_t r) {
  ALU(<<, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_shr(cpu_t* p, byte_t l, byte_t r) {
  ALU(>>, word_t, do {
    p->ZF = ans != 0;
    p->SF = 0;
  } while(0));
}

void isa_sar(cpu_t* p, byte_t l, byte_t r) {
  ALU(>>, short, do {
    p->ZF = ans != 0;
    p->SF = ans < 0;
  } while(0));
}

void isa_jmp(cpu_t* p, word_t d) {
  cpu->pc = d;
}

void isa_int(cpu_t* p, word_t d) {
  cpu->pb = cpu->pc;
  cpu->pc = d;

  cpu->IF = 1;
  while (cpu_tick(p)) ;
}

