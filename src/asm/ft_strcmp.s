global ft_strcmp
    section .text

ft_strcmp:
    mov rax, 0
    mov r8d, 0
    mov r9d, 0
.loop:
	mov r8b, [rdi + rax] ; str1[i]
	mov r9b, [rsi + rax] ; str2[i]

	; str1[i] == '\0' -> break
    cmp r8b, 0
    je .break

	; str1[i] != str2[i] -> break
    cmp r8b, r9b
    jne .break

    inc rax
    jmp .loop
.break:
	sub r8d, r9d

	xor rax, rax
	mov eax, r8d

    ret