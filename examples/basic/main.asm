.386
IDEAL
MODEL large
STACK 100h

include "\ngl\ngl.inc"

DATASEG

; --------------------------
vwu dw 3 dup(0)
x1 dw 194
y1 dw 56
x2 dw 230
y2 dw 89
x3 dw 75
y3 dw 157

;     x   y   z  u   v  x   y   z  u    v  z   y   z   u  v
t1 dw 40, 20, 0, 69, 0, 40, 40, 0, 180, 0, 40, 40, 20, 1, 0
t2 dw 40, 20, 0, 69, 0, 40, 20, 20, 5, 0, 40, 40, 20, 1, 0
t3 dw 40, 20, 0, 69, 0, 60, 20, 0, 3, 0, 40, 20, 20, 5, 0
t4 dw 40, 20, 20, 5, 0, 60, 20, 20, 215, 0, 60, 20, 0, 3, 0
t5 dw 40, 20, 0, 69, 0, 40, 40, 0, 180, 0, 60, 20, 0, 3, 0
t6 dw 40, 40, 0, 180, 0, 60, 40, 0, 17, 0, 60, 20, 0, 3, 0
t7 dw 20, 0, 20, 0, 0, 80, 0, 20, 0, 0, 20, 60, 20, 0, 0
t8 dw 20, 60, 20, 0, 0, 80, 60, 20, 0, 0, 80, 0, 20, 0, 0

shade_r dw ?
shade_g dw ?
shade_b dw ?
; --------------------------

CODESEG NGL
proc fragshader basic near
uses ax, bx, dx

    mov ax, [uniforms+4]
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes+8]

    mov [shade_r], ax

    mov ax, bx
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes+8]

    mov [shade_g], ax

    mov ax, bx
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes+8]

    mov [shade_b], ax


    mov ax, [uniforms+12]
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes]

    add [shade_r], ax

    mov ax, bx
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes]

    add [shade_g], ax

    mov ax, bx
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes]

    add [shade_b], ax


    mov ax, [uniforms+20]
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes+4]

    add [shade_r], ax

    mov ax, bx
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes+4]

    add [shade_g], ax

    mov ax, bx
    xor dx, dx
    div [six]
    
    mov bx, ax
    mov ax, dx
    
    mul [word ptr atributes+4]

    add [shade_b], ax


    shr [shade_r], 4
    shr [shade_g], 4
    shr [shade_b], 4

    mov ax, [shade_b]
    mul [six]
    add ax, [shade_g]
    mul [six]
    add ax, [shade_r]

    
    mov [shader_out], al

    ret
endp fragshader


CODESEG
start:
    mov ax, @data
    mov ds, ax

    lea si, [fragshader]
    mov [fragment_shader], si

; --------------------------
    init_graphics
    init_palette

loop1:
    fill_screen 43

    lea si, [t8]
    push si
    call ngl_render_triangle
    
    lea si, [t7]
    push si
    call ngl_render_triangle

    lea si, [t4]
    push si
    call ngl_render_triangle

    lea si, [t3]
    push si
    call ngl_render_triangle

    lea si, [t6]
    push si
    call ngl_render_triangle

    lea si, [t5]
    push si
    call ngl_render_triangle

    lea si, [t2]
    push si
    call ngl_render_triangle

    lea si, [t1]
    push si
    call ngl_render_triangle

    update_display

    get_keystroke
    cmp al, 'd'
    jne next1
    sub [transform+4], 2

next1:
    cmp al, ' '
    jne next2
    add [transform+8], 2

next2:
    cmp al, 'a'
    jne next3
    add [transform+4], 2

next3:
    cmp al, 'c'
    jne next4
    sub [transform+8], 2

next4:
    cmp al, 'w'
    jne next5
    dec [transform]

next5:
    cmp al, 's'
    jne next6
    inc [transform]

next6:
    cmp al, 'p'
    jne next7
    mov ax, ax

next7:
    cmp al, 'q'
    jne loop1
; --------------------------
    
exit:
    init_text
    mov ax, 4c00h
    int 21h

END start


