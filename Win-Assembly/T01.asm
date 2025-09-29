; build: ml64 /c /Fo hello.obj hello.asm
; link : link /SUBSYSTEM:WINDOWS /ENTRY:main hello.obj user32.lib kernel32.lib

EXTERN  MessageBoxA:PROC
EXTERN  ExitProcess:PROC
PUBLIC  main

.data
msg db "Hello, World!", 0
cap db "Hello", 0

.code
main PROC
    ; Reserve 32-byte shadow space (Windows x64 calling convention)
    sub     rsp, 20h

    ; MessageBoxA(NULL, "Hello, World!", "Hello", 0);
    xor     rcx, rcx          ; hWnd = NULL
    lea     rdx, msg          ; lpText
    lea     r8,  cap          ; lpCaption
    xor     r9d, r9d          ; uType = 0
    call    MessageBoxA

    ; ExitProcess(0);
    xor     ecx, ecx
    call    ExitProcess

    ; (Unreached, ExitProcess does not return)
    add     rsp, 20h
    ret
main ENDP
END
