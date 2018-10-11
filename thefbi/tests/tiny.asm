  ; tiny.asm
  BITS 32
  GLOBAL _start
  SECTION .text
  _start:

	mov ecx,0
line:
	mov ebx,0
pixel:	
	
	mov eax,ebx
	sub eax,320	
	imul eax,eax
	
	mov edx,ecx
	sub edx,240
	imul edx,edx
	add edx,eax
	mov eax,0x19
	int 0x80
	mov esi,ebx
	mov edi,ecx
	
	mov eax,0x05
	int 0x80

	mov ecx,ebx
	mov ebx,eax
	mov edx,0xff80ff
	mov eax,0x19 
	int 0x80

	mov ebx,esi
	mov ecx,edi
	
	inc ebx
	cmp ebx,640
	jnz pixel
	inc ecx
	cmp ecx,480
	jnz line
	
	mov edx,len
	mov ecx,heya
	mov ebx,1
	mov eax,4
	int 0x80

	mov ebx,somescript
	mov eax,0x03
	int 0x80

m:
	mov ebx,0x10
	mov eax,0x02
	int 0x80
	mov eax,0x05
	int 0x80
	mov ecx,ebx
	mov ebx,eax
	mov edx,0xff80ff
	mov eax,0x19 
	int 0x80
	jmp m
	
	

	mov     eax, 1
        mov     ebx, 42  
        int     0x80

	mov eax,53281
	mov ebx,0xBADF00D
	mov [eax],ebx

section .data
	
heya db 'All Done!',0xa,0
somescript db 'var Title = document.getElementById("title"); Title.innerHTML+=" I added some text "; ',0
len  equ $ - heya

		
