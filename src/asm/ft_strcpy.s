    section .text
global ft_strcpy

ft_strcpy:
    mov rax, 0
.loop:
    cmp BYTE [rsi + rax], 0
    je .break
	mov bl, [rsi + rax]
	mov [rdi + rax], bl
    inc rax
    jmp .loop
.break:
	mov bl, [rsi + rax]
	mov [rdi + rax], bl

	mov rax, rdi
    ret