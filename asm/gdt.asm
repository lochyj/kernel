; Global Descriptor Table
global gdt_flush
extern gp
gdt_flush:
	; Load the GDT
	lgdt [gp]
	; Flush the values to 0x10
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:csflush

; Why do we need this? Here: https://littleosbook.github.io/ -> 5.3
csflush:
	ret