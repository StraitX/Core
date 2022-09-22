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


static bool IsExtension(StringView ext) {
    return ext.Size();
}

static std::wstring ToCommonFileDialogType(Span<StringView> extensions) {
    std::wstring type;
    for (auto ext : extensions) {
        type += L"*." + Windows::Utf8ToWstr(ext) + L';';
    }
    return type;
}

String Dialog::SelectFile(StringView title, Span<StringView> extensions) {
    if (!SUCCEEDED(::CoInitializeEx(NULL, ::COINIT_APARTMENTTHREADED)))
        return {};
    
    String path;

    IFileDialog *pfd;
    if (SUCCEEDED(CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd))))
    {

        pfd->SetTitle(Windows::Utf8ToWstr(title).c_str());
        
        std::wstring file_type = ToCommonFileDialogType(extensions);
        COMDLG_FILTERSPEC spec = {
            L"Desired",
            file_type.c_str()
        };
        pfd->SetFileTypes(1, &spec);

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
