#ifndef _MEMLAYOUT_H_
#define _MEMLAYOUT_H_

/*
 o     4G ----------->  +----------------------------+------------0x100000000
 o                      |       ...                  |  kseg3
 o                      +----------------------------+------------0xe000 0000
 o                      |       ...                  |  kseg2
 o      IOBASE   -----> +----------------------------+------------0xc000 0000 (Physics Memory Max)
 o                      |       ...                  |  kseg1
 o                      +----------------------------+------------0xa000 0000
 o                      |       ...                  |  kseg0
 o  VPT,KSTACKTOP-----> +----------------------------+----|-------0x8040 0000-------end
 o                      |       Kernel Stack         |    | KSTKSIZE            /|\
 o                      +----------------------------+----|------                |
 o                      |       Kernel Text          |    |                  4 * PDMAP
 o      KERNBASE -----> +----------------------------+----|-------0x8001 0000    | 
 o                      |   Interrupts & Exception   |   \|/                    \|/
 o      ULIM     -----> +----------------------------+------------0x8000 0000-------    
 o                      |         User VPT           |     PDMAP                /|\ 
 o      UVPT     -----> +----------------------------+------------0x7fc0 0000    |
 o                      |         PAGES              |     PDMAP                 |
 o      UPAGES   -----> +----------------------------+------------0x7f80 0000    |
 o                      |         ENVS               |     PDMAP                 |
 o  UTOP,UENVS   -----> +----------------------------+------------0x7f40 0000    |
 o  UXSTACKTOP -/       |     user exception stack   |     BY2PG                 |
 o                      +----------------------------+------------0x7f3f f000    |
 o                      |       Invalid memory       |     BY2PG                 |
 o      USTACKTOP ----> +----------------------------+------------0x7f3f e000    |
 o                      |     normal user stack      |     BY2PG                 |
 o                      +----------------------------+------------0x7f3f d000    |
 a                      |                            |                           |
 a                      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                           |
 a                      .                            .                           |
 a                      .                            .                         kuseg
 a                      .                            .                           |
 a                      |~~~~~~~~~~~~~~~~~~~~~~~~~~~~|                           |
 a                      |                            |                           |
 o       UTEXT   -----> +----------------------------+                           |
 o                      |                            |     2 * PDMAP            \|/
 a     0 ------------>  +----------------------------+ -----------------------------
 o
*/

#define BY2PG       4096        // bytes to a page
#define PGSHIFT     12
#define PDSHIFT     20
#define PDMAP       (1024 * 1024)   // bytes mapped by a page directory entry

#define DRAMBASE 0x40000000
#define DRAMSIZE 0x40000000

#define KERNBASE 0x80010000
#define IOVABASE 0xC0000000

#define IOPABASE 0x01C00000
#define IOPATOP 0x01D00000

#define ULIM 0x80000000
 
#define VPT (ULIM + 4 * PDMAP)
#define KSTACKTOP (VPT - 0x100)
#define KSTKSIZE (8 * BY2PG)

#define UVPT (ULIM - PDMAP)
#define UPAGES (UVPT - PDMAP)
#define UENVS (UPAGES - PDMAP)

#define UTOP UENVS
#define UXSTACKTOP (0x82000000)

#define USTACKTOP (UTOP - 2 * BY2PG)
#define UTEXT 0x00400000

#endif // _MEMLAYOUT_H_
