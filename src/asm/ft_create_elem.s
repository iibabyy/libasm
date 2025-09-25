extern __errno_location
extern malloc

struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

	section .text
	global ft_create_elem

;t_list *ft_create_elem(void *data);
ft_create_elem:
	; rdi = void *data
	mov r9, malloc
	push rdi
	mov rdi, t_list_size; sizeof(t_list)
	call r9
	pop rdi
	cmp rax, 0
	je ft_create_elem.malloc_failed

	; mov r8, rdi
	mov qword [rax + t_list.data], rdi
	mov qword [rax + t_list.next], 0
	jmp ft_create_elem.end
ft_create_elem.malloc_failed:
	mov r9, __errno_location
	call r9
	mov dword [rax], 12; ENOMEM (12)
ft_create_elem.end:
	ret
