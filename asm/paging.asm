global read_cr0
read_cr0:
	mov eax, cr0
	retn

global write_cr0
write_cr0:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr0,  eax
	pop ebp
	retn

global read_cr3
read_cr3:
	mov eax, cr3
	retn

global write_cr3
write_cr3:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax
	pop ebp
	retn

global switch_page_directory
switch_page_directory:
	push ebp
	mov ebp, esp
	mov eax, [ebp+8]
	mov cr3, eax

	mov eax, cr0
	or eax, 0x80000001
	mov cr0, eax
	retn