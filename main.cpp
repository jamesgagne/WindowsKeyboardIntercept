#include <Windows.h>
#include <map>
#include <iostream>

std::map<int, bool> keyState;
std::map<int, bool> prevState;
const int VK_A = 65;
const int VK_I = 73;
const int VK_K = 74;
const int VK_J = 75;
const int VK_L = 76;

void moveMouse(int dx, int dy) {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    SetCursorPos(cursorPos.x + dx, cursorPos.y + dy);
}
void scrollMouse(int dx, int dy) {
    INPUT input = { 0 };
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.mouseData = dy;
    SendInput(1, &input, sizeof(INPUT));
}
/**
 * Handle input
 * LCTRL + RSHIFT + A will quit the program and stop it from running
 * @return 1 will block the input, 0 will not block the input
 */
 int handleInput() {
        if (keyState[VK_LCONTROL] && keyState[VK_RSHIFT] && keyState[VK_A]) {
            PostQuitMessage(0); // Post a quit message to break out of the message loop
            return 1;
        }
        int modifier = 2;
        int speed=10;
         if (keyState[VK_LCONTROL]) {
            if(keyState[VK_I] || keyState[VK_K] || keyState[VK_J] || keyState[VK_L]) {
                if(keyState[VK_LSHIFT]) {
                    modifier = 1;
                }
                if(keyState[VK_I]) {
                    moveMouse(0, 0 - (speed * modifier)); // Move mouse up
                }
                if(keyState[VK_K]) {
                    moveMouse(0 - (speed * modifier), 0); // Move mouse down
                }
                if(keyState[VK_J]) {
                    moveMouse(0, speed * modifier); // Move mouse left
                }
                if(keyState[VK_L]) { 
                    moveMouse(speed * modifier, 0); // Move mouse right
                }
                return 1;
            }
            if(keyState[13] && !prevState[13]){
                //send a mouse left down
                // std::cout << "Mouse left click" << std::endl;
                INPUT input = { 0 };
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
                SendInput(1, &input, sizeof(INPUT));
                return 1;
            }
            if(!keyState[13] && prevState[13]){
                //send a mouse left up
                // std::cout << "Mouse left click" << std::endl;
                INPUT input = { 0 };
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
                SendInput(1, &input, sizeof(INPUT));
                return 1;
            }
            if(keyState[VK_SPACE]){
                //send a mouse right down
                INPUT input = { 0 };
                input.type = INPUT_MOUSE;
                 input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
                SendInput(1, &input, sizeof(INPUT));
                return 1;
            }
            if(!keyState[VK_SPACE] && prevState[VK_SPACE]){
                //send a mouse right up
                INPUT input = { 0 };
                input.type = INPUT_MOUSE;
                input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
                SendInput(1, &input, sizeof(INPUT));
                return 1;
            }
         }
        return 0;
}
// Hook callback function
LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {

    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
        prevState = keyState;
        if (wParam == WM_KEYDOWN) {
            keyState[pKeyBoard->vkCode] = true;
            // std::cout << "Key pressed: " << pKeyBoard->vkCode << std::endl;
        } else if (wParam == WM_KEYUP) {
            keyState[pKeyBoard->vkCode] = false;
            // std::cout << "Key released: " << pKeyBoard->vkCode << std::endl;
        }
        if(handleInput()){
            return 1;
        }
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
    std::cout << "Hook installed!" << std::endl;
    if (hHook == NULL) {
        std::cerr << "Failed to install hook!" << std::endl;
        return 1;
    }

    // Message loop to keep the hook alive
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Uninstall the hook
    UnhookWindowsHookEx(hHook);
    return 0;
}

int main() {
    return startHook();
}