;================================================

section .text

;================================================

global list_search_asm

extern srtcmp

;================================================

; rax, rcx, rdx, rsi, rdi, r8, r9, r10, r11

; entry: rdi = list
;        rsi = elem

list_search_asm:

        push r12

        cmp rdi, 0                      ; if list == NULL
        jne .skip1                      ; return -1;

        mov rax, -1
        ret

    .skip1:
        cmp word [rdi + 48], 0          ; if size of list is 0
        je .ret_not_found               ; then return -2;

        mov   r12d, dword [rdi + 48]    ; max ct = list->size
        movzx r8,    word [rdi + 24]    ; r8 = list->head = cur_index

        xor r9d, r9d                    ; counter

    .loop:
        cmp r9d, r12d                   ; if ct >= max ct
        jge .ret_not_found              ; stop

        inc r9d                         ; inc ct

        mov r10, rsi                    ; r10 = const char* elem

        mov r11, [rdi]                  
        mov r11, [r11 + 8 * r8]         ; r11 = const char* data

        mov rax, -16

    .compare_loop:
        add rax, 16

        movdqu xmm1, [r10 + rax]
        movdqu xmm2, [r11 + rax]

        pcmpistri xmm1, xmm2, 00011000b
    ; mask = 00011000b =  _SIDD_UBYTE_OPS         | 
                        ; _SIDD_CMP_EQUAL_EACH    |
                        ; _SIDD_NEGATIVE_POLARITY

        jnbe .compare_loop
        jnc  .ret_found

    ;strings are not equal
        mov   r10,      [rdi + 8]       ; r10 = list->next
        movzx r8 , word [r10 + 4 * r8]  ; r8 = list->next[cur_index]
        jmp .loop

    .ret_found:
        mov rax, r8
        pop r12
        ret 

    .ret_not_found:
        mov rax, -2
        pop r12
        ret 

    ; .strcmp_loop:
    ;     mov byte dl, [r10]
    ;     mov byte al, [r11]              ; load values

    ;     inc r10
    ;     inc r11                         ; to next symb

    ;     cmp dl, 0                       ; if first reached end
    ;     je .strcmp_stop                 ; jmp to check 2nd

    ;     cmp dl, al                      ; compare bytes
    ;     je .strcmp_loop                 ; if equal, repeat

    ;     jmp .next_iter                  ; else next iteration of compare

    ; .strcmp_stop:               
    ;     cmp al ,0                       ; if 2nd is not equal 1st
    ;     je .ret_found                   ; iterate to next compare

    ; .next_iter:
    ;     mov   r10,      [rdi + 8]       ; r10 = list->next
    ;     movzx r8 , word [r10 + 4 * r8]  ; r8 = list->next[cur_index]

    ;     jmp .loop
    ; .ret_found:
    ;     mov rax, r8
    ;     ret 
    
    ; .ret_not_found:
    ;     mov rax, -2
    ;     ret 

;---------------------------------------------------

    ;     cmp rdi, 0                      ; if list == NULL
    ;     jne .skip1                      ; return -1;

    ;     mov rax, -1
    ;     ret

    ; .skip1:
    ;     cmp word [rdi + 48], 0          ; if size of list is 0
    ;     je .ret_not_found               ; then return -2;

    ;     movzx rcx, word [rdi + 48]      ; max ct = list->size
    ;     movzx r8 , word [rdi + 24]      ; r8 = list->head = cur_index

    ;     xor r9, r9                      ; counter

    ; .loop:
    ;     cmp r9, rcx                     ; if ct >= max ct
    ;     jge .ret_not_found              ; stop

    ;     inc r9                          ; inc ct

    ;     mov r10, rsi                    ; r10 = const char* elem

    ;     mov r11, [rdi]
    ;     mov r11, [r11 + 8 * r8]         ; r11 = const char* data

    ; .strcmp_loop:
    ;     mov byte dl, [r10]
    ;     mov byte al, [r11]              ; load values

    ;     inc r10
    ;     inc r11                         ; to next symb

    ;     cmp dl, 0                       ; if first reached end
    ;     je .strcmp_stop                 ; jmp to check 2nd

    ;     cmp dl, al                      ; compare bytes
    ;     je .strcmp_loop                 ; if equal, repeat

    ;     jmp .next_iter                  ; else next iteration of compare

    ; .strcmp_stop:               
    ;     cmp al ,0                       ; if 2nd is not equal 1st
    ;     je .ret_found                   ; iterate to next compare

    ; .next_iter:
    ;     mov   r10,      [rdi + 8]       ; r10 = list->next
    ;     movzx r8 , word [r10 + 4 * r8]  ; r8 = list->next[cur_index]

    ;     jmp .loop
    ; .ret_found:
    ;     mov rax, r8
    ;     ret 
    
    ; .ret_not_found:
    ;     mov rax, -2
    ;     ret 

;================================================ 