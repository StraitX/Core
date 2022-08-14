#include <shlobj.h>
#include "core/os/dialog.hpp"
#include "platform/windows/wchar.hpp"

String Dialog::SelectFolder(StringView title) {

    if (!SUCCEEDED(::CoInitializeEx(NULL, ::COINIT_APARTMENTTHREADED)))
        return {};
    
    String path;

    IFileDialog *pfd;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
    {
        DWORD dwOptions;
        if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
        {
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }

        pfd->SetTitle(Windows::Utf8ToWstr(title).c_str());

        if (SUCCEEDED(pfd->Show(NULL)))
        {
            IShellItem *psi;
            if (SUCCEEDED(pfd->GetResult(&psi)))
            {
                LPWSTR wpath = nullptr;
                psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &wpath);

                if (wpath)
                    path = Windows::WPathToUtf8(wpath);

                CoTaskMemFree(wpath);
                psi->Release();
            }
        }
        pfd->Release();
    }

    CoUninitialize();

    return path;
}