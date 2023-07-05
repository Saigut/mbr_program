# 使用 C 语言编写 mbr 程序
Write an MBR program using the C programming language

* 程序主体使用 C 语言编写
* 对于 bios 接口用内联汇编调用
* 为了满足对 mgr 程序的布局要求，需要用到一个链接脚本

## 环境
* x86 cpu

## 编译
```
make
```
手动命令：
```
gcc -m16 -O0 -ffreestanding -Wall -Wextra -fno-pic -c mbr_program.c -o build/mbr_program.o
ld -m elf_i386 -T link_script.ld mbr_program.o -o build/mbr_program.elf
objcopy -O binary mbr_program.elf build/mbr_program.bin
```

## 虚拟机加载程序
```
qemu-system-i386 -hda build/mbr_program.bin
```

## 调试工具
```
objdump -d xxx
objdump -D -b binary -mi386 -Maddr16,data16 ./build/mbr_program.bin
ndisasm -b16 -o7c00h ./build/mbr_program.bin
hexdump xxx
xxd -p xxx
file xxx
```
