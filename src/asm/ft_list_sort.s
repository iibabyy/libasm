struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

;t_list *ft_list_remove(t_list **begin, t_list *to_remove)
extern ft_list_remove

;void ft_lstadd_front(t_list **lst, t_list *new);
extern ft_lst_add_front



%macro allocate_stack_frame 1
    ; %1 : number of bytes needed

	push rbp          ; Save the old base pointer (RBP)
	mov rbp, rsp      ; Set RBP to the current stack pointer
	push r12
	mov r12, %1
	add r12, 16
	and r12, 0xFFFFFFFFFFFFFFF0
	sub rsp, r12   ; arrondi à 16
	pop r12
%endmacro

%macro free_stack_frame 0
	mov rsp, rbp      ; Restore stack pointer to base pointer
	pop rbp           ; Restore caller’s base pointer
%endmacro

section .text
	global remove_lowest
	global ft_list_sort

;void ft_list_sort(t_list **begin_list, int (*cmp)());
ft_list_sort:
	; rdi = t_list **begin
	; rsi = int (*cmp)()

	cmp rdi, 0
	je ft_list_sort.end

	cmp qword [rdi], 0
	je ft_list_sort.end

	cmp rsi, 0
	je ft_list_sort.end

	mov r8, qword [rdi]
	cmp qword [r8 + t_list.next], 0
	je ft_list_sort.end

ft_list_sort.loop:

	push rdi
	push rsi
	call remove_lowest
	mov rsi, rax
	call ft_lst_add_front
	pop rsi
	pop rdi

	mov r8, qword [rdi]
	cmp qword [r8 + t_list.next], 0
	je ft_list_sort.end

	
	lea rdi, [r8 + t_list.next]
	jmp ft_list_sort.loop
ft_list_sort.end:
	ret



;t_list *remove_lowest(t_list **begin, int (*cmp)());
remove_lowest:
	; rdi = t_list **begin
	; rsi = int (*cmp)()
	cmp rdi, 0
	je remove_lowest.error

	cmp qword [rdi], 0
	je remove_lowest.error

	mov r8, 0
	add r8, 8; t_list **begin
	add r8, 8; int (*cmp)()
	add r8, 8; t_list *current_lowest
	add r8, 8; t_list *current_node
	allocate_stack_frame r8

	mov qword [rbp -8], rdi;		[rbp -8] = t_list **begin
	mov qword [rbp -16], rsi;		[rbp -16] = int (*cmp)()
	mov r8, qword [rdi]; current_lowest
	mov qword [rbp -24], r8;		[rbp -24] = t_list *current_lowest

	mov r8, qword [rbp -24]; current_lowest
	mov qword [rbp -32], r8;	[rbp -32] (current_node) = current_lowest (we skip to the next node at the beginning of the loop)

remove_lowest.find_lowest_loop:
	mov r8, qword [rbp -32]; r8 = current_node
	mov r11, qword [r8 + t_list.next]; r11 = current_node->next
	mov qword [rbp -32], r11; current_node = r11

	cmp qword [rbp -32], 0; current_node == 0 ?
	je remove_lowest.end

	mov rdi, qword [rbp -24]; current_lowest
	mov rdi, [rdi + t_list.data]
	mov rsi, qword [rbp -32]; current_node
	mov rsi, [rsi + t_list.data]
	call qword [rbp -16]; cmp()

	cmp eax, 0; current_lowest < current_node ?
	jle remove_lowest.find_lowest_loop

	mov r8, qword [rbp -32]; r8 = current_node
	mov qword [rbp -24], r8; current_lowest = current_node
	jmp remove_lowest.find_lowest_loop
remove_lowest.end:
	mov rdi, qword [rbp -8]; rdi = t_list **begin
	mov rsi, qword [rbp -24]; rsi = t_list *current_lowest

	call ft_list_remove
	free_stack_frame
	ret
remove_lowest.error:
	mov rax, 0
	ret