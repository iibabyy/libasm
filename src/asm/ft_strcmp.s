    section .text
	global ft_strcmp

ft_strcmp:
    mov rax, 0
.loop:

	; str2[i] != '\0'
	; mov cl, [rsi + rax]
    cmp BYTE [rdi + rax], 0
    je .break

	; str1[i] != '\0'
	; mov cl, [rsi + rax]
    cmp BYTE [rsi + rax], 0
    je .break

	mov bl, [rsi + rax]
	mov [rdi + rax], bl
    inc rax
    jmp .loop
.break:
	mov rdi, [rdi + rax]
	mov rax, [rsi + rax]
	sub rax, rdi

    ret