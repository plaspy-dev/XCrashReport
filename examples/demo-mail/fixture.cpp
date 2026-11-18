#include <windows.h>
#include <mapi.h>
#include <cstring>
#include <cstdio>

// Synthetic-mail provider for an explicitly selected test build.
// Only accept the exact demo message and ZIP. SMTP runs in the test harness.
extern "C" ULONG WINAPI CaptureDemoMail(LHANDLE, ULONG_PTR, lpMapiMessage message, FLAGS, ULONG)
{
    if (!message || message->nRecipCount != 1 || !message->lpRecips ||
        message->nFileCount != 1 || !message->lpFiles ||
        !message->lpszSubject || !message->lpszNoteText) {
        return MAPI_E_INVALID_MESSAGE;
    }

    const char* address = message->lpRecips[0].lpszAddress;
    if (!address || (_stricmp(address, "reports@example.invalid") != 0 &&
                     _stricmp(address, "SMTP:reports@example.invalid") != 0) ||
        strcmp(message->lpszSubject, "Reporte de Error de DemoApp.exe") != 0 ||
        !strstr(message->lpszNoteText, "DEMO-104")) {
        return MAPI_E_UNKNOWN_RECIPIENT;
    }

    const char* attachment = message->lpFiles[0].lpszPathName;
    const char* filename = attachment ? strrchr(attachment, '\\') : nullptr;
    char fixture[MAX_PATH];
    const DWORD size = GetEnvironmentVariableA("XCRASHREPORT_DEMO_DIR", fixture, MAX_PATH);
    if (!filename || _stricmp(filename + 1, "DemoApp.zip") != 0 ||
        size == 0 || size + sizeof("\\message.txt") >= MAX_PATH) {
        return MAPI_E_ATTACHMENT_OPEN_FAILURE;
    }

    char output[MAX_PATH];
    sprintf_s(output, "%s\\report.zip", fixture);
    if (!CopyFileA(attachment, output, TRUE)) {
        return MAPI_E_ATTACHMENT_OPEN_FAILURE;
    }

    FILE* written = nullptr;
    sprintf_s(output, "%s\\message.txt", fixture);
    if (fopen_s(&written, output, "wb") != 0) {
        return MAPI_E_DISK_FULL;
    }
    const int count = fprintf(written, "%s\n%s\n", message->lpszSubject, message->lpszNoteText);
    const int closed = fclose(written);
    return count > 0 && closed == 0 ? SUCCESS_SUCCESS : MAPI_E_DISK_FULL;
}
