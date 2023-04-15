LOCALS
CODESEG NGL

proc ngl__init_palette basic far
uses ax, bx, cx, dx

    mov bx, 0; color number

@@loop:
    mov ax, bx
    xor dx, dx
    div [six]
    xor dx, dx
    div [six]   
    xor dx, dx
    div [six]
    
    mov ax, dx
    mul [sixty_three]
    xor dx, dx
    div [five]

    mov cl, al; blue (0~63)


    mov ax, bx
    xor dx, dx
    div [six]
    xor dx, dx
    div [six]
    
    mov ax, dx
    mul [sixty_three]
    xor dx, dx
    div [five]

    mov ch, al; green (0~63)


    mov ax, bx
    xor dx, dx
    div [six]

    mov ax, dx
    mul [sixty_three]
    xor dx, dx
    div [five]

    mov dh, al; red (0~63)


    mov ax, 1010h; change color palette
    int 10h; video bios interrupt

    inc bx
    cmp bx, 216
    jne @@loop

    ret
endp ngl__init_palette


macro init_palette
    call ngl__init_palette
endm

