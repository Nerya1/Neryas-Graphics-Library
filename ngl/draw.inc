LOCALS
CODESEG NGL


; -------------------------
proc ngl__draw_pixel basic far
; draws a pixel to the screen

uses es, ax, si, bp
arg @@x:word, @@y:word, @@color:byte
; -------------------------

    mov bp, sp

    mov ax, @@y
    mul [display_width]
    add ax, @@x

    ; si = pixel address
    mov si, ax

    ; es = buffer segment
    mov ax, _buffer
    mov es, ax

    ; set pixel to color
    mov al, @@color
    mov [es:si], al

    ret
endp ngl__draw_pixel


; ngl__draw_pixel macro
macro draw_pixel x, y, c
    push &x
    push &y
    push &c
    call ngl__draw_pixel
endm draw_pixel



; -------------------------
proc ngl__fill_screen basic far
; fills screen with a single color

uses ax, cx, di, es, bp
arg @@color:byte
; -------------------------

    mov bp, sp
    add bp, 10

    mov cx, 16000
    mov di, 0
    mov ax, _buffer
    mov es, ax

    mov al, @@color
    mov ah, @@color
    shl eax, 16
    mov al, @@color
    mov ah, @@color

    cld
    rep stosd

    ret
endp ngl__fill_screen


; ngl__fill_screen macro
macro fill_screen c
    push &c
    call ngl__fill_screen
endm fill_screen

