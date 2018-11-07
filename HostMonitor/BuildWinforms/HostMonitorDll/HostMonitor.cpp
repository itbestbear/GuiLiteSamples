// HostMonitorDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <assert.h>
#include <shellapi.h>

extern int startHostMonitor(int main_cnt, int main_width, int main_height, int sub_cnt, int sub_width, int sub_height, int color_bytes);
extern void* getUiOfHostMonitor(int display_id, int* width, int* height);
extern int sendTouch2HostMonitor(void* buf, int len, int display_id);

DWORD WINAPI threadHostMonitor(LPVOID lpParam)
{
	startHostMonitor(1, 1024, 768, 8, 1024, 370, 4);
	return 0;
}

extern "C" __declspec(dllexport) int startHostMonitorFromDll()
{
	ShellExecute(0, L"open", L"https://github.com/idea4good/GuiLite", L"", L"", SW_SHOWNORMAL);
	ShellExecute(NULL, L"open", L"sync_data.bat", L"60 98 30 120 80 100 WinForms", NULL, SW_HIDE);//Ping cloud
	DWORD pid;
	CreateThread(NULL, 0, threadHostMonitor, NULL, 0, &pid);
	return 0;
}

extern "C" __declspec(dllexport) HBITMAP getHbitmapOfHostMonitorUiFromDll(int display_id, int width, int height)
{
	static HBITMAP s_bitmap;
	BYTE* buffer = (BYTE*)getUiOfHostMonitor(display_id, NULL, NULL);
	if (buffer)
	{
		if (s_bitmap)
		{
			DeleteObject(s_bitmap);
		}
		s_bitmap = CreateBitmap(width, height, 1, 32, buffer);
		return s_bitmap;
	}
	return 0;
}

typedef struct
{
	unsigned int dwMsgId;
	unsigned int dwParam1;
	unsigned int dwParam2;
}OUTMSGINFO;
extern "C" __declspec(dllexport) void sendTouch2HostMonitorFromDll(int display_id, unsigned int msg_id, int x, int y)
{
	OUTMSGINFO msg;
	msg.dwMsgId = msg_id;
	msg.dwParam1 = x;
	msg.dwParam2 = y;
	sendTouch2HostMonitor(&msg, sizeof(msg), display_id);
}
