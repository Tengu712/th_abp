#include "../directx/HeaderDX11.hpp"
#pragma comment(lib, "mydx.lib")

class App {
private:
    D3DManager dmanager;

public:
    App();
    bool init(HINSTANCE hInst, int cmdShow);
    bool update();
};
