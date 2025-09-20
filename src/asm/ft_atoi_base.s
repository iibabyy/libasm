	extern ft_strlen; rdi: char *str

;---------- HELP ----------;

;	| word  | dword | qword |
;   | 2byte | 4byte | 8byte |

;---------- LIBRARY ----------;

%macro jump_if_null 2
	; %1 : value to check
    ; %2 : label where to jump if %1 is null

	cmp %1, 0
	je %2
%endmacro
;-----;


%macro jump_if_less_than 3
	; %1 : value to check
	; %2 : value with which compare %1
    ; %3 : label where to jump if %1 is less than %2

	cmp %1, %2
	jl %3
%endmacro
;-----;


%macro jump_if_negative 2
	; %1 : value to check
    ; %2 : label where to jump if %1 is negative

	jump_if_less_than %1, 0, %2
%endmacro
;-----;


%macro jump_if_equal 3
	; %1 : value to check
	; %2 : value with which compare %1
    ; %3 : label where to jump if %1 is euqal %2

	cmp %1, %2      ; tab
    je %3
%endmacro
;-----;


%macro jump_if_not_equal 3
	; %1 : value to check
	; %2 : value with which compare %1
    ; %3 : label where to jump if %1 is not equal to %2

	cmp %1, %2      ; tab
    jne %3
%endmacro
;-----;


%macro jump_if_whitespace 2
    ; %1 : registre contenant le caractère
    ; %2 : label where to jump if %1 is a whitespace

	jump_if_equal %1, 9, %2      ; tab
    jump_if_equal %1, 10, %2     ; LF
    jump_if_equal %1, 11, %2     ; VT
    jump_if_equal %1, 12, %2     ; FF
    jump_if_equal %1, 13, %2     ; CR
    jump_if_equal %1, 32, %2     ; space
%endmacro
;-----;

;---------- VARIABLES ----------;

	section .data

str_ptr		dq 0
base_ptr	dq 0
base_len	dq 0

check_duplicate_array	times 127 dd 0

;---------- PROGRAM ----------;

	section .text
	global ft_atoi_base


ft_atoi_base:
	; check that arguments are not null
	jump_if_null rdi, return_0
	jump_if_null rsi, return_0

	; init .bss section variables
	call init_base
	jump_if_less_than QWORD [base_len], 2, return_0

	call check_base
	jump_if_negative rax, nimp

	mov rax, 1
	nimp:
	ret
;-----;


check_base:
	mov r10, 0; index
check_base.loop:
	mov r11, 0
	mov r11b, byte [base_ptr + r10]; base_ptr[index]

	jump_if_equal r11b, 0, check_base.end; r11b == '\0'

	jump_if_whitespace r11b, check_base.error1; is_whitespace(r11b)
	jump_if_equal r11b, '+', check_base.error2; r11b == '+'
	jump_if_equal r11b, '-', check_base.error3; r11b == '-'

	jump_if_equal dword [check_duplicate_array + r11], 1, check_base.error4; check for duplicates
	inc dword [check_duplicate_array + r11]; write that we have the char r11b in the string

	inc r10
	jmp check_base.loop
check_base.end:
	mov rax, 0
	ret
check_base.error1:
	mov rax, -1
	ret
check_base.error2:
	mov rax, -2
	ret
check_base.error3:
	mov rax, -3
	ret
check_base.error4:
	mov rax, -4
	ret
;-----;


init_base:
	mov qword [str_ptr], rdi
	mov qword [base_ptr], rsi

	mov rdi, qword [str_ptr]
	call ft_strlen
	mov dword [base_len], eax

	xor rax, rax
	ret
;-----;

return_0:
	mov rax, 0
	ret