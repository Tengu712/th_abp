#include "HeaderApp.hpp"

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
    debug("      SkyDog Assoc of WordSpiritism, Tengu712 \n");
    debug("==================================================\n\n");
    debug("Start up ...\n");

    try {
        // Initialize API
        if (!dmanager.init(hInst, cmdShow, L"射命丸文の弾幕稽古", L"TH_ABP", 1280U, 960U, kWindowed))
            throw "Failed to initialize D3DManager.";
        debug(" - Direct3D11 : Success\n");
        if (!imanager.init(64))
            throw "Failed to initialize InputManager.";
        debug(" - XInput : Success\n");
        // Create idea
        idea.numIdx = 6U;
        struct Vertex dataPCNU[4U] = {
            {-0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
            {-0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
            {+0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
            {+0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        };
        unsigned int dataIdx[6U] = {0, 1, 2, 0, 2, 3};
        if (!dmanager.createModelBuffers(idea.numIdx, dataPCNU, dataIdx, &idea)) 
            throw "Failed to create idea.";
        debug(" - Idea : Success\n");
        // Initialize scene
        pScene = new SceneTitle(this);
        if (!pScene->init())
            throw "Failed to initialize title.";
        debug(" - Title : Success\n");
        // Finish
        debug("\nAll initializations succeeded.\nWelcome Bullet-Hell!\n\n");
    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }
    return true;
}

bool App::update() {
    imanager.inspect();
    dmanager.drawBegin(nullptr);
    pScene->update();
    dmanager.drawEnd();
    return false;
}

void App::drawIdea() {
    dmanager.drawModel(&idea);
}

void App::applyFact(Fact* pFact) {
    idea.posX = pFact->posX;
    idea.posY = pFact->posY;
    idea.posZ = pFact->posZ;
    idea.degX = pFact->degX;
    idea.degY = pFact->degY;
    idea.degZ = pFact->degZ;
    idea.sclX = pFact->sclX;
    idea.sclY = pFact->sclY;
    idea.sclZ = pFact->sclZ;
    idea.colR = pFact->colR;
    idea.colG = pFact->colG;
    idea.colB = pFact->colB;
    idea.colA = pFact->colA;
}

void App::createCamera(float width, float height, Camera* pCamera) {
    dmanager.createCamera(width, height, pCamera);
}

void App::applyCamera(Camera* pCamera, bool parse) {
    dmanager.applyCamera(pCamera, parse);
}

#include <stdio.h>

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

void App::debug(const int msg) {
    if (ableDebug)
        printf("%d", msg);
}
