#include "../directx/_dx11private.hpp"
#include "_app.hpp"

struct AppInf {
    D3DManager dmanager;
    InputManager imanager;
    bool is_debug;
    ModelInf idea;
    Camera cmr_ui;
    Image* imgs;
    Scene* p_scene;
    AppInf() :
        dmanager(D3DManager()),
        imanager(InputManager()),
        is_debug(false),
        idea(ModelInf()),
        cmr_ui(Camera()),
        imgs(nullptr),
        p_scene(nullptr) {
    }
    ~AppInf() {
        if (imgs != nullptr)
            delete imgs;
        if (p_scene != nullptr)
            delete p_scene;
    }
};

int compareString(char* p1, char* p2) {
    for (; *p1 == *p2; ++p1, ++p2) {
        if (*p1 == '\0')
            return 0;
    }
    return *p1 - *p2;
}

bool App::init(HINSTANCE h_inst, LPSTR p_cmd, int cmd_show) {
    p_inf = new AppInf();
    if (p_inf == nullptr)
        return false;

    const bool kWindowed = MessageBoxW(nullptr, L"フルスクリーンで起動しますか", L"確認", MB_YESNO) == IDNO;
    if (kWindowed && compareString(p_cmd, "debug") == 0 && !createConsole())
        return false;

    debug("\n==================================================\n");
    debug("            \"Aya's Bullethell Practice\"\n");
    debug("      SkyDog Assoc of WordSpiritism, Tengu712 \n");
    debug("==================================================\n\n");
    debug("Start up ...\n");

    try {
        if (!p_inf->dmanager.init(h_inst, cmd_show, L"射命丸文の弾幕稽古", L"TH_ABP", 1280U, 960U, kWindowed))
            throw "Failed to initialize D3DManager.";
        debug(" - Direct3D11 : Success\n");
        if (!p_inf->imanager.init(64))
            throw "Failed to initialize InputManager.";
        debug(" - XInput : Success\n");
        p_inf->idea.num_idx = 6U;
        struct Vertex data_pcnu[4U] = {
            {-0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f},
            {-0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f},
            {+0.5f, +0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f},
            {+0.5f, -0.5f, +0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f},
        };
        unsigned int data_indx[6U] = {0, 1, 2, 0, 2, 3};
        if (!p_inf->dmanager.createModelBuffers(p_inf->idea.num_idx, data_pcnu, data_indx, &p_inf->idea))
            throw "Failed to create p_inf->idea.";
        debug(" - Idea : Success\n");
        p_inf->p_scene = new SceneTitle(this);
        if (!p_inf->p_scene->init())
            throw "Failed to initialize title.";
        debug(" - Title Scene : Success\n");
        HMODULE h_module = LoadLibraryA("./resource.dll");
        if (h_module == nullptr)
            throw "Failed to load resource.dll.";
        Image img_load = Image();
        memset(&img_load, 0, sizeof(Image));
        if (!p_inf->dmanager.createImage(h_module, IMG_BG_LOAD, &img_load))
            throw "Failed to load an image.";
        p_inf->dmanager.applyImage(&img_load);
        Model model_load = Model();
        model_load.scl_x = 1280.0f;
        model_load.scl_y = 960.0f;
        applyModel(&model_load);
        p_inf->imgs = new Image[kNumImage];
        if (p_inf->imgs == nullptr)
            throw "Failed to create array of images.";
        memset(p_inf->imgs, 0, sizeof(Image) * kNumImage);
        auto loadImage = [&](unsigned int id) {
            for (int i = 0; i < kNumImage; ++i) {
                if (p_inf->imgs[i].id != 0U)
                    continue;
                return p_inf->dmanager.createImage(h_module, id, &p_inf->imgs[i]);
            }
            return false;
        };
        bool flg = true;
        flg = flg && loadImage(IMG_BG_TITLE);
        if (!flg)
            throw "Failed to load some images.";
        debug(" - Images : Success\n");
        p_inf->cmr_ui.pos_z = -10.0f;
        p_inf->cmr_ui.parse = false;
        debug("\nAll initializations succeeded.\nWelcome Bullet-Hell!\n\n");
    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }
    return true;
}

bool App::isIconic() {
    return p_inf->dmanager.isIconic();
}

bool App::update() {
    p_inf->imanager.inspect();
    p_inf->dmanager.drawBegin(nullptr);
    p_inf->p_scene->update();
    p_inf->dmanager.drawEnd();
    return false;
}

void App::drawIdea() {
    p_inf->dmanager.drawModel(&p_inf->idea);
}

void App::applyModel(Model* p_model) {
    if (p_model == nullptr)
        return;
    p_inf->dmanager.setMatrixScale(p_model->scl_x, p_model->scl_y, p_model->scl_z);
    p_inf->dmanager.setMatrixRotate(p_model->deg_x, p_model->deg_y, p_model->deg_z);
    p_inf->dmanager.setMatrixTranslate(p_model->pos_x, p_model->pos_Y, p_model->pos_z);
    p_inf->dmanager.setVectorColor(p_model->col_r, p_model->col_g, p_model->col_b, p_model->col_a);
}

void App::applyCamera(Camera* p_camera) {
    if (p_camera == nullptr) {
        p_inf->dmanager.enableDepthStencil(false);
        applyCamera(&p_inf->cmr_ui);
        return;
    }
    p_inf->dmanager.setMatrixView(p_camera->pos_x, p_camera->pos_Y, p_camera->pos_z, p_camera->dir_x, p_camera->dir_y,
        p_camera->dir_z, p_camera->upp_x, p_camera->upp_y, p_camera->upp_z);
    p_inf->dmanager.setMatrixProject(
        p_camera->width, p_camera->height, p_camera->angle, p_camera->nearZ, p_camera->farZ, p_camera->parse);
}

void App::applyImage(unsigned int id) {
    for (int i = 0; i < kNumImage; ++i) {
        if (p_inf->imgs[i].id == id)
            return p_inf->dmanager.applyImage(&p_inf->imgs[i]);
    }
    p_inf->dmanager.applyImage(nullptr);
}

FrameBuffer* App::createFrameBuffer(unsigned int width, unsigned int height) {
    FrameBuffer* res = new FrameBuffer();
    p_inf->dmanager.createFrameBuffer(width, height, res);
    return res;
}

void App::applyFrameBuffer(FrameBuffer* p_fbuf) {
    p_inf->dmanager.drawBegin(p_fbuf);
}

#include <stdio.h>

bool App::createConsole() {
    if (!AllocConsole())
        return false;
    if (freopen("CONOUT$", "w", stdout) == nullptr)
        return false;
    p_inf->is_debug = true;
    return true;
}

void App::debug(const char* msg) {
    if (p_inf->is_debug)
        printf(msg);
}

void App::debug(const int msg) {
    if (p_inf->is_debug)
        printf("%d", msg);
}
