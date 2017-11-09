#include "FileLoader.h"
#include <string>
#include <bemapiset.h>
#include <tchar.h>
#include <filesystem>
#include <arrayfire.h>
#include <strsafe.h>

namespace fs = std::experimental::filesystem;

void image_acquisition::FileLoader::SetPrefix(std::string prefix)
{
	this->prefix = prefix;
}

void image_acquisition::FileLoader::WatchDirectory()
{
	LPCTSTR lpDir = this->directory.c_str();

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

			this->ProcessFiles();
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

void image_acquisition::FileLoader::ProcessFiles()
{
	LPCSTR lpDir = this->directory.c_str();

	// This is where you might place code to refresh your
	// directory listing, but not the subtree because it
	// would not be necessary.

	printf("\n Using Directory: %s\n", lpDir);

	std::string path = "C:\\data\\image\\";

	//for (auto& p : fs::directory_iterator("D:\\data\\image"))
	auto di = fs::directory_iterator("D:\\data\\image");
	auto begin = fs::begin(di);
	auto end = fs::end(di);
	for (auto it = begin; begin != end; ++it)
	{
		auto p = *it;
		std::string fPathS = p.path().string();
		LPCSTR fPath = fPathS.c_str();
		std::cout << fPath << "\n";

		HANDLE hFile = CreateFile(
			fPath,
			GENERIC_READ,
			(FILE_SHARE_READ | FILE_SHARE_WRITE, FILE_SHARE_DELETE),
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

		//std::cout << "file: " << p << ", Last modified timestamp: " << szBuf << std::endl;


		auto image_1 = *it;
		std::string image_1_path = image_1.path().string();
		LPCSTR image_1_path_c = image_1_path.c_str();

		++it;

		auto image_2 = *it;
		std::string image_2_path = image_2.path().string();
		LPCSTR image_2_path_c = image_2_path.c_str();


		af::array I1_color = af::loadImage(image_1_path_c, true);
		af::array I2_color = af::loadImage(image_2_path_c, true);
		af::array I1 = colorSpace(I1_color, AF_GRAY, AF_RGB);
		af::array I2 = colorSpace(I2_color, AF_GRAY, AF_RGB);

		//saveImage("c:\\temp\\out_image.jpg", I1);

		af::array I3 = I1 - I2;

		I3 = af::abs(I3);

		I3 = (I3 < 50.0f) * 0.0f + 255.0f * (I3 > 50.0f);

		I3 /= 255.f;

		//const static int width = 1024, height = 768;
		//Window window(width, height, "Image Viewer");
		//do
		//{
		//	window.image(I3, "Image Titlte");
		//}
		//while (!window.close());
	}


	_tprintf(TEXT(" Directory (%s) changed.\n"), lpDir);
}

image_acquisition::FileLoader::FileLoader(std::string directory, std::string prefix,
                                          image_segmentation::Controller* segmentation_controller)
{
	this->directory = directory;
	this->prefix = prefix;
	this->segmentation_controller = segmentation_controller;

	this->WatchDirectory();
}

void image_acquisition::FileLoader::SetDirectory(std::string directory)
{
	this->directory = directory;
}

void image_acquisition::FileLoader::SetImageSegmentationController(
	image_segmentation::Controller* segmentation_controller)
{
	this->segmentation_controller = segmentation_controller;
}


image_acquisition::FileLoader::~FileLoader()
{
}
