#include "FileLoader.h"
#include <string>
#include <bemapiset.h>
#include <tchar.h>
#include <filesystem>
#include <arrayfire.h>
#include <strsafe.h>
#include <set>
#include "dto/Image.h"

namespace fs = std::experimental::filesystem;

void image_acquisition::FileLoader::WatchDirectory()
{
	// Watch Directory
	LPCTSTR lpDir = this->directory.c_str();

	// Extract Drive
	TCHAR lpDrive[4];
	TCHAR lpFile[_MAX_FNAME];
	TCHAR lpExt[_MAX_EXT];
	_tsplitpath_s(lpDir, lpDrive, 4, nullptr, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);
	lpDrive[2] = (TCHAR)'\\';
	lpDrive[3] = (TCHAR)'\0';

	// Watch directory for file creations
	HANDLE dwChangeHandle = FindFirstChangeNotification(
		lpDir, // directory to watch 
		FALSE, // do not watch subtree 
		FILE_NOTIFY_CHANGE_FILE_NAME); // watch file name changes 

	// Error Handling
	if (dwChangeHandle == INVALID_HANDLE_VALUE)
	{
		printf("\n ERROR: FileLoader:s FindFirstChangeNotification function failed.\n");
		ExitProcess(GetLastError());
	}
	if (dwChangeHandle == nullptr)
	{
		printf("\n ERROR: FileLoader: Unexpected NULL from FindFirstChangeNotification.\n");
		ExitProcess(GetLastError());
	}

	// Wait for file changes
	while (true)
	{
		const DWORD dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE);

		switch (dwWaitStatus)
		{
		case WAIT_OBJECT_0:

			// Handle files in directory
			this->ProcessFiles();

			// Error handling
			if (FindNextChangeNotification(dwChangeHandle) == FALSE)
			{
				printf("\n ERROR: FileLoader: FindNextChangeNotification function failed.\n");
				ExitProcess(GetLastError());
			}
			break;

		default:
			printf("\n ERROR: FileLoader: Unhandled dwWaitStatus.\n");
			ExitProcess(GetLastError());
		}
	}
}

void image_acquisition::FileLoader::ProcessFiles()
{
	// Directory to process files in
	LPCSTR lpDir = this->directory.c_str();

	// Loop through files in directory
	const auto di = fs::directory_iterator(lpDir);
	const auto begin = fs::begin(di);
	const auto end = fs::end(di);
	for (auto it = begin; begin != end; ++it)
	{
		// Read full file path
		auto p = *it;
		std::string fPathS = p.path().string();
		LPCSTR fPath = fPathS.c_str();
		//std::cout << "comparing " << fPath << " with " << this->path_prefix_c << "\n";
		if (isPrefix(fPath, this->path_prefix_c))
		{
			std::cout << "Processing " << fPath << "\n";
		}
		else
		{
			std::cout << "Skipping " << fPath << "\n";
			continue;
		}

		// Create file handler to read last modified date
		const HANDLE hFile = CreateFile(
			fPath,
			GENERIC_READ,
			(FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE),
			nullptr,
			OPEN_EXISTING,
			0,
			nullptr
		);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("Error: FileLoader: CreateFile failed for %s with %d, retry during next execution\n", fPath, GetLastError());
			continue;
		}

		// Retrieve the file times for the file.
		FILETIME ftCreate, ftAccess, ftWrite;
		SYSTEMTIME stUTC, stLocal;
		if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		{
			printf("GetFileTime failed for %s with error:  %d, retry during next execution\n", fPath, GetLastError());
		}

		// Convert the last-write time to local time.
		FileTimeToSystemTime(&ftWrite, &stUTC);
		SystemTimeToTzSpecificLocalTime(nullptr, &stUTC, &stLocal);

		//// Build a string showing the date and time.
		//TCHAR szBuf[MAX_PATH];
		//DWORD dwRet = StringCchPrintf(szBuf, MAX_PATH,
		//                              TEXT("  FileTime: %02d/%02d/%d  %02d:%02d:%02d.%03d"),
		//                              stLocal.wMonth, stLocal.wDay, stLocal.wYear,
		//                              stLocal.wHour, stLocal.wMinute, stLocal.wSecond, stLocal.wMilliseconds);
		//printf("%s\n", szBuf);

		CloseHandle(hFile);

		// Load Image for further processing
		auto image_1 = *it;
		std::string image_1_path = image_1.path().string();
		LPCSTR image_1_path_c = image_1_path.c_str();
		af::array I1_color = af::loadImage(image_1_path_c, true);

		// Extract FileName
		std::string filename = this->extract_filename(image_1_path_c);

		dto::Image image;
		image.filename = filename;
		image.af_image_color = I1_color;
		image.path = image_1_path;
		image.filetime = stLocal;
		//Process image
		//this->segmentation_controller->ProcessImage(&stLocal, I1_color, image_1_path, filename);
		this->segmentation_controller->ProcessImage(image);

		//af::array I1 = colorSpace(I1_color, AF_GRAY, AF_RGB);
		//af::array I2 = colorSpace(I2_color, AF_GRAY, AF_RGB);
		//
		////saveImage("c:\\temp\\out_image.jpg", I1);
		//
		//af::array I3 = I1 - I2;
		//
		//I3 = af::abs(I3);
		//
		//I3 = (I3 < 50.0f) * 0.0f + 255.0f * (I3 > 50.0f);
		//
		//I3 /= 255.f;

		//const static int width = 1024, height = 768;
		//Window window(width, height, "Image Viewer");
		//do
		//{
		//	window.image(I3, "Image Titlte");
		//}
		//while (!window.close());
	}
}

image_acquisition::FileLoader::FileLoader(std::string directory, std::string prefix,
	image_segmentation::Controller* segmentation_controller)
{
	this->directory = directory;

	this->prefix = prefix;
	this->prefix_c = prefix.c_str();

	this->path_prefix = directory + prefix;
	this->path_prefix_c = path_prefix.c_str();

	this->segmentation_controller = segmentation_controller;
}

image_acquisition::FileLoader::FileLoader(dto::Camera camera, image_segmentation::Controller* segmentation_controller)
{
	this->camera = camera;

	this->directory = camera.directory;

	this->prefix = camera.prefix;
	this->prefix_c = this->prefix.c_str();

	this->path_prefix = camera.directory + camera.prefix;
	this->path_prefix_c = this->path_prefix.c_str();

	this->segmentation_controller = segmentation_controller;
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

bool image_acquisition::FileLoader::isPrefix(const char* s1, const char* s2)
{
	while (*s1 && *s2)
		if (*s1++ != *s2++)
			return false;
	return true;
}


image_acquisition::FileLoader::~FileLoader()
{
}

std::string image_acquisition::FileLoader::extract_filename(char const* path_c)
{
	const std::set<char> delimiters = {'\\'};

	std::vector<std::string> ret;

	char const* start = path_c;
	for (; *path_c; ++path_c)
	{
		if (delimiters.find(*path_c) != delimiters.end())
		{
			if (start != path_c)
			{
				std::string str(start, path_c);
				ret.push_back(str);
			}
			else
			{
				ret.push_back("");
			}
			start = path_c + 1;
		}
	}
	ret.push_back(start);

	return ret.back();
}
