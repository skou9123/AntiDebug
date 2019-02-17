.code

;x64‚Å‚ÍPEB‚Ígs:[60h]‚É‚ ‚é
CheckBeingDebuggedAsm64 proc
	push rbx
	xor rbx,rbx
	xor rax,rax
	mov rbx, qword ptr gs:[00000060h]
	mov al, byte ptr [rbx+2]
	pop rbx
	ret
CheckBeingDebuggedAsm64 endp

CheckNtGlobalFlagsAsm64 proc
	push rbx
	xor rbx,rbx
	xor rax,rax
	mov rbx, qword ptr gs:[00000060h]
	mov al, byte ptr [rbx+188]
	pop rbx
	ret
CheckNtGlobalFlagsAsm64 endp

GetTEBAsm64 proc
	push rbx
	xor rbx,rbx
	xor rax,rax
	mov rbx, qword ptr gs:[00000030h]
	mov rax, rbx
	pop rbx
	ret
GetTEBAsm64 endp

GetPEBAsm64 proc
	push rbx
	xor rbx,rbx
	xor rax,rax
	mov rbx, qword ptr gs:[00000060h]
	mov rax, rbx
	pop rbx
	ret
GetPEBAsm64 endp

end