extern __errno_location

	section .text
	global ft_write

ft_write:
	; arg[0] == rdi == int fd 
	; arg[1] == rsi == void *buf
	; arg[2] == rdx == size_t count
	
	; check if fd valid ?


	; write rdi, rsi, rdx
	mov rax, 1        ; syscall number for write in 64-bit
	syscall

	cmp rax, 0
	jl .set_error

    ret

.set_error:
	neg rax
	mov rdi, rax

	call __errno_location
	mov [rax], edi
	mov rax, -1
	ret