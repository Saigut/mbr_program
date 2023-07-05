#ifndef _CODE16GCC_H_
#define _CODE16GCC_H_
__asm__(".code16gcc\n");
#endif

#include <stdint.h>

void print_str(const char* str);
void print_block();
void sleep_one_second();

__attribute__((section(".text"))) void main_func()
{
    const char* message = "Hello, World!\n";
    print_str(message);
    sleep_one_second();
    sleep_one_second();
    print_block();
    while(1) {
        asm volatile ("hlt");
    }
}

void print_str(const char* str)
{
    while (*str) {
        asm volatile (
                "movb %0, %%al\n"
                "movb $0x0E, %%ah\n"
                "movb $0x00, %%bh\n"
                "movb $0x07, %%bl\n"
                "int $0x10\n"
                :
                : "r" (*str)
                : "%ah", "%bh", "%bl"
                );
        str++;
    }
}

void print_block()
{
    uint8_t color;
    uint16_t i = 0;

    // 配置 vga
    asm volatile (
            // 配置 vga 中断
            "mov $0, %%ah\n"
            "mov $0x13, %%al\n"
            "int $0x10\n"

            // 设置 vga 地址
            "mov $0xa000, %%bx\n"
            "mov %%bx, %%ds\n"
            :
            :
            : "%ax", "%bx"
            );

    // 着色
    while (i < 64000) {
        color = i >> 8;
        asm volatile (
                // 写入颜色
                "mov %0, %%al\n"
                "mov %1, %%bx\n"
                "mov %%al, %%ds:(%%bx)\n"
                :
                : "r" (color), "r" (i)
                : "%ax", "%bx"
                );
        i++;
    }
}

// 粗略的1秒，不精确
void sleep_one_second() {
    asm volatile (
            "movb $0x86, %%ah;"   /* BIOS Wait */
            "xorw %%bx, %%bx;"    /* Clear BX */
            "movw $0x09, %%cx;"   /* High word of delay */
            "movw $0x27c0, %%dx;" /* Low word of delay (cx:dx = 0x0F * 2^16 + 0x4240 = 1,000,000 microseconds or 1 second) */
            "int $0x15;"          /* BIOS interrupt */
            :
            :
            : "%bx", "%cx", "%dx", "%ah"
            );
}
