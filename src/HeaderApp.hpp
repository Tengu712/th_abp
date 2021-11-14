#include "../directx/HeaderDX11.hpp"
#pragma comment(lib, "mydx.lib")

class App {
private:
    D3DManager dmanager;

public:
    App();
    bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, unsigned int width,
        unsigned int height, bool windowed);
    bool update();
};
