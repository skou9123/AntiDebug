# AntiDebug  
## BeingDebugged
BeingDebuggedはPEB構造体のメンバである  
PEB構造体のアドレスは  
x86ならFSレジスタ:[30h]に配置されている  
mov eax, dword ptr fs : [00000030h]  
eaxにPEB構造体のアドレスが入る  
x64ならGSレジスタ:[60h]に配置されている  
mov rax, qword ptr gs:[00000060h]  
raxにPEB構造体のアドレスが入る
#### 防衛側
PEB構造体のBeingDebuggedをチェックすれば  
デバッガによってアタッチされているかを確認できる。  
アタッチされている場合trueされていない場合false  
  
#### 攻撃側
デバッグ直後BeingDebuggeの値を0にすればこの対策を無効化できる  
## NtGlobalFlags  
NtGlobalFlagはPEB構造体のメンバである
デバッガによってプロセスが作成されると  
NtGlobalFlagで設定される値が変わる
#### 防衛側
PEB構造体のNtGlobalFlagをチェックすれば  
デバッガによってプロセスが作成されているかを確認できる。  
デバッグされている場合以下の論理和0x70  
FLG_HEAP_ENABLE_TAIL_CHECK (0x10)  
FLG_HEAP_ENABLE_FREE_CHECK (0x20)  
FLG_HEAP_VALIDATE_PARAMETERS (0x40)  
されていない場合  
0  
が設定されている
  
#### 攻撃側
PEB構造体のNtGlobalFlagをソフトウェア側にチェックされる前に  
0にすればこの対策を無効化できる  
または  
HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options
