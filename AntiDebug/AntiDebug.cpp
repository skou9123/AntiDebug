// AntiDebug.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "AntiDebug.h"

//CreateToolhelp32Snapshotが定義されている
#include <tlhelp32.h>


#define MAX_LOADSTRING 100

// グローバル変数:
HINSTANCE hInst;                                // 現在のインターフェイス
WCHAR szTitle[MAX_LOADSTRING];                  // タイトル バーのテキスト
WCHAR szWindowClass[MAX_LOADSTRING];            // メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
#if _WIN64 
extern "C" bool CheckBeingDebuggedAsm64();
extern "C" unsigned char CheckNtGlobalFlagsAsm64();
#endif
bool CheckBeingDebuggedAsm32();
bool CheckNtGlobalFlagsAsm32();
bool CheckBeingDebuggedWinapi();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{		
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: ここにコードを挿入してください。

    // グローバル文字列を初期化する
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_ANTIDEBUG, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // アプリケーション初期化の実行:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_ANTIDEBUG));

    MSG msg;

    // メイン メッセージ ループ:
	while (true)
	{
		#if _WIN64 
		//デバッグされているかをPEB構造体のBeingDebuggedの値をみて判断する
		//asm x64
		if (CheckBeingDebuggedAsm64())
		{
			return -1;
		}
		//デバッグされているかをPEB構造体のNtGlobalFlagの値をみて判断する
		//デバッガによってロードされているかどうかを検出する
		//asm x64
		if (CheckNtGlobalFlagsAsm64())
		{
			return -1;
		}
		#else 		
		//デバッグされているかをPEB構造体のBeingDebuggedの値をみて判断する
		//asm x86
		if (CheckBeingDebuggedAsm32())
		{
			return -1;
		}
		//デバッグされているかをPEB構造体のNtGlobalFlagの値をみて判断する
		//デバッガによってロードされているかどうかを検出する
		//asm x86
		if (CheckNtGlobalFlagsAsm32())
		{
			return -1;
		}
		#endif

		//デバッグされているかをPEB構造体のBeingDebuggedの値をみて判断する
		//WINAPI
		if (CheckBeingDebuggedWinapi())
		{
			//return -1;
		}
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			if (msg.message == WM_QUIT) 
			{
				PostQuitMessage(0);
				break;
			}

			if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	}


    return (int) msg.wParam;
}



//
//  関数: MyRegisterClass()
//
//  目的: ウィンドウ クラスを登録します。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ANTIDEBUG));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_ANTIDEBUG);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   関数: InitInstance(HINSTANCE, int)
//
//   目的: インスタンス ハンドルを保存して、メイン ウィンドウを作成します
//
//   コメント:
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // グローバル変数にインスタンス ハンドルを格納する

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  関数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND  - アプリケーション メニューの処理
//  WM_PAINT    - メイン ウィンドウを描画する
//  WM_DESTROY  - 中止メッセージを表示して戻る
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 選択されたメニューの解析:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: HDC を使用する描画コードをここに追加してください...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

#ifndef _WIN64 
bool CheckBeingDebuggedAsm32()
{
	char* peb = 0;
	//__asm はvc++32bitでしか使用不可 PEB は fs : [00000030h]
	__asm
	{
		mov eax, dword ptr fs : [00000030h]
		mov peb, eax
	}
	//BeingDebuggedはデバッグ後に書き換えられたら検出できない
	//*(peb + 2)=0;

	//アセンブリでPEBのBeingDebuggedをチェックしている
	if (*(peb + 2))
	{
		//デバッグされている
		return true;
	}
	return false;
}

bool CheckNtGlobalFlagsAsm32()
{
	char* peb = 0;
	//__asm はvc++32bitでしか使用不可 PEB は fs : [00000030h]
	__asm
	{
		mov eax, dword ptr fs : [00000030h]
		mov peb, eax
	}
	//NtGlobalFlagはデバッグ後に書き換えられたら検出できない
	//*(peb + 0x68)=0;

	//アセンブリでPEBのNtGlobalFlagをチェックしている
	if (*(peb + 0x68))
	{
		//デバッグされている
		return true;
	}
	return false;
}
#endif

bool CheckBeingDebuggedWinapi()
{
	//WINAPIでPEBのBeingDebuggedをチェックしている
	if (IsDebuggerPresent())
	{
		//デバッグされている
		return true;
	}
	return false;
}
