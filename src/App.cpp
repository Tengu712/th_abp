#include "../include/HeaderApp.hpp"

App::App() : dmanager(D3DManager()) {}

bool App::init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, unsigned int width,
        unsigned int height, bool windowed)
{
    bool res = true;
    res = res && dmanager.init(hInst, cmdShow, wndName, wndClassName, width, height, windowed);
    return res;
}

bool App::update() {
    dmanager.drawBegin(nullptr);
    dmanager.drawEnd();
    return false;
}
