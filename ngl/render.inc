CODESEG NGL
LOCALS



; -------------------------
proc ngl__sort_point_trio basic far
; sort 3 points by height

uses ax, bx, bp, si, es
arg @@begin:word, @@seg:word
; -------------------------
    mov bp, sp
    add bp, 10

    mov si, @@begin
    mov es, @@seg
    
    mov ax, [es:si-2]
    cmp ax, [es:si-6]
    jna @@swap2

    mov eax, [dword ptr es:si-2]
    mov ebx, [dword ptr es:si-6]
    mov [dword ptr es:si-2], ebx
    mov [dword ptr es:si-6], eax

    mov eax, [dword ptr uniforms]
    mov ebx, [dword ptr uniforms+8]
    mov [dword ptr uniforms], ebx
    mov [dword ptr uniforms+8], eax

    mov eax, [dword ptr uniforms+4]
    mov ebx, [dword ptr uniforms+12]
    mov [dword ptr uniforms+4], ebx
    mov [dword ptr uniforms+12], eax

@@swap2:
    mov ax, [es:si-6]
    cmp ax, [es:si-10]
    jna @@swap3

    mov eax, [dword ptr es:si-6]
    mov ebx, [dword ptr es:si-10]
    mov [dword ptr es:si-6], ebx
    mov [dword ptr es:si-10], eax

    mov eax, [dword ptr uniforms+8]
    mov ebx, [dword ptr uniforms+16]
    mov [dword ptr uniforms+8], ebx
    mov [dword ptr uniforms+16], eax

    mov eax, [dword ptr uniforms+12]
    mov ebx, [dword ptr uniforms+20]
    mov [dword ptr uniforms+12], ebx
    mov [dword ptr uniforms+20], eax

@@swap3:
    mov ax, [es:si-2]
    cmp ax, [es:si-6]
    jna @@finish

    mov eax, [dword ptr es:si-2]
    mov ebx, [dword ptr es:si-6]
    mov [dword ptr es:si-2], ebx
    mov [dword ptr es:si-6], eax

    mov eax, [dword ptr uniforms]
    mov ebx, [dword ptr uniforms+8]
    mov [dword ptr uniforms], ebx
    mov [dword ptr uniforms+8], eax

    mov eax, [dword ptr uniforms+4]
    mov ebx, [dword ptr uniforms+12]
    mov [dword ptr uniforms+4], ebx
    mov [dword ptr uniforms+12], eax


@@finish:
    ret
endp ngl__sort_point_trio


; -------------------------
proc ngl__sort_duo basic far
; sorts 2 numbers

uses bp, si, ax, bx, es
arg @@begin:word, @@seg:word
; -------------------------
    mov bp, sp
    add bp, 10

    mov si, @@begin
    mov es, @@seg

    mov ax, [es:si]
    cmp ax, [es:si+2]
    jnb @@finish

    mov ax, [es:si]
    mov bx, [es:si+2]
    mov [es:si], bx
    mov [es:si+2], ax

@@finish:
    ret
endp ngl__sort_duo

; -------------------------
proc ngl__draw_triangle basic far
; draws a triangle to the screen

uses bp, si, cx, ax, bx, es
arg @@ax:word, @@ay:word, @@bx:word, @@by:word, @@cx:word, @@cy:word
; -------------------------
    mov bp, sp
    add bp, 12

    lea si, @@ax
    push si
    push ss
    call ngl__sort_point_trio

    mov ax, _buffer
    mov es, ax

    ; calculating vectors
    mov ax, @@bx
    mov [ngl_v0x], eax
    mov ax, @@ax
    sub [ngl_v0x], eax

    mov ax, @@by
    mov [ngl_v0y], eax
    mov ax, @@ay
    sub [ngl_v0y], eax


    mov ax, @@cx
    mov [ngl_v1x], eax
    mov ax, @@ax
    sub [ngl_v1x], eax

    mov ax, @@cy
    mov [ngl_v1y], eax
    mov ax, @@ay
    sub [ngl_v1y], eax



    ; calculating denomenator
    mov eax, [ngl_v1y]
    imul [ngl_v0x]

    mov [ngl_denom], eax
    
    mov eax, [ngl_v0y]
    imul [ngl_v1x]

    sub [ngl_denom], eax


    mov cx, @@ay
    cmp cx, 200
    jb @@firstline


@@nextline:
    inc cx
@@firstline:
    cmp cx, 8000h
    jnb @@nextline
    cmp cx, 200
    jnb @@exit
    cmp cx, @@cy
    jnb @@exit

    cmp cx, @@by
    jnb @@bc_ac

@@ab_ac:
    interpolate_capped @@ax @@ay @@bx @@by cx
    jmp @@continue

@@bc_ac:
    interpolate_capped @@bx @@by @@cx @@cy cx

@@continue:
    interpolate_capped @@ax @@ay @@cx @@cy cx


    push sp
    push ss
    call ngl__sort_duo

    pop bx
    pop dx

    inc dx

@@nextpixel:
    cmp dx, 320
    jnb @@nextline
    cmp dx, bx
    ja @@nextline


    mov ax, dx
    mov [ngl_v2x], eax
    mov ax, @@ax
    sub [ngl_v2x], eax

    mov ax, cx
    mov [ngl_v2y], eax
    mov ax, @@ay
    sub [ngl_v2y], eax


    barycentric_reduced
    call fragment_shader

    push dx

    mov si, dx
    mov ax, cx
    mul [display_width]
    add si, ax
    
    pop dx

    mov al, [shader_out]
    mov [byte ptr es:si], al

    inc dx
    jmp @@nextpixel
    

@@exit:
    ret

endp ngl__draw_triangle



; -------------------------
proc ngl_render_triangle basic far
; renders a triangle to the screen

uses si, di, ax, bx, dx, bp, cx
arg @@triangle:word ; xyzuvxyzuvxyzuv
; -------------------------
    mov bp, sp
    add bp, 14

    mov cx, 3

    mov si, @@triangle
    lea di, [uniforms]

@@projection_loop:
    xor edx, edx
    xor eax, eax
    xor ebx, ebx

    mov ax, [word ptr si+2]
    add eax, [transform+4]
    mov bx, [word ptr si]
    add ebx, [transform]

    jz @@exit_projection
    js @@exit_projection

    cdq
    shl eax, 6
    idiv ebx

    mov [di], ax
    add [word ptr di], 160

    xor edx, edx
    xor eax, eax
    xor ebx, ebx

    mov ax, [word ptr si+4]
    add eax, [transform+8]
    mov bx, [word ptr si]
    add ebx, [transform]

    jz @@exit_projection
    js @@exit_projection

    cdq
    shl eax, 6
    idiv ebx

    mov [di+2], ax
    add [word ptr di+2], 100


    mov ax, [word ptr si+6]
    mov [di+4], ax
    mov ax, [word ptr si+8]
    mov [di+6], ax

    
    add si, 10
    add di, 8

    dec cx
    jnz @@projection_loop


    push [uniforms]
    push [uniforms+2]
    push [uniforms+8]
    push [uniforms+10]
    push [uniforms+16]
    push [uniforms+18]
    call ngl__draw_triangle

@@exit_projection:
    ret
endp ngl_render_triangle

