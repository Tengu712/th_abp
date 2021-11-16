#include "_app.hpp"
#include "../directx/_dx11private.hpp"

struct AppInf {
    D3DManager dmanager;
    InputManager imanager;
    bool ableDebug;
    ModelInf idea;
    Scene* pScene;
    AppInf() :
        dmanager(D3DManager()),
        imanager(InputManager()),
        ableDebug(false),
        idea(ModelInf()),
        pScene(nullptr)
    {}
    ~AppInf() {
        if (pScene != nullptr)
            delete pScene;
    }
};

App::App() : pInf(nullptr) {}

App::~App() {
    if (pInf != nullptr)
        delete pInf;
}

int str_cmp(char *p1, char *p2) {
    for(; *p1 == *p2; p1++, p2++) {
        if (*p1 == '\0')
            return 0;
    }
    return *p1 - *p2;
}

bool App::init(HINSTANCE hInst, LPSTR pCmd, int cmdShow) {
    pInf = new AppInf();
    if (pInf == nullptr)
        return false;

    const bool kWindowed = MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO;
    if (kWindowed && str_cmp(pCmd, "debug") == 0 && !createConsole()) 
        return false;

    debug("\n==================================================\n");
    debug("            \"Aya's Bullethell Practice\"\n");
    debug("      SkyDog Assoc of WordSpiritism, Tengu712 \n");
    debug("==================================================\n\n");
    debug("Start up ...\n");

    try {
        if (!pInf->dmanager.init(hInst, cmdShow, L"射命丸文の弾幕稽古", L"TH_ABP", 1280U, 960U, kWindowed))
            throw "Failed to initialize D3DManager.";
        debug(" - Direct3D11 : Success\n");
        if (!pInf->imanager.init(64))
            throw "Failed to initialize InputManager.";
        debug(" - XInput : Success\n");
        pInf->idea.numIdx = 6U;
        struct Vertex dataPCNU[4U] = {
            {-0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
            {-0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
            {+0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
            {+0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        };
        unsigned int dataIdx[6U] = {0, 1, 2, 0, 2, 3};
        if (!pInf->dmanager.createModelBuffers(pInf->idea.numIdx, dataPCNU, dataIdx, &pInf->idea)) 
            throw "Failed to create pInf->idea.";
        debug(" - Idea : Success\n");
        pInf->pScene = new SceneTitle(this);
        if (!pInf->pScene->init())
            throw "Failed to initialize title.";
        debug(" - Title : Success\n");
        debug("\nAll initializations succeeded.\nWelcome Bullet-Hell!\n\n");
    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }
    return true;
}

bool App::update() {
    pInf->imanager.inspect();
    pInf->dmanager.drawBegin(nullptr);
    pInf->pScene->update();
    pInf->dmanager.drawEnd();
    return false;
}

void App::drawIdea() {
    pInf->dmanager.drawModel(&pInf->idea);
}

void App::applyFact(Fact* pFact) {
    pInf->idea.posX = pFact->posX;
    pInf->idea.posY = pFact->posY;
    pInf->idea.posZ = pFact->posZ;
    pInf->idea.degX = pFact->degX;
    pInf->idea.degY = pFact->degY;
    pInf->idea.degZ = pFact->degZ;
    pInf->idea.sclX = pFact->sclX;
    pInf->idea.sclY = pFact->sclY;
    pInf->idea.sclZ = pFact->sclZ;
    pInf->idea.colR = pFact->colR;
    pInf->idea.colG = pFact->colG;
    pInf->idea.colB = pFact->colB;
    pInf->idea.colA = pFact->colA;
}

void App::createCamera(float width, float height, Camera* pCamera) {
    pInf->dmanager.createCamera(width, height, pCamera);
}

void App::applyCamera(Camera* pCamera, bool parse) {
    pInf->dmanager.applyCamera(pCamera, parse);
}

#include <stdio.h>

bool App::createConsole() {
    if (!AllocConsole())
        return false;
    if (freopen("CONOUT$", "w", stdout) == nullptr)
        return false;
    pInf->ableDebug = true;
    return true;
}

void App::debug(const char* msg) {
    if (pInf->ableDebug)
        printf(msg);
}

void App::debug(const int msg) {
    if (pInf->ableDebug)
        printf("%d", msg);
}
