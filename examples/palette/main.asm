.386
IDEAL
MODEL large
STACK 100h

include "\ngl\ngl.inc"

DATASEG

; --------------------------
x dw 0
y dw 0
three dw 3
hundred_and_sixty dw 160
two_hundred_and_sixteen dw 216
one_hundred dw 100
red dw 0
; --------------------------

CODESEG
start:
    mov ax, @data
    mov ds, ax

    init_graphics
    init_palette
    call main
    init_text
    
    jmp exit

; --------------------------

proc main
    mov ax, _buffer
    mov es, ax
    assume es:_buffer

    mov si, 0
    jmp draw_loop

increase:
    inc [red]
    cmp [red], 6
    jne draw
    mov [red], 0
    jmp draw

decrease:
    dec [red]
    jns draw
    mov [red], 5

draw:
    mov si, 0

draw_loop:
    mov ax, si

    xor dx, dx
    div [display_width]

    push ax
    mov ax, dx

    mul [three]
    xor dx, dx
    div [hundred_and_sixty]

    mov [es:si], al

    pop ax

    mul [three]
    xor dx, dx
    div [one_hundred]
    mul [six]

    add [es:si], al

    mov ax, [red]
    mul [six]
    mul [six]
    add [es:si], al

    fill_screen 69
    update_display 20 20 40 40

    inc si
    cmp si, 64000
    jne draw_loop

    update_display

    get_keystroke

    cmp ah, 4Bh
    je decrease

    cmp ah, 4Dh
    je increase

    cmp al, 'q'
    jne draw

    ret
endp main

; --------------------------
    
exit:
    mov ax, 4c00h
    int 21h
END start

