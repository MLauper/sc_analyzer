#include "image_acquisition.h"
#include <arrayfire.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <filesystem>
#include <iostream>
#include <iostream>

using namespace af;

namespace fs = std::experimental::filesystem;

void RefreshDirectory(LPTSTR);
void RefreshTree(LPTSTR);
void WatchDirectory(LPTSTR);


int main()
{
	WatchDirectory("D:\\data\\image");


	return 0;
}

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, nullptr);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
	                                  (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
	                LocalSize(lpDisplayBuf) / sizeof(TCHAR),
	                TEXT("%s failed with error %d: %s"),
	                lpszFunction, dw, lpMsgBuf);
	MessageBox(nullptr, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}


void WatchDirectory(LPTSTR lpDir)
{
	DWORD dwWaitStatus;
	HANDLE dwChangeHandle;
	TCHAR lpDrive[4];
	TCHAR lpFile[_MAX_FNAME];
	TCHAR lpExt[_MAX_EXT];

	_tsplitpath_s(lpDir, lpDrive, 4, nullptr, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);

	lpDrive[2] = (TCHAR)'\\';
	lpDrive[3] = (TCHAR)'\0';

	// Watch the directory for file creation and deletion. 

	dwChangeHandle = FindFirstChangeNotification(
		lpDir, // directory to watch 
		FALSE, // do not watch subtree 
		FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes 

	if (dwChangeHandle == INVALID_HANDLE_VALUE)
	{
		printf("\n ERROR: FindFirstChangeNotification function failed.\n");
		ExitProcess(GetLastError());
	}
	if (dwChangeHandle == nullptr)
	{
		printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
		ExitProcess(GetLastError());
	}

	// Change notification is set. Now wait on notification.
	while (true)
	{
		printf("\nWaiting for notification...\n");

		dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:

			// A file was created, renamed, or deleted in the directory.
			// Refresh this directory and restart the notification.

			RefreshDirectory(lpDir);
			if (FindNextChangeNotification(dwChangeHandle) == FALSE)
			{
				printf("\n ERROR: FindNextChangeNotification function failed.\n");
				ExitProcess(GetLastError());
			}
			break;

		case WAIT_TIMEOUT:

			// A timeout occurred, this would happen if some value other 
			// than INFINITE is used in the Wait call and no changes occur.
			// In a single-threaded environment you might not want an
			// INFINITE wait.

			printf("\nNo changes in the timeout period.\n");
			break;

		default:
			printf("\n ERROR: Unhandled dwWaitStatus.\n");
			ExitProcess(GetLastError());
			break;
		}
	}
}

void RefreshDirectory(LPTSTR lpDir)
{
	// This is where you might place code to refresh your
	// directory listing, but not the subtree because it
	// would not be necessary.

	printf("\n Using Directory: %s\n", lpDir);

	//HANDLE hDir = CreateFile(
	//	lpDir,
	//	GENERIC_READ,
	//	(FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE),
	//	nullptr,
	//	OPEN_EXISTING,
	//	FILE_FLAG_BACKUP_SEMANTICS,
	//	nullptr
	//);
	//
	//if (hDir == INVALID_HANDLE_VALUE){
	//	printf("\n ERROR: Cannot create a handle for directory.\n");
	//	ErrorExit(TEXT("RefreshDirectory"));
	//}

	std::string path = "C:\\data\\image\\";
	for (auto& p : fs::directory_iterator("D:\\data\\image"))
	{
		std::string fPathS = p.path().string();
		LPCSTR fPath = fPathS.c_str();
		std::cout << fPath << "\n";

		HANDLE hFile = CreateFile(
			fPath,
			GENERIC_READ,
			(FILE_SHARE_READ | FILE_SHARE_WRITE , FILE_SHARE_DELETE),
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr
		);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("CreateFile failed with %d\n", GetLastError());
		}


		FILETIME ftCreate, ftAccess, ftWrite;
		SYSTEMTIME stUTC, stLocal;
		DWORD dwRet;

		// Retrieve the file times for the file.
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		{
			printf("GetFileTime failed with error:  %d\n", GetLastError());
		}

		// Convert the last-write time to local time.
		FileTimeToSystemTime(&ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);

		// Build a string showing the date and time.
		TCHAR szBuf[MAX_PATH];
		dwRet = StringCchPrintf(szBuf, MAX_PATH,
		                        TEXT("%02d/%02d/%d  %02d:%02d"),
		                        stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		                        stLocal.wHour, stLocal.wMinute);

		CloseHandle(hFile);

		std::cout << "file: " << p << ", Last modified timestamp: " << szBuf << std::endl;


		array I1 = loadImage(fPath);
		array I2 = loadImage(fPath);

		saveImage("c:\\temp\\out_image.jpg", I1);

		I1 = I1 /= 255.f;

		const static int width = 1024, height = 768;
		Window window(width, height, "Image Viewer");
		do
		{
			window.image(I1, "Image Titlte");
		}
		while (!window.close());
	}


	_tprintf(TEXT(" Directory (%s) changed.\n"), lpDir);
}
