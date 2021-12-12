#include "../directx/_dx11private.hpp"
#include "_app.hpp"

struct AppInf {
    D3DManager dmanager;
    InputManager imanager;
    bool ableDebug;
    ModelInf idea;
    Scene* pScene;
    AppInf() : dmanager(D3DManager()), imanager(InputManager()), ableDebug(false), idea(ModelInf()), pScene(nullptr) {
    }
    ~AppInf() {
        if (pScene != nullptr)
            delete pScene;
    }
};

int str_cmp(char* p1, char* p2) {
    for (; *p1 == *p2; p1++, p2++) {
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
        debug(" - Title Scene : Success\n");
        HMODULE h_module = LoadLibraryA("./resource.dll");
        if (h_module == nullptr)
            throw "Failed to load resource.dll.";
        Image img_load = Image();
        memset(&img_load, 0, sizeof(Image));
        if (!pInf->dmanager.createImage(h_module, IMG_BG_LOAD, &img_load))
            throw "Failed to load an image.";
        pInf->dmanager.applyImage(&img_load);
        Model model_load = Model();
        model_load.sclX = 1280.0f;
        model_load.sclY = 960.0f;
        applyModel(&model_load);
        imgs = new Image[kNumImage];
        if (imgs == nullptr)
            throw "Failed to create array of images.";
        memset(imgs, 0, sizeof(Image) * kNumImage);
        auto loadImage = [&](unsigned int id) {
            for (int i = 0; i < kNumImage; ++i) {
                if (imgs[i].id != 0U)
                    continue;
                return pInf->dmanager.createImage(h_module, id, &imgs[i]);
            }
            return false;
        };
        bool flg = true;
        flg = flg && loadImage(IMG_BG_TITLE);
        if (!flg)
            throw "Failed to load some images.";
        debug(" - Images : Success\n");
        cameraUI.posZ = -10.0f;
        cameraUI.parse = false;
        debug("\nAll initializations succeeded.\nWelcome Bullet-Hell!\n\n");
    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }
    return true;
}

bool App::isIconic() {
    return pInf->dmanager.isIconic();
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

void App::applyModel(Model* pFact) {
    if (pFact == nullptr)
        return;
    pInf->dmanager.setMatrixScale(pFact->sclX, pFact->sclY, pFact->sclZ);
    pInf->dmanager.setMatrixRotate(pFact->degX, pFact->degY, pFact->degZ);
    pInf->dmanager.setMatrixTranslate(pFact->posX, pFact->posY, pFact->posZ);
    pInf->dmanager.setVectorColor(pFact->colR, pFact->colG, pFact->colB, pFact->colA);
}

void App::applyCamera(Camera* pCamera) {
    if (pCamera == nullptr) {
        pInf->dmanager.enableDepthStencil(false);
        applyCamera(&cameraUI);
        return;
    }
    pInf->dmanager.setMatrixView(pCamera->posX, pCamera->posY, pCamera->posZ, pCamera->dirX, pCamera->dirY,
        pCamera->dirZ, pCamera->uppX, pCamera->uppY, pCamera->uppZ);
    pInf->dmanager.setMatrixProject(
        pCamera->width, pCamera->height, pCamera->angle, pCamera->nearZ, pCamera->farZ, pCamera->parse);
}

void App::applyImage(unsigned int id) {
    for (int i = 0; i < kNumImage; ++i) {
        if (imgs[i].id == id)
            return pInf->dmanager.applyImage(&imgs[i]);
    }
    pInf->dmanager.applyImage(nullptr);
}

FrameBuffer* App::createFrameBuffer(unsigned int width, unsigned int height) {
    FrameBuffer* res = new FrameBuffer();
    pInf->dmanager.createFrameBuffer(width, height, res);
    return res;
}

void App::applyFrameBuffer(FrameBuffer* pFBuffer) {
    pInf->dmanager.drawBegin(pFBuffer);
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
