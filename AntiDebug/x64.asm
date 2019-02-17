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

end