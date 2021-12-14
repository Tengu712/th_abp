#include "_app.hpp"

int WINAPI WinMain(HINSTANCE h_inst, HINSTANCE h_pinst, LPSTR p_cmd, int cmd_show) {
    App app = App();
    if (!app.init(h_inst, p_cmd, cmd_show))
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
        if (app.isIconic())
            Sleep(16);
        else if (app.update())
            break;
    }
    return 0;
}
