extern ft_strlen       ; input: rdi = char *str, output: rax = length

;---------- HELP ----------;
; Size unit reference:
;   word  = 2 bytes
;   dword = 4 bytes
;   qword = 8 bytes

;---------- MACRO LIBRARY ----------;

%macro jump_if_null 2
    ; Jump if pointer/value is NULL (== 0)
    ; %1 : value to check
    ; %2 : label to jump to if %1 == 0

    cmp %1, 0
    je %2
%endmacro
;-----;

%macro jump_if_less_than 3
    ; Jump if %1 < %2
    ; %1 : value to check
    ; %2 : comparison value
    ; %3 : label to jump to if true

    cmp %1, %2
    jl %3
%endmacro
;-----;

%macro jump_if_negative 2
    ; Jump if %1 < 0 (negative value)
    ; %1 : value to check
    ; %2 : label to jump to if negative

    jump_if_less_than %1, 0, %2
%endmacro
;-----;

%macro jump_if_equal 3
    ; Jump if %1 == %2
    ; %1 : value to check
    ; %2 : comparison value
    ; %3 : label to jump to if equal

    cmp %1, %2
    je %3
%endmacro
;-----;

%macro jump_if_not_equal 3
    ; Jump if %1 != %2
    ; %1 : value to check
    ; %2 : comparison value
    ; %3 : label to jump to if not equal

    cmp %1, %2
    jne %3
%endmacro
;-----;

%macro jump_if_whitespace 2
    ; Jump if character in %1 is a whitespace
    ; %1 : register containing character (byte-sized)
    ; %2 : label to jump to if whitespace

    jump_if_equal %1, 0x9, %2    ; HT (tab)
    jump_if_equal %1, 0xa, %2    ; LF (line feed)
    jump_if_equal %1, 0xb, %2    ; VT (vertical tab)
    jump_if_equal %1, 0xc, %2    ; FF (form feed)
    jump_if_equal %1, 0xd, %2    ; CR (carriage return)
    jump_if_equal %1, 0x20, %2   ; space
%endmacro
;-----;

;---------- VARIABLES ----------;

section .data
    str_ptr     dq 0            ; stores input string pointer (rdi)
    base_ptr    dq 0            ; stores base string pointer (rsi)
    base_len    dd 0            ; stores length of base string
	result		dd 0            ; stores the conversion result
	sign		dq 1            ; stores the result sign
section .bss
    check_duplicate_array resb 256   ; table for checking duplicate base chars
                                     ; index = ASCII code, value = occurrence count

;---------- PROGRAM ----------;

section .text
global ft_atoi_base


; ---------- entry point ----------
ft_atoi_base:
    ; check that arguments are not NULL
    jump_if_null rdi, return_err     ; rdi = str
    jump_if_null rsi, return_err     ; rsi = base

    ; initialize internal variables (str_ptr, base_ptr, base_len)
    call atoi_init

    ; base must have at least 2 characters
    jump_if_less_than dword [rel base_len], 2, return_err

    ; check base validity (no whitespace, +, -, or duplicates)
    call check_base
	jump_if_negative eax, return_err

    call compute_number

    mov eax, dword [rel result]
    imul qword [rel sign]
    ret

; return_negative:
;     mov 
;-----;

; ---------- atoi_init ----------
atoi_init:
	; rdi = str
	; rsi = base
    mov qword [rel base_ptr], rsi  ; base_ptr = rsi (base string)

    ; sign = 1
    mov qword [rel sign], 1

	dec rdi
atoi_init.skip_whitespace_loop:
	inc rdi
	jump_if_whitespace byte [rdi], atoi_init.skip_whitespace_loop

	dec rdi
atoi_init.check_sign:
	inc rdi
	jump_if_equal byte [rdi], '+', atoi_init.check_sign
	jump_if_not_equal byte [rdi], '-', atoi_init.end
	neg qword [rel sign]
	jmp atoi_init.check_sign

atoi_init.end:

    mov qword [rel str_ptr], rdi   ; str_ptr = rdi (input string)

	; bzero check_duplicate_array
	lea rdi, [rel check_duplicate_array]
    mov rcx, 256
    xor rax, rax
    rep stosb

    ; result = 1
    mov dword [rel result], 1

    ; compute base length using ft_strlen(base)
    mov rdi, rsi
    call ft_strlen
    mov dword [rel base_len], eax

    xor rax, rax
    ret



;-----;

; ---------- check_base ----------
; Validates the base string:
; - must not contain whitespace
; - must not contain '+' or '-'
; - must not contain duplicate characters
check_base:
	push r12
	push r13

	xor rax, rax
    xor r10, r10                   ; index i = 0

    mov r12, qword [rel base_ptr]  ; r12 = base_ptr
    lea r13, [rel check_duplicate_array] ; r13 -> duplicate check array

check_base.loop:
    movzx r11, byte [r12 + r10]    ; r11 = base[i]

    jump_if_equal r11b, 0, check_base.end   ; end of string (NUL)

    jump_if_whitespace r11b, check_base.error ; whitespace not allowed
    jump_if_equal r11b, '+', check_base.error ; '+' not allowed
    jump_if_equal r11b, '-', check_base.error ; '-' not allowed

    ; check for duplicates
    jump_if_equal dword [r13 + r11], 1, check_base.error
    inc byte [r13 + r11]          ; mark char as seen

    inc r10
    jmp check_base.loop

check_base.end:
	pop r13
	pop r12
    ret

check_base.error:
    mov eax, -1
    jmp check_base.end

; ---------- compute_number ----------
compute_number:
    xor rax, rax
    mov dword [rel result], 0                             
    xor r8, r8

    mov r8, qword [rel str_ptr]  ; char *str
    xor r9, r9  ; int index = 0

    dec r9
compute_number.loop:
    inc r9

    ; str[r9] == 0 -> end
    jump_if_equal byte [r8 + r9], 0, compute_number.end

    ; find str[r9] index in base[]
    mov dil, byte [r8 + r9]
    call get_char_index_from_base_ptr

    ; not found -> end
    jump_if_negative eax, compute_number.end
    
    push rax
    xor eax, eax
    mov eax, dword [rel base_len]
    mul dword [rel result]
    mov dword [rel result], eax
    pop rax
    add dword [rel result], eax

    jmp compute_number.loop

compute_number.end:
    ret


; ---------- get_char_index_from_base_ptr ----------
get_char_index_from_base_ptr:
    ; dil = char c

    push r12
    push r13

    xor r12, r12
    xor r13, r13

    mov r12, qword [rel base_ptr]

    dec r13
get_char_index_from_base_ptr.loop:
    inc r13

    ; end of string 'base' -> not found
    jump_if_equal byte [r12 + r13], 0, get_char_index_from_base_ptr.not_found

    ; char c == base[r13] -> found
    jump_if_equal byte [r12 + r13], dil, get_char_index_from_base_ptr.found

    jmp get_char_index_from_base_ptr.loop

get_char_index_from_base_ptr.not_found:
    mov eax, -1
    jmp get_char_index_from_base_ptr.end

get_char_index_from_base_ptr.found:
    mov eax, r13d
    jmp get_char_index_from_base_ptr.end

get_char_index_from_base_ptr.end:
    ; restoring the used callee-saved registers
    pop r13
    pop r12

    ret

; ---------- return_0 ----------
return_err:
    mov eax, 0
    ret