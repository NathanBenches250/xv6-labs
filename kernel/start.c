#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "defs.h"

void main();
void timerinit();

// entry.S needs one stack per CPU.
__attribute__ ((aligned (16))) char stack0[4096 * NCPU]; // make sure this array starts at an address that is a multiple of 16 bytes. 
// since char is 1 byte, we are setting up a stack of size 4096 bytes each for a each CPU. 
// creating a stack for each CPU to execute C code. 

// entry.S jumps here in machine mode on stack0.
void
start()
{
  // set M Previous Privilege mode to Supervisor, for mret.
  unsigned long x = r_mstatus(); 
  x &= ~MSTATUS_MPP_MASK; // mask out garbage
  x |= MSTATUS_MPP_S; // set previous privilege to S
  w_mstatus(x); // write this to the CSRR 
  // NOTE: csrr registers are mapped directly to physical registers, but GP registers use register renaming (tomasulo like)
  // set M Exception Program Counter to main, for mret.
  // requires gcc -mcmodel=medany
  w_mepc((uint64)main);
  // when we have an exception, we store the instruction that caused it's address

  // disable paging for now.
  w_satp(0);
  // all PA->PA mapping

  // delegate all interrupts and exceptions to supervisor mode.
  w_medeleg(0xffff);
  w_mideleg(0xffff);
  w_sie(r_sie() | SIE_SEIE | SIE_STIE);
  // why? because CPU sends all these to M mode, and not S, so we are transferring this over.

  // configure Physical Memory Protection to give supervisor mode
  // access to all of physical memory.
  w_pmpaddr0(0x3fffffffffffffull); // maximum address for a 56-bit physical address.
  w_pmpcfg0(0xf);

  /*PMP is physical memory protection. memory is divided into zones normally, however in XV6, we set the entire physical
  address space as zone o (line 42). We then set the config to anyone can R/W/X in this physical zone.
  nornally OS splits it into zone 0 - kernel R only instructions zone 1 - kernel R/W data zone 2 - machine only cryptographic data etc.*/

  // ask for clock interrupts.
  timerinit();

  // keep each CPU's hartid in its tp register, for cpuid().
  int id = r_mhartid();
  w_tp(id);

  // switch to supervisor mode and jump to main().
  asm volatile("mret");
}

// ask each hart to generate timer interrupts.
void
timerinit()
{
  // enable supervisor-mode timer interrupts.
  w_mie(r_mie() | MIE_STIE);
  
  // enable the sstc extension (i.e. stimecmp).
  w_menvcfg(r_menvcfg() | (1L << 63)); 
  
  // allow supervisor to use stimecmp and time.
  w_mcounteren(r_mcounteren() | 2);
  
  // ask for the very first timer interrupt.
  w_stimecmp(r_time() + 1000000);
}
