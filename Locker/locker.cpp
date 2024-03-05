#pragma once

/**
	Author: Alwyn Dippenaar
	The impl for locker.
**/

#include "locker.h"


HHOOK g_hKeyboardHook = NULL;
HHOOK g_MouseHook = NULL;

wchar_t tmpbuf[2048];

wchar_t key[128];
wchar_t keyval[2048];

bool quit = false;

wchar_t pwd[2048];

void mousemov(void* args)
{
	while (!quit)
	{
		SetCursorPos(100, 100);
		Sleep(1);
	}
	_endthread();
}


LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam){

	PKBDLLHOOKSTRUCT k = (PKBDLLHOOKSTRUCT)(lParam);
	POINT p;


	return 1;
}


/**
This function handles the key events.
**/
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{

	ZeroMemory(key, sizeof(wchar_t)* 128);
	ZeroMemory(keyval, sizeof(wchar_t)* 2048);


	LPKBDLLHOOKSTRUCT str = NULL;
	str = (LPKBDLLHOOKSTRUCT)lParam;


	if (str->vkCode == VK_ESCAPE)
	{
		//quit = true;
	}
	else
	{

		if (wParam == WM_KEYDOWN)
		{

			wchar_t buff[10];
			ZeroMemory(buff, sizeof(wchar_t)* 10);

			BYTE keyState[256] = { 0 };
			int result = ToUnicodeEx(str->vkCode, str->scanCode, keyState, buff, _countof(buff), 0, NULL);
			if (result > 0)
			{
				wcscat_s(tmpbuf, buff);

				//logger->debug(L"TmpBuf: ");
				//logger->debug(tmpbuf);
				//logger->debug(L"\r\n");

				if (wcslen(tmpbuf) > wcslen(pwd))
				{
					ZeroMemory(tmpbuf, sizeof(wchar_t)* 2048);
				}

				for (int i = 0; i < wcslen(tmpbuf); i++)
				{
					if (tmpbuf[i] != pwd[i])
					{
						ZeroMemory(tmpbuf, sizeof(wchar_t)* 2048);
						break;
					}
				}
				if (_wcsicmp(tmpbuf, pwd) == 0)
				{
					quit = true;
				}
			}
		}
	}


	if (quit)
	{
		Sleep(100);
		ShowCursor(TRUE);

		if (g_MouseHook) UnhookWindowsHookEx(g_MouseHook);
		if (g_hKeyboardHook) UnhookWindowsHookEx(g_hKeyboardHook);


		g_MouseHook			= NULL;
		g_hKeyboardHook		= NULL;

		exit(0);
		return 1;
	}

	return 1;
}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{

	ZeroMemory(tmpbuf, sizeof(wchar_t)* 2048);

	SAFE_ZERO(pwd, wchar_t, 2048);

	LPWSTR *szArgList;
	int argCount;
	szArgList = CommandLineToArgvW(GetCommandLineW(), &argCount);
	if (szArgList && argCount > 1)
	{
		swprintf_s(tmpbuf, 2048, L"Arguments \n\0", argCount);
		for (int i = 0; i < argCount; i++)
		{
			swprintf_s(tmpbuf, 2048, L"# %i '%s' \n\0", i, szArgList[i]);
		}

		memcpy_s(pwd, sizeof(wchar_t) * 2048, szArgList[1], sizeof(wchar_t) * wcslen(szArgList[1]));

	} //if
	else
	{
		swprintf_s(pwd, 2048, L"password\0", pwd);
	} //else

	LocalFree(szArgList);
	szArgList = NULL;

	ZeroMemory(tmpbuf, sizeof(wchar_t) * 2048);


	g_hKeyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
	g_MouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseHookProc, hInstance, 0);

	_beginthread(mousemov, 0, NULL);

	MSG msg;
	int bRet = 0;
	while (!quit && (bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		Sleep(1);

		if (bRet == -1)
		{
			// handle the error and possibly exit
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	ShowCursor(TRUE);

	if (g_MouseHook) UnhookWindowsHookEx(g_MouseHook);
	if (g_hKeyboardHook) UnhookWindowsHookEx(g_hKeyboardHook);

	return 0;
}