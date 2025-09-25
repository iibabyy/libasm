extern __errno_location
extern _malloc
extern ft_strlen
extern ft_strcpy

	section .text
	global ft_stdup

; rdi: char *str
ft_stdup:
	call ft_strlen
	push rdi

	inc rax
	mov rdi, rax
	call _malloc

	cmp rax, 0
	je .error

	mov rdi, rax
	pop rsi
	call ft_strcpy
	ret

.error:
	mov r10, __errno_location
	call r10
	mov dword [rax], 12; ENOMEM (12)
	mov rax,0
	ret