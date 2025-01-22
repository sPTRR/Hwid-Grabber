#include <Windows.h>

#include "thread"
#include <string>
#include <iostream>
#include <Shlobj.h>
#include <filesystem>
#include <fstream>

#include <windows.h>
#include <wininet.h>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <wincrypt.h>
#include <iomanip>

#include <windows.h> // For Windows API functions like OpenProcess, GetModuleHandleEx, etc.
#include <TlHelp32.h> // For process and module enumeration (e.g., PROCESSENTRY32, MODULEENTRY32)
#include <string> // For working with strings
#include <iostream> // For debugging with std::cout (optional)
#include <intrin.h>


#include <windows.h>
#include <iostream>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <wininet.h>
#include <string>
#include <ShlObj.h>
#pragma comment(lib, "wininet.lib")



std::string fetch_url_content(const std::string& url) {
    HINTERNET hInternet, hConnect;
    DWORD bytesRead;
    char buffer[4096];
    std::string result;

    hInternet = InternetOpenW(L"WinINet Example", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

    if (hInternet == NULL) {
        std::cerr << "InternetOpen failed with error: " << GetLastError() << std::endl;
        return "";
    }

    hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (hConnect == NULL) {
        std::cerr << "InternetOpenUrlA failed with error: " << GetLastError() << std::endl;
        InternetCloseHandle(hInternet);
        return "";
    }

    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        result.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return result;
}



std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

std::string get_cpu_id() {
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 0);

    std::stringstream ss;
    ss << std::hex << std::setw(8) << std::setfill('0') << CPUInfo[0]
        << std::setw(8) << std::setfill('0') << CPUInfo[1]
        << std::setw(8) << std::setfill('0') << CPUInfo[2]
        << std::setw(8) << std::setfill('0') << CPUInfo[3];

    return ss.str();
}

std::string custom_hash(const std::string& input) {
    uint64_t hash = 0;

    for (size_t i = 0; i < input.size(); ++i) {
        hash ^= (input[i] << (i % 64));
        hash *= 16777619;
    }

    std::stringstream ss;
    ss << std::setw(16) << std::setfill('0') << std::hex << hash;
    return ss.str();
}

std::string get_hwid() {
    std::string cpu_id = get_cpu_id();
    return custom_hash(cpu_id);
}
std::string clean_hwid(const std::string& hwid) {
    std::string clean = trim(hwid);
    std::transform(clean.begin(), clean.end(), clean.begin(), ::toupper); // Ensure the case is the same
    return clean;
}
void SetClipboardText(const std::string& text) {
    // Open the clipboard to allow access
    if (OpenClipboard(NULL)) {
        // Empty the clipboard to remove any old data
        EmptyClipboard();

        // Allocate global memory for the text
        HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(char));
        if (hGlobal == NULL) {
            std::cerr << "GlobalAlloc failed!" << std::endl;
            CloseClipboard();
            return;
        }

        // Lock the global memory so we can write to it
        char* pGlobal = (char*)GlobalLock(hGlobal);
        if (pGlobal == NULL) {
            std::cerr << "GlobalLock failed!" << std::endl;
            GlobalFree(hGlobal);
            CloseClipboard();
            return;
        }

        // Copy the text to the global memory
        strcpy_s(pGlobal, text.size() + 1, text.c_str());

        // Unlock the global memory
        GlobalUnlock(hGlobal);

        // Set the clipboard data to the global memory
        SetClipboardData(CF_TEXT, hGlobal);

        // Close the clipboard
        CloseClipboard();
    }
    else {
        std::cerr << "OpenClipboard failed!" << std::endl;
    }
}

int main() {
    std::string hwid = get_hwid();
    std::string cleanHwid = clean_hwid(hwid);
    std::cout << "Hwid: " << cleanHwid << std::endl;
    SetClipboardText(cleanHwid);
    std::cout << "Hwid Copied To ClipBoard" << std::endl;
  
    while (true) {

    }
}
