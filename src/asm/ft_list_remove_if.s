struc t_list
	.data:	resb 8
	.next:		resb 8
endstruc

;t_list *ft_list_remove(t_list **begin, t_list *to_remove)
extern ft_list_remove

%macro allocate_stack_frame 1
    ; %1 : number of bytes needed

	push rbp          ; Save the old base pointer (RBP)
	mov rbp, rsp      ; Set RBP to the current stack pointer
	push r12
	mov r12, %1
	add r12, 16; 					arrondi à 16
	and r12, 0xFFFFFFFFFFFFFFF0;	arrondi à 16
	sub rsp, r12
	pop r12
%endmacro

%macro free_stack_frame 0
	mov rsp, rbp      ; Restore stack pointer to base pointer
	pop rbp           ; Restore caller’s base pointer
%endmacro

%macro jmp_if_null 2
	cmp %1, 0      ; Restore stack pointer to base pointer
	je %2           ; Restore caller’s base pointer
%endmacro

section .text
	global ft_list_remove_if

;void ft_list_remove_if(
;	t_list **begin,
;	void *to_compare_to,
;	int (*cmp)(),
;	void (*free_fct)(void *)
;);
ft_list_remove_if:
	; rdi = t_list **begin
	; rsi = void *to_compare_to
	; rdx = int (*cmp)()
	; rcx = void (*free_fct)(void *)

	jmp_if_null rdi, ft_list_remove_if.error
	jmp_if_null qword [rdi], ft_list_remove_if.error
	jmp_if_null rsi, ft_list_remove_if.error
	jmp_if_null rdx, ft_list_remove_if.error

	mov r8, 0
	add r8, 8; [rbp -8] = t_list **begin
	add r8, 8; [rbp -16] = int (*cmp)()
	add r8, 8; [rbp -24] = void (*free_fct)(void *)
	add r8, 8; [rbp -32] = void *to_compare_to
	add r8, 8; [rbp -40] = t_list *current_node
	allocate_stack_frame r8

	mov qword [rbp -8], rdi;	begin
	mov qword [rbp -16], rdx;	cmp()
	mov qword [rbp -24], rcx;	free_ctf()
	mov qword [rbp -32], rsi;	to_compare_to

	mov r8, qword [rbp - 8]; begin
	mov r8, qword [r8]; *begin
	mov qword [rbp -40], r8; current_node
ft_list_remove_if.loop:
	jmp_if_null qword [rbp -40], ft_list_remove_if.end

	mov rdi, qword [rbp -32]
	mov rsi, qword [rbp -40]
	mov rsi, qword [rsi + t_list.data]
	mov r8, qword [rbp -16]
	call r8

	cmp eax, 0

	mov r8, qword [rbp -40]
	mov r9, [r8 + t_list.next]
	mov qword [rbp -40], r9

	je ft_list_remove_if.loop.delete

	jmp ft_list_remove_if.loop

ft_list_remove_if.loop.delete:

	mov rdi, qword [rbp -8]
	mov rsi, r8
	call ft_list_remove

	jmp_if_null qword [rbp -24], ft_list_remove_if.loop

	mov rdi, qword [rax + t_list.data]
	call qword [rbp -24]

	jmp ft_list_remove_if.loop

ft_list_remove_if.end:
	free_stack_frame
ft_list_remove_if.error:
	ret