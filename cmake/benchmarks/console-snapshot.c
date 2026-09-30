#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Acknowledge only this advisory dialog in the benchmark-owned process.
 * Required-extension errors remain visible in the captured startup log.
 */
static BOOL CALLBACK driver_ok(HWND button, LPARAM unused)
{
	char text[16], kind[16];
	(void)unused;
	GetWindowTextA(button, text, sizeof(text));
	GetClassNameA(button, kind, sizeof(kind));
	if (!strcmp(text, "OK") && !strcmp(kind, "Button"))
		PostMessageA(button, BM_CLICK, 0, 0);
	return TRUE;
}

static BOOL CALLBACK driver_warning(HWND window, LPARAM process)
{
	DWORD owner;
	char title[128];
	GetWindowThreadProcessId(window, &owner);
	if (owner != (DWORD)process) return TRUE;
	GetWindowTextA(window, title, sizeof(title));
	if (!strcmp(title, "Warning - Old Video Card Driver"))
		EnumChildWindows(window, driver_ok, 0);
	return TRUE;
}

int main(int argc, char **argv)
{
	if (argc != 3) return 1;
	DWORD pid = strtoul(argv[1], NULL, 10);
	if (!strcmp(argv[2], "--ack-driver-warning")) {
		EnumWindows(driver_warning, pid);
		return 0;
	}
	FreeConsole();
	if (!AttachConsole(pid)) return 2;
	HANDLE h = CreateFileA("CONOUT$", GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	CONSOLE_SCREEN_BUFFER_INFO info;
	if (h == INVALID_HANDLE_VALUE || !GetConsoleScreenBufferInfo(h, &info))
		return 3;
	size_t width = info.dwSize.X, rows = info.dwCursorPosition.Y + 1;
	char *buffer = (char *)malloc(width * rows);
	DWORD got = 0;
	COORD start = {0, 0};
	if (!buffer ||
		!ReadConsoleOutputCharacterA(
			h, buffer, (DWORD)(width * rows), start, &got))
		return 4;
	FILE *log = fopen(argv[2], "wb");
	if (!log) return 5;
	for (size_t row = 0; row * width < got; row++) {
		size_t length = width;
		if ((row + 1) * width > got) length = got - row * width;
		while (length && buffer[row * width + length - 1] == ' ') length--;
		fwrite(buffer + row * width, 1, length, log);
		fputc('\n', log);
	}
	fclose(log);
	free(buffer);
	CloseHandle(h);
	FreeConsole();
	return 0;
}
