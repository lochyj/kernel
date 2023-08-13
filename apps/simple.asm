; set eax to some distinguishable number, to read from the QEMU log afterwards
mov eax, 0x0000FEF1

; Random int to print eax to the QEMU log
int 0x80

; Halt but we also dont want interrupts to happen
cli
hlt
