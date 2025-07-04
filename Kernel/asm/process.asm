; loadProcess.asm


global loadProcessAsm

global idle

global forceTimerInterruption


%macro pushState 0
    push r15
    push r14
    push r13
    push r12
    push r11
    push r10
    push r9
    push r8
    push rsi         
    push rdi
    push rbp
    push rdx
    push rcx
    push rbx
    push rax
%endmacro

%macro popState 0
    pop rax
    pop rbx
    pop rcx
    pop rdx
    pop rbp
    pop rdi
    pop rsi
    pop r8
    pop r9
    pop r10
    pop r11
    pop r12
    pop r13
    pop r14
    pop r15
%endmacro

section .text

loadProcessAsm:
    mov rsp, rdi     ; Load stack pointer from first argument
    pop rbp          ; Restore base pointer
    ret              ; Return to entryPoint



idle: 
    hlt
    jmp idle

forceTimerInterruption:
    int 0x20
    ret
