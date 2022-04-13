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

        cmp rdi, 0                      ; if list == NULL
        jne .skip1                      ; return -1;

        mov rax, -1
        ret

    .skip1:
        cmp word [rdi + 48], 0          ; if size of list is 0
        je .ret_not_found               ; then return -2;

        mov rcx, [rdi + 48]             ; max ct = list->size
        mov r8,  [rdi + 24]             ; r8 = list->head = cur_index

        xor r9, r9                      ; counter

    .loop:
        cmp r9, rcx                     ; if ct >= max ct
        jge .ret_not_found              ; stop

        inc r9                          ; inc ct

        mov r10, rsi                    ; r10 = const char* elem

        mov r11, [rdi]
        mov r11, [r11 + 8 * r8]         ; r11 = const char* data

    .strcmp_loop:
        mov byte dl, [r10 + r9]
        mov byte al, [r11 + r9]         ; load values

        cmp dl, al                      ; compare bytes
        je .strcmp_loop                 ; if equal, repeat

        cmp dl, 0                       ; if first reached end
        je .strcmp_stop                 ; jmp to check 2nd

        jmp .next_iter                  ; else next iteration of compare

    .strcmp_stop:               
        cmp al ,0                       ; if 2nd is not equal 1st
        je .ret_found                   ; iterate to next compare

    .next_iter:
        mov r10, [rdi + 8]              ; r10 = list->next
        mov r8, [r10 + 4 * r8]          ; r8 = list->next[cur_index]

    .ret_found:
        mov rax, r8
        ret 
    
    .ret_not_found:
        mov rax, -2
        ret 

;================================================ 