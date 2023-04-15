; -------------------------
; - include library files -
; -------------------------

; disable warns from ngl
IFNDEF DONGLWARN
    NOWARN
ENDIF

; include constants
include "\ngl\const.inc"

; include math
include "\ngl\math.inc"

; include pallete procedure
include "\ngl\palette.inc"

; create screen buffer
include "\ngl\buff.inc"

; shaders and shader parameters
include "\ngl\shaders.inc"

; screen related procedures
include "\ngl\screen.inc"

; user io procedures
include "\ngl\usrio.inc"

; drawing procedures
include "\ngl\draw.inc"

; rendering procedures
include "\ngl\render.inc"

WARN

