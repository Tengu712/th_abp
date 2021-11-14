#include "HeaderApp.hpp"
#include <stdio.h>

int str_cmp(char *p1, char *p2) {
    for(; *p1 == *p2; p1++, p2++) {
        if (*p1 == '\0')
            return 0;
    }
    return *p1 - *p2;
}

bool App::init(HINSTANCE hInst, LPSTR pCmd, int cmdShow) {
    // Start up
    const bool kWindowed = MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO;
    if (kWindowed && str_cmp(pCmd, "debug") == 0 && !createConsole())
        return false;
    debug("\n==================================================\n");
    debug("            \"Aya's Bullethell Practice\"\n");
    debug("           SkyDog, Asoc. Tengu712 (2021) \n");
    debug("==================================================\n\n");
    debug("Start up ...\n");

    // Initialize API
    if (!dmanager.init(hInst, cmdShow, L"射命丸文の弾幕稽古", L"TH_ABP", 1280U, 960U, kWindowed)) {
        debug(" - Direct3D11 : Failed\n");
        return false;
    }
    debug(" - Direct3D11 : Success\n");
    if (!imanager.init(64)) {
        debug(" - XInput : Failed\n");
        return false;
    }
    debug(" - XInput : Success\n");
    // Initialize App
    pScene = new SceneTitle(this);
    // Finish
    debug("\nAll initializations succeeded.\nWelcome Bullet-Hell!\n\n");
    return true;
}

bool App::update() {
    imanager.inspect();
    dmanager.drawBegin(nullptr);
    pScene->update();
    dmanager.drawEnd();
    return false;
}

bool App::createConsole() {
    if (!AllocConsole())
        return false;
    if (freopen("CONOUT$", "w", stdout) == nullptr)
        return false;
    ableDebug = true;
    return true;
}

void App::debug(const char* msg) {
    if (ableDebug)
        printf(msg);
}
