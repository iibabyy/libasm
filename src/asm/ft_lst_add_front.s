struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

section .text
	global ft_lst_add_front

;void ft_lstadd_front(t_list **lst, t_list *new);
ft_lst_add_front:
	; rdi = t_list **lst
	; rsi = t_list *new
	cmp rdi, 0
	je ft_lst_add_front.end

	cmp rsi, 0
	je ft_lst_add_front.end

	mov rax, [rdi]
	mov qword [rsi + t_list.next], rax
	mov qword [rdi], rsi
ft_lst_add_front.end:
	ret
