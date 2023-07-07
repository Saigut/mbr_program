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

void print_char(char c);
void print_str(const char* str);
void print_san_lian(char *bx);
void print_color();
void sleep_1s();

SECTION(".text")
void main_func()
{
    const char* message = "Hello, World!";
    print_str(message);

    sleep_1s();
    sleep_1s();
    char san_lian[] = {
            0x01, 0x00,
            0x01, 0x00,
            0x0d, 0x04, 0x12, 0x0b, 0x12, 0x03, 0x00,
            0x0d, 0x05, 0x0f, 0x0f, 0x0f, 0x05, 0x00,
            0x0d, 0x05, 0x0e, 0x11, 0x0d, 0x07, 0x00,
            0x0b, 0x07, 0x0d, 0x05, 0x09, 0x05, 0x08, 0x0f, 0x00,
            0x05, 0x04, 0x01, 0x0e, 0x06, 0x0a, 0x01, 0x0a, 0x04, 0x15, 0x00,
            0x04, 0x05, 0x01, 0x0f, 0x05, 0x07, 0x07, 0x07, 0x04, 0x15, 0x00,
            0x04, 0x05, 0x01, 0x0f, 0x05, 0x06, 0x02, 0x02, 0x01, 0x02, 0x02, 0x06, 0x05, 0x13, 0x00,
            0x04, 0x05, 0x01, 0x0f, 0x05, 0x06, 0x01, 0x03, 0x01, 0x03, 0x01, 0x06, 0x07, 0x0f, 0x00,
            0x04, 0x05, 0x01, 0x0f, 0x05, 0x05, 0x02, 0x03, 0x01, 0x03, 0x02, 0x05, 0x08, 0x0d, 0x00,
            0x04, 0x05, 0x01, 0x0e, 0x07, 0x09, 0x01, 0x09, 0x09, 0x0d, 0x00,
            0x04, 0x05, 0x01, 0x0e, 0x08, 0x11, 0x09, 0x0f, 0x00,
            0x04, 0x05, 0x01, 0x0d, 0x0b, 0x0d, 0x0b, 0x07, 0x01, 0x07, 0x00,
            0x05, 0x04, 0x01, 0x0b, 0x10, 0x07, 0x0f, 0x03, 0x07, 0x03, 0x00,
            0x01, 0x00,
            0x01, 0x00,
            0x00
    };
    print_san_lian(san_lian);

    sleep_1s();
    sleep_1s();
    print_color();

    while(1) {
        asm volatile ("hlt");
    }
}

void print_char(char c)
{
    __asm__ __volatile__ (
            "mov $0x0e, %%ah\n"
            "mov $0x00, %%bh\n"
            "mov $0x07, %%bl\n"
            "mov %0, %%al\n"
            "int $0x10\n"
            :
            : "r" (c)
            : "%ah", "%bh", "%bl"
            );
}

void print_str(const char* str)
{
    while (*str) {
        print_char(*str);
        str++;
    }
//    asm volatile (
//            "movw %%bx, %%si\n"
//            "1:\n"
//            "lodsb\n"
//            "cmpb $0, %%al\n"
//            "je 2f\n"
//            "movb $0xe, %%ah\n"
//            "movw $0x0001, %%bx\n"
//            "int $0x10\n"
//            "jmp 1b\n"
//            "2:\n"
//            :
//            : "b" (str)
//            : "%ah", "%al"
//            );
}

void print_san_lian(char *bx)
{
    char cl;

    while (1) {
        cl = *bx;
        if (cl == 0) {
            break;
        }
        bx++;

        uint8_t is_0 = 0;
        do {
            do {
                if (is_0) {
                    print_char('0');
                } else {
                    print_char(' ');
                }
            } while (--cl);
            is_0 = !is_0;
            cl = *bx;
            bx++;
        } while (cl != 0);
        print_char('\r');  // 0x0d
        print_char('\n');  // 0x0a
    }
}

// Roughly 1 second, not precise
void sleep_1s() {
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

void print_color()
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
