struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

section .text
	global ft_list_find

;t_list *ft_list_find(t_list *begin, t_list *to_find)
ft_list_find:
	; rdi = t_list *begin
	; rsi = t_list *to_find

	mov rax, rdi
ft_list_find.loop:

	cmp rax, 0
	je ft_list_find.end

	cmp rax, rsi
	je ft_list_find.end

	mov rax, qword [rax + t_list.next]

	jmp ft_list_find.loop
ft_list_find.end:
	ret
