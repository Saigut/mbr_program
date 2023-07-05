__attribute__((section(".kernel"))) void print(const char* str) {
    while (*str) {
        asm volatile (
            "movb %0, %%al\n"
            "movb $0x0E, %%ah\n"
            "xor %%ebx, %%ebx\n"
            "int $0x10\n"
            :
            : "g" (*str)
            : "eax", "ebx"
        );
        str++;
    }
}

__attribute__((section(".kernel"))) void main() {
    const char* message = "Hello, World!\n";
    print(message);
    
    while(1); // Infinite loop
}
