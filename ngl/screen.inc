LOCALS
CODESEG NGL


; -------------------------
proc ngl__init_graphics basic far
; sets video mode to graphical

uses ax
; ------------------------
    
    mov al, 13h
    mov ah, 0
    int 10h

    ret
endp ngl__init_graphics


; macro for ngl__init_graphics
macro init_graphics
    call ngl__init_graphics
endm init_graphics



; ------------------------
proc ngl__init_text basic far
; sets video mode to text

uses ax
; ------------------------

    mov al, 03h
    mov ah, 0
    int 10h

    ret
endp ngl__init_text


; macro for ngl__init_text
macro init_text
    call ngl__init_text
endm



; ------------------------
proc ngl__update_display basic far
; writes screen buffer to display

uses ax, si, di, cx, bx, dx, ds, es ; bp at macro
arg @@x0:word, @@y0:word, @@x1:word, @@y1:word
; ------------------------

    mov bp, sp
    add bp, 16

    mov ax, @@y0
    mul [display_width]
    add ax, @@x0

    ; si = starting index
    mov si, ax
    
    mov ax, @@x1
    sub ax, @@x0

    ; dx = number of pixels in a row
    mov dx, ax

    mov ax, @@y1
    sub ax, @@y0

    ; bx = number of rows
    mov bx, ax

    ; ds = buffer segment
    mov ax, _buffer
    mov ds, ax
    
    ; es = display address
    mov ax, 0A000h
    mov es, ax

    ; ax = distance to new line
    mov ax, 320
    sub ax, @@x1
    add ax, @@x0

    ; clear direction flag
    cld

@@next_row:
    mov di, si
    mov cx, dx

    rep movsb

    add si, ax

    dec bx
    jnz @@next_row

    ret
endp ngl__update_display


; ------------------------
proc ngl__update_display_all basic far
; writes entire screen buffer to display

uses ax, si, di, cx, ds, es
; ------------------------

    ; ds = buffer segment
    mov ax, _buffer
    mov ds, ax

    ; es = display address
    mov ax, 0A000h
    mov es, ax

    ; cx = addresses to copy
    mov cx, 16000
    
    ; set si and di to 0
    xor si, si
    xor di, di
    
    ; clear direction flag
    cld

    ; copy dwords from buffer to display
    rep movsd

    ret
endp ngl__update_display_all


; ngl__update_display macro
macro update_display x0, y0, x1, y1
    IF "&x0&" EQ ""

    call ngl__update_display_all
    
    ELSE

    push bp

    push &x0
    push &y0
    push &x1
    push &y1
    call ngl__update_display

    pop bp

    ENDIF

endm update_display

