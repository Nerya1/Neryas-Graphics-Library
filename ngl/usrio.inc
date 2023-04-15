LOCALS
CODESEG NGL


; -------------------------
proc ngl__check_keystroke basic far
; zf = 1 if no keystroke available

uses ax
; -------------------------

    mov ah, 01h
    int 16h

    ret
endp ngl__check_keystroke


; ngl__check_keystroke macro
macro check_keystroke
    call ngl__check_keystroke
endm check_keystroke



; -------------------------
proc ngl__get_keystroke basic far
; get keystroke from keyboard, stores result in ax
; -------------------------

    mov ah, 00h
    int 16h

    ret
endp ngl__get_keystroke


; ngl__get_keystroke macro
macro get_keystroke
    call ngl__get_keystroke
endm get_keystroke

