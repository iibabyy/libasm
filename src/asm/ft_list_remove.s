struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

section .text
	global ft_list_remove

;t_list *ft_list_remove(t_list **begin, t_list *to_remove)
ft_list_remove:
	; rdi = t_list **begin
	; rsi = t_list *to_remove

	xor rax, rax
	mov r8, qword [rdi]

	cmp r8, 0
	je ft_list_remove.end

	cmp r8, rsi
	je ft_list_remove.found_first

	mov rax, qword [r8 + t_list.next]
ft_list_remove.loop:

	cmp rax, 0
	je ft_list_remove.end

	cmp rax, rsi
	je ft_list_remove.found

	mov r8, rax
	mov rax, qword [rax + t_list.next]

	jmp ft_list_remove.loop
ft_list_remove.found_first:
	mov r9, qword [r8 + t_list.next]
	mov qword [rdi], r9
	mov rax, r8
	jmp ft_list_remove.end
ft_list_remove.found:
	mov r9, qword [rax + t_list.next]
	mov qword [r8 + t_list.next], r9
ft_list_remove.end:
	ret
