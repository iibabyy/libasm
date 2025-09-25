extern ft_create_elem
extern ft_lst_add_front

struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

section .text
	global ft_list_push_front

;void ft_list_push_front(t_list **begin_list, void *data);
ft_list_push_front:
	; rdi = t_list **begin_list
	; rsi = void *data
	
	cmp rdi, 0
	je ft_list_push_front.end

	push rdi
	mov rdi, rsi
	call ft_create_elem
	pop rdi

	cmp rax, 0
	je ft_list_push_front.end

	mov rsi, rax
	call ft_lst_add_front

ft_list_push_front.end:
	ret
