DATASEG
ngl_temp dd ?

CODESEG NGL
LOCALS

; -------------------------
proc ngl__linear_interpolation basic far
; returns interpolated x value between 2 points

uses bx, dx, bp, cx
arg @@y:word, @@x1:word, @@y1:word, @@x2:word, @@y2:word
; -------------------------
    ; (y-y1)(x2-x1) / (y2-y1) + x1

    mov bp, sp
    add bp, 8

    mov ax, @@y
    sub ax, @@y1

    mov bx, @@x2
    sub bx, @@x1

    imul bx

    mov bx, @@y2
    sub bx, @@y1
    
    mov cx, ax
    mov ax, @@x1

    imul bx
    add ax, cx

    xor dx, dx
    idiv bx

    ret
endp ngl__linear_interpolation


; linear interpolation macro
macro interpolate x1, y1, x2, y2, y
    push ax
    push &y
    push &x1
    push &y1
    push &x2
    push &y2
    call ngl__linear_interpolation
    pop [word ptr ngl_temp]
    push ax
    mov ax, [word ptr ngl_temp]
endm interpolate

; linear interpolation macro
macro interpolate_capped x1, y1, x2, y2, y
    LOCAL @@positive_res
    push ax
    push &y
    push &x1
    push &y1
    push &x2
    push &y2
    call ngl__linear_interpolation
    pop [word ptr ngl_temp]

    cmp ax, 512
    jna @@positive_res

    mov ax, 0

@@positive_res:
    push ax
    mov ax, [word ptr ngl_temp]
endm interpolate_capped



; -------------------------
proc ngl__dot_product basic far
; calculate the dot product of a pair of 2d vectors

uses ax, bp, dx
arg @@a:dword, @@b:dword, @@x:dword, @@y:dword
; -------------------------
    mov bp, sp
    add bp, 6

    mov eax, @@a
    imul @@x
    mov ebx, eax
    mov eax, @@b
    imul @@y
    add ebx, eax

    ret
endp ngl__dot_product


macro dot_product a, b, x, y
    push bx
    ror ebx, 16
    push bx
    
    push &a
    push &b
    push &x
    push &y
    call ngl__dot_product

    pop [word ptr ngl_temp+2]
    pop [word ptr ngl_temp]

    push bx
    ror ebx, 16
    push bx

    mov ebx, [ngl_temp]
endm dot_product


DATASEG
ngl_v0x dd ? 
ngl_v0y dd ?
ngl_v1x dd ?
ngl_v1y dd ?
ngl_v2x dd ?
ngl_v2y dd ?

ngl_d00 dd ?
ngl_d01 dd ?
ngl_d11 dd ?
ngl_d20 dd ?
ngl_d21 dd ?

ngl_denom dd ?

CODESEG NGL


; -------------------------
proc ngl__barycentric_coordinates basic far
; compute the barycentric coordinates of a point

uses ax, si, bp, dx
arg @@vwu:word, @@px:word, @@py:word, @@ax:word, @@ay:word, @@bx:word, @@by:word, @@cx:word, @@cy:word
; -------------------------
    mov bp, sp
    add bp, 8

    xor eax, eax

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


    mov ax, @@px
    mov [ngl_v2x], eax
    mov ax, @@ax
    sub [ngl_v2x], eax

    mov ax, @@py
    mov [ngl_v2y], eax
    mov ax, @@ay
    sub [ngl_v2y], eax



    ; calculating denomenator
    mov eax, [ngl_v1y]
    imul [ngl_v0x]

    mov [ngl_denom], eax
    
    mov eax, [ngl_v0y]
    imul [ngl_v1x]

    sub [ngl_denom], eax

    cmp [ngl_denom], 0
    jne @@continue
    ret

@@continue:

    ; calculating v
    mov eax, [ngl_v1y]
    imul [ngl_v2x]

    mov si, @@vwu
    mov [dword ptr ds:si], eax

    mov eax, [ngl_v2y]
    imul [ngl_v1x]

    sub [dword ptr ds:si], eax
    
    ; calculating w
    mov eax, [ngl_v2y]
    imul [ngl_v0x]

    add si, 4
    mov [dword ptr ds:si], eax

    mov eax, [ngl_v0y]
    imul [ngl_v2x]

    sub [dword ptr ds:si], eax

    ; calculating u (denom - w - v)
    mov eax, [ngl_denom]
    sub eax, [dword ptr ds:si]
    sub si, 4
    sub eax, [dword ptr ds:si]
    add si, 8
    mov [dword ptr ds:si], eax

    ; moving u v and w between 1 and 16
    mov eax, [dword ptr ds:si]
    shl eax, 4
    xor edx, edx
    idiv [ngl_denom]
    mov [dword ptr ds:si], eax

    sub si, 4

    mov eax, [dword ptr ds:si]
    shl eax, 4
    xor edx, edx
    idiv [ngl_denom]
    mov [dword ptr ds:si], eax

    sub si, 4

    mov eax, [dword ptr ds:si]
    shl eax, 4
    xor edx, edx
    idiv [ngl_denom]
    mov [dword ptr ds:si], eax

    ret
endp ngl__barycentric_coordinates


; barycentric coordinates conversion macro
macro barycentric vwu, aax, aay, bbx, bby, ccx, ccy, ppx, ppy
    push &vwu
    push &ppx
    push &ppy
    push &aax
    push &aay
    push &bbx
    push &bby
    push &ccx
    push &ccy
    call ngl__barycentric_coordinates
endm barycentric



; -------------------------
proc ngl__barycentric_reduced_computation basic far
; compute the barycentric coordinates of a point

uses ax, si, dx, bx
; -------------------------
    ; calculating v
    mov eax, [ngl_v1y]
    imul [ngl_v2x]

    lea si, [atributes]
    mov [dword ptr ds:si], eax

    mov eax, [ngl_v2y]
    imul [ngl_v1x]

    sub [dword ptr ds:si], eax
    
    ; calculating w
    mov eax, [ngl_v2y]
    imul [ngl_v0x]

    add si, 4
    mov [dword ptr ds:si], eax

    mov eax, [ngl_v0y]
    imul [ngl_v2x]

    sub [dword ptr ds:si], eax

    ; moving u v and w between 1 and 16
    mov eax, [ngl_denom]
    mov ebx, eax

    mov eax, [dword ptr ds:si]
    shl eax, 4
    cdq
    idiv ebx
    mov [dword ptr ds:si], eax

    sub si, 4

    mov eax, [dword ptr ds:si]
    shl eax, 4
    cdq
    idiv ebx
    mov [dword ptr ds:si], eax

    add si, 8
    mov [dword ptr ds:si], 16
    mov eax, [dword ptr ds:si-4]
    add eax, [dword ptr ds:si-8]
    sub [dword ptr ds:si], eax

    ret
endp ngl__barycentric_reduced_computation


; barycentric coordinates conversion macro (reduced computation)
macro barycentric_reduced
    call ngl__barycentric_reduced_computation
endm barycentric_reduced

