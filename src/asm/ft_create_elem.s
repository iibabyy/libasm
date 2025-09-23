extern malloc

struc t_list
	.content:	resb 8
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
	je ft_create_elem.end

	; mov r8, rdi
	mov qword [rax + t_list.content], rdi
	mov qword [rax + t_list.next], 0
ft_create_elem.end:
	ret
