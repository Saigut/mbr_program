[bits 16]
[section .text]

section .text
global main_func

main_func:
mov ah, 0
mov al, 0x13
int 0x10

mov bx, 0xa000
mov ds, bx

mov bx, 0
mov cx, 64000
begin:
mov [ds:bx], bh
inc bx
loop begin

end:
hlt
jmp end