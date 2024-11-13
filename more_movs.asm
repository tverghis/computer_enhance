bits 16

mov si, bx
mov dh, al
mov al, [bx + si]
mov bx, [bp + di]
mov dx, [bp]
mov [bx + di], cx
mov [bp + si], cl
mov [bp], ch
