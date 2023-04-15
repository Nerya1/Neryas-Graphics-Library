; buffer for shader parameters
DATASEG
uniforms dw 12 dup(?)
atributes dd 3 dup(?)
shader_out db 30 dup(?)
vertex_shader dw ?
fragment_shader dw ?
transform dd 3 dup(0)

