    section .text
	global ft_strlen

ft_strlen:
    mov rax, 0
.loop:
    cmp BYTE [rdi + rax], 0
    je .break
    inc rax
    jmp .loop
.break:
    ret