global cr3_load_paging_directory
cr3_load_paging_directory:
    mov eax, [esp+4]    ; Get the page directory address from the passed argument
    mov cr3, eax        ; Load the page directory address into CR3
    ret                 ; Finish!

global cr0_enable_paging
cr0_enable_paging:
    mov eax, cr0        ; Get the current value of CR0
    or eax, 0x80000001  ; Set the PG and PE flags
    mov cr0, eax        ; Load the new value into CR0
    ret                 ; Finish!