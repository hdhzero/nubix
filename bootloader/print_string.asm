[bits 16]

print_string:
    pusha

print_string_loop:
    mov al, [bx]
    mov ah, 0x0f

    cmp al, 0
    je print_string_done

    int 0x10
    add ebx, 1

    jmp print_string_loop

print_string_done:
    popa
    ret
