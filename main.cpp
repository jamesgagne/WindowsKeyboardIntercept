#include <Windows.h>
#include <map>
#include <iostream>
std::map<int, bool> keyState;

// Hook callback function
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if(nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
        keyState[pKeyBoard->vkCode] = wParam == WM_KEYDOWN;
    }
    // if (nCode == HC_ACTION && wParam == WM_KEYDOWN) {
    //     KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;

    //     // Check if 'A' key is pressed
    //     if (pKeyBoard->vkCode == 0x41) { // 0x41 is the virtual key code for 'A'
    //         // Simulate 'B' key press instead
    //         INPUT input = { 0 };
    //         input.type = INPUT_KEYBOARD;
    //         input.ki.wVk = 0x42; // Virtual key code for 'B'
    //         SendInput(1, &input, sizeof(INPUT));

    //         // Block the original 'A' key press
    //         return 1;
    //     }
    // }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}
int startHook() {
     // Install the keyboard hook
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, NULL, 0);
    if (hHook == NULL) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    // Message loop to keep the hook alive
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        std::cout << "Key pressed: " << msg.wParam << std::endl;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninstall the hook
    UnhookWindowsHookEx(hHook);
    return 0;
}

int main() {
  startHook();
}
