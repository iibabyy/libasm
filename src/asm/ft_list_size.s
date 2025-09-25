struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

section .text
	global ft_list_size

;int ft_list_size(t_list *begin_list)
ft_list_size:
	; rdi = t_list *begin

	mov eax, 0

	cmp rdi, 0
	je ft_list_size.end
ft_list_size.loop:
	inc eax

	cmp qword [rdi + t_list.next], 0
	je ft_list_size.end

	mov rdi, qword [rdi + t_list.next]

	jmp ft_list_size.loop
ft_list_size.end:
	ret
