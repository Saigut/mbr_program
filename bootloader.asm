[bits 16]
[extern main]
[section .text]

global _start
_start:
    cli                    ; 关闭所有中断
    xor ax, ax             ; 将 ax 寄存器清零
    mov ss, ax             ; 设置栈段寄存器
    mov sp, 0x9C00         ; 设置栈指针寄存器
    sti                    ; 重新启用中断
    call main              ; 调用 C 程序入口
hang:
    hlt                    ; 使 CPU 进入休眠状态
    jmp hang               ; 无限循环
