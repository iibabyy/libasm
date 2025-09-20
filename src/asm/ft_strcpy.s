    section .text
	global ft_strcpy

ft_strcpy:
    xor rax, rax
.loop:
	mov bl, [rsi + rax]
	mov [rdi + rax], bl
    inc rax
    cmp bl, 0
    jne .loop

	mov rax, rdi 
    ret