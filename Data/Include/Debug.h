#pragma once
#include <setjmp.h>
#define print(a) __print__((void*)(L##a))
#define print_e(a) __print__((void*)a)
#define ERROR_0 __ERROR__(0);
#define ERROR_1 __ERROR__(1);
extern jmp_buf jump_buffer_1;
void __print__(void*);
void __ERROR__(int);
