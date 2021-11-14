#include "HeaderApp.hpp"

App::App() : dmanager(D3DManager()) {}

bool App::init(HINSTANCE hInst, int cmdShow)
{
    bool res = true;
    res = res && dmanager.init(hInst, cmdShow,
            L"射命丸文の弾幕稽古", L"TH_ABP", 1280U, 960U,
            MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO);
    return res;
}

bool App::update() {
    dmanager.drawBegin(nullptr);
    dmanager.drawEnd();
    return false;
}
