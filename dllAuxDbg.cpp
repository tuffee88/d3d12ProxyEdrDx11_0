#include <shlobj.h>
#include <strsafe.h>
#include <string>


/*Slightly modified sample code for PIX Capture support from: https://devblogs.microsoft.com/pix/taking-a-capture/ */

std::wstring GetLatestWinPixGpuCapturerPath(bool* status)
{
    LPWSTR programFilesPath = nullptr;
    SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

    std::wstring pixSearchPath = programFilesPath + std::wstring(L"\\Microsoft PIX\\*");

    WIN32_FIND_DATA findData;
    bool foundPixInstallation = false;
    wchar_t newestVersionFound[MAX_PATH];

    HANDLE hFind = FindFirstFile(pixSearchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) &&
                (findData.cFileName[0] != '.'))
            {
                if (!foundPixInstallation || wcscmp(newestVersionFound, findData.cFileName) <= 0)
                {
                    foundPixInstallation = true;
                    StringCchCopy(newestVersionFound, _countof(newestVersionFound), findData.cFileName);
                }
            }
        } while (FindNextFile(hFind, &findData) != 0);
    }

    FindClose(hFind);
    wchar_t output[MAX_PATH] =L"";


    if (!foundPixInstallation)
    {
        *status = false;
    }
    else
    {
        *status = true;

        StringCchCopy(output, pixSearchPath.length(), pixSearchPath.data());
        StringCchCat(output, MAX_PATH, &newestVersionFound[0]);
        StringCchCat(output, MAX_PATH, L"\\WinPixGpuCapturer.dll");
    }
      
    return &output[0];
}

/* Helper function to be called by from main file to enable Pix runtime attachement support*/
void enablePixRuntimeAttach()
{
    if (GetModuleHandle(L"WinPixGpuCapturer.dll") == 0)
    {
        bool WinPixSearchResult = false;
        OutputDebugString(L"D3D12CreateDevice - DEBUG: Attempt to load Pix support!\n");

        std::wstring WinPixPath = GetLatestWinPixGpuCapturerPath(&WinPixSearchResult);

        if (WinPixSearchResult)
        {
            OutputDebugString(L"D3D12CreateDevice - DEBUG: Loading Pix WinPixGpuCapturer.dll\n");
            LoadLibrary(WinPixPath.c_str());
        }
        else
        {
            OutputDebugString(L"D3D12CreateDevice - DEBUG: Pix WinPixGpuCapturer.dll not found, no extended Pix support available\n");
        }
    }
}