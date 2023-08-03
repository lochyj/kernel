; TODO: fix these functions
global check_cpu_id_support
check_cpu_id_support:
    pushfd                               ;Save EFLAGS
    pushfd                               ;Store EFLAGS
    xor dword [esp],0x00200000           ;Invert the ID bit in stored EFLAGS
    popfd                                ;Load stored EFLAGS (with ID bit inverted)
    pushfd                               ;Store EFLAGS again (ID bit may or may not be inverted)
    pop eax                              ;eax = modified EFLAGS (ID bit may or may not be inverted)
    xor eax,[esp]                        ;eax = whichever bits were changed
    popfd                                ;Restore original EFLAGS
    and eax,0x00200000                   ;eax = zero if ID bit can't be changed, else non-zero
    ret

global get_cpu_vendor_string
get_cpu_vendor_string:
    mov eax, 0x0
    cpuid
    mov [ebx], eax
    mov [ebx + 4], ebx
    mov [ebx + 8], ecx
    ; return the values
    mov eax, ebx
    ret

global call_function_from_pointer
call_function_from_pointer:
    mov eax, [esp+4]
    jmp eax
    ret