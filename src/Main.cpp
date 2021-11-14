#include "HeaderApp.hpp"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPInst, LPSTR pCmd, int cmdShow) {
    App app = App();
    if (!app.init(hInst, cmdShow, L"射命丸文の弾幕稽古", L"TH_ABP", 1280U, 960U,
                MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO))
        return 1;
    MSG msg;
    while (true) {
        if (PeekMessageW(&msg, nullptr, 0U, 0U, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                break;
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
            continue;
        }
        if (app.update())
            break;
    }
    UnregisterClassW(L"TH_ABP", hInst);
    return 0;
}

