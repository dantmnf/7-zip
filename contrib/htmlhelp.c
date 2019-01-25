#include <windows.h>

typedef HWND (__stdcall *FuncHtmlHelpW)(HWND hwndCaller, LPCWSTR pszFile, UINT uCommand, DWORD_PTR dwData);
typedef HWND (__stdcall *FuncHtmlHelpA)(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData);

static HMODULE hhctrl;

static HMODULE load() {
    if(hhctrl) return hhctrl;
    hhctrl = LoadLibraryA("hhctrl.ocx");
    return hhctrl;
}

HWND __stdcall HtmlHelpW(HWND hwndCaller, LPCWSTR pszFile, UINT uCommand, DWORD_PTR dwData) {
    if(load())
        return ((FuncHtmlHelpW)GetProcAddress(hhctrl, "HtmlHelpW"))(hwndCaller, pszFile, uCommand, dwData);
    return NULL;
}

HWND __stdcall HtmlHelpA(HWND hwndCaller, LPCSTR pszFile, UINT uCommand, DWORD_PTR dwData) {
    if(load())
        return ((FuncHtmlHelpA)GetProcAddress(hhctrl, "HtmlHelpA"))(hwndCaller, pszFile, uCommand, dwData);
    return NULL;
}
