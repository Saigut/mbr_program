void init_func() {
    asm volatile (
            "cli\n"
            "xorw %%ax, %%ax\n"
            "movw %%ax, %%ds\n"
            "movw %%ax, %%ss\n"
            "movw %%ax, %%es\n"
            "call main_func\n"
            :
            :
            : "ax"
            );
}


#include <stdint.h>

#define SECTION(s) __attribute__((section(s)))

void print_str(const char* str);
void print_block();
void sleep_one_second();


SECTION(".text")
void main_func()
{
    const char* message = "Hello, World!";

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
    asm volatile (
            "movw %%bx, %%si\n"
            "1:\n"
            "lodsb\n"
            "cmpb $0, %%al\n"
            "je 2f\n"
            "movb $0xe, %%ah\n"
            "movw $0x0001, %%bx\n"
            "int $0x10\n"
            "jmp 1b\n"
            "2:\n"
            :
            : "b" (str)
            : "%ah", "%al"
            );
}

void print_block()
{
    uint8_t color;
    uint16_t i = 0;

    // setup vga
    asm volatile (
            // setup interrupt of vga
            "mov $0, %%ah\n"
            "mov $0x13, %%al\n"
            "int $0x10\n"

            // set address of vga
            "mov $0xa000, %%bx\n"
            "mov %%bx, %%ds\n"
            :
            :
            : "%ax", "%bx"
            );

    // draw
    while (i < 64000) {
        color = i >> 8;
        asm volatile (
                // draw color
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

// Roughly 1 second, not precise
void sleep_one_second() {
    asm volatile (
            "movb $0x86, %%ah;"   /* BIOS Wait */
            "xorw %%bx, %%bx;"    /* Clear BX */
            "movw $0x09, %%cx;"   /* High word of delay */
            "movw $0x27c0, %%dx;" /* Low word of delay (eg: cx:dx = 0x0F * 2^16 + 0x4240 = 1,000,000 microseconds or 1 second) */
            "int $0x15;"          /* BIOS interrupt */
            :
            :
            : "%bx", "%cx", "%dx", "%ah"
            );
}
