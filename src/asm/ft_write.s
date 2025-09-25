extern __errno_location
global ft_write

	section .text

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
	mov r10, __errno_location
	neg rax
	mov rdi, rax

	call r10
	mov dword [rax], edi
	mov rax, -1
	ret