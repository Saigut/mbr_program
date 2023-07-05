void print_str(const char* str);

__attribute__((section(".text"))) void main_func() {
    const char* message = "Hello, World!\n";
    print_str(message);
    while(1) {
        asm volatile ("hlt");
    }
}

__attribute__((section(".text"))) void print_str(const char* str) {
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
