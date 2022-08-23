#include <windows.h>

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    PROCESS_INFORMATION pi;
    STARTUPINFOW si;

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    memset(&pi, 0, sizeof(pi));

    if (!CreateProcessW(L"ServerMonitor.exe", L"ServerMonitor.exe -shardmonitor", NULL, NULL, 0, 0, NULL, NULL, &si, &pi))
    {
        MessageBoxW(NULL, L"Error spawning ServerMonitor.exe -shardmonitor", L"Error", MB_OK);
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 0;
}
