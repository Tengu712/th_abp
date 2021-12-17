#include "../directx/_dx11private.hpp"
#include "_app.hpp"

void ModelColorCode2RGBA(Model* p_model, unsigned int col) {
    p_model->col_r = (float)(col & 0x000000ff) / 255.0f;
    p_model->col_g = (float)((col & 0x0000ff00) >> 8) / 255.0f;
    p_model->col_b = (float)((col & 0x00ff0000) >> 16) / 255.0f;
    p_model->col_a = (float)((col & 0xff000000) >> 24) / 255.0f;
}

double Deg2Rad(double deg) {
    return (double)deg * PI / 180.0;
}

class FontBank {
private:
    unsigned int num_font;
    Image* imgs;
public:
    FontBank() : num_font(0U), imgs(nullptr) {}
    ~FontBank() {
        if (imgs != nullptr)
            delete imgs;
    }
    bool init(unsigned int num_font) {
        imgs = new Image[num_font];
        if (imgs == nullptr)
            return false;
        memset(imgs, 0, sizeof(Image) * num_font);
        this->num_font = num_font;
        return true;
    }
    Image* getFont(unsigned int code) {
        for (int i = 0; i < num_font; ++i) {
            if (imgs[i].id == code)
                return &imgs[i];
        }
        return nullptr;
    }
    Image* getFontNext() {
        for (int i = 0; i < num_font; ++i) {
            if (imgs[i].id != 0U)
                continue;
            return &imgs[i];
        }
        return nullptr;
    }
};

struct AppInf {
    D3DManager dmanager;
    InputManager imanager;
    bool is_debug;
    ModelInf idea;
    Camera cmr_ui;
    unsigned long time_last;
    unsigned int cnt_fps;
    float fps;
    Image* imgs;
    FontBank* fnts;
    unsigned int no_scene_cur;
    unsigned int no_scene_nex;
    Scene* p_scene;
    AppInf()
        : dmanager(D3DManager()),
          imanager(InputManager()),
          is_debug(false),
          idea(ModelInf()),
          cmr_ui(Camera()),
          time_last(timeGetTime()),
          cnt_fps(0U),
          fps(0.0f),
          imgs(nullptr),
          fnts(nullptr),
          no_scene_cur(0),
          no_scene_nex(0),
          p_scene(nullptr) {
    }
    ~AppInf() {
        if (imgs != nullptr)
            delete imgs;
        if (fnts != nullptr)
            delete fnts;
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

    const bool kWindowed = MessageBoxA(nullptr, "フルスクリーンで起動しますか", "確認", MB_YESNO) == IDNO;
    if (kWindowed && compareString(p_cmd, "debug") == 0 && !createConsole())
        return false;

    debug("\n==================================================\n");
    debug("            \"Aya's Bullethell Practice\"\n");
    debug("      SkyDog Assoc of WordSpiritism, Tengu712 \n");
    debug("==================================================\n\n");
    debug("Starts up ...\n");

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

        p_inf->dmanager.drawBegin(nullptr);
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
        model_load.scl_y = 1280.0f;
        p_inf->cmr_ui.pos_z = -10.0f;
        p_inf->cmr_ui.parse = false;
        applyCamera(nullptr);
        applyModelUI(&model_load);
        drawIdea();
        p_inf->dmanager.drawEnd();
        debug("Load begins ...\n");

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
        flg = flg && loadImage(IMG_CH_KOSUZU_B0);
        if (!flg)
            throw "Failed to load some images.";
        debug(" - Images : Success\n");

        p_inf->fnts = new FontBank[kNumFontBank];
        if (p_inf->fnts == nullptr)
            throw "Failed to create array of fonts.";
        memset(p_inf->fnts, 0, sizeof(FontBank) * kNumFontBank);
        DESIGNVECTOR design;
        if (AddFontResourceExA("C:/Windows/Fonts/ELEPHNT.TTF", FR_PRIVATE, &design) == 0)
            throw "Failed to load 'Elephant' font.";
        auto loadString = [&](LOGFONTA* logfont, unsigned int idx_bank, const char* str) {
            const int kLenStr = strlen(str);
            for (int i = 0; i < kLenStr; ++i) {
                unsigned int code = 0U;
                if (IsDBCSLeadByte(str[i])) { 
                    code = (unsigned char)str[i] << 8 | (unsigned char)str[i + 1];
                    ++i;
                } else
                    code = (unsigned int)str[i];
                flg = flg && p_inf->dmanager.createFontImage(logfont, code, p_inf->fnts[idx_bank].getFontNext());
            }
        };
        LOGFONTA logfont_msg = {
            64, 0, 0, 0,
            0, 0, 0, 0,
            DEFAULT_CHARSET,
            OUT_TT_ONLY_PRECIS,
            CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY,
            DEFAULT_PITCH | FF_MODERN,
            "MS Gothic",
        };
        p_inf->fnts[kIdxNormal].init(140U);
        loadString(&logfont_msg, 0, " .0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZあいうえおかきくけこさしすせそざじずぜぞたちつてとだぢづでどなにぬねのはひふへほばびぶべぼぱぴぷぺぽまみむめもやゆよらりるれろわをんゃゅょ");
        LOGFONTA logfont_elp = {
            64, 0, 0, 0,
            0, 0, 0, 0,
            DEFAULT_CHARSET,
            OUT_TT_ONLY_PRECIS,
            CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY,
            DEFAULT_PITCH | FF_MODERN,
            "Elephant",
        };
        p_inf->fnts[kIdxElephant].init(25U);
        loadString(&logfont_elp, 1, " StarChpecoRlyQuikDgAsW20");
        if (!flg)
            throw "Failed to load some fonts";
        debug(" - Fonts : Success.\n");

        FILE* p_file_cfg = fopen("./keyconfig.cfg", "r");
        if (!p_file_cfg) 
            throw "Failed to open keyconfig.cfg.";
        int cnt_key = 0;
        int cnt_buf = 0;
        int buf = 0;
        int bufs[2] = {0, 0};
        char map_key[8] = {VK_UP, VK_DOWN, VK_LEFT, VK_RIGHT, 0x5A, 0x58, VK_SHIFT, VK_ESCAPE};
        while ((buf = fgetc(p_file_cfg)) != EOF) {
            if (buf != 'u' && buf != 'd' && buf != 'l' && buf != 'r' 
                    && buf != 's' && buf != 't' && buf != 'b' && buf != 'c'
                    && buf != '1' && buf != '2' && buf != '3'
                    && buf != 'a' && buf != 'b' && buf != 'x' && buf != 'y' && buf != ',')
                continue;
            if (cnt_key >= 8 || cnt_buf > 2)
                throw "Invalid keyconfig.";
            if (buf == ',') {
                GAMEPAD_KEYTYPE t;
                short c = 0;
                if (bufs[0] == 'l' && bufs[1] == '2') {
                    t = GAMEPAD_KEYTYPE::LTrigger;
                    c = 12800;
                } else if (bufs[0] == 'r' && bufs[1] == '2') {
                    t = GAMEPAD_KEYTYPE::RTrigger;
                    c = 12800;
                } else if (bufs[0] == 'l' && bufs[1] == 'l') {
                    t = GAMEPAD_KEYTYPE::ThumbLL;
                    c = -12800;
                } else if (bufs[0] == 'l' && bufs[1] == 'r') {
                    t = GAMEPAD_KEYTYPE::ThumbLR;
                    c = 12800;
                } else if (bufs[0] == 'l' && bufs[1] == 'u') {
                    t = GAMEPAD_KEYTYPE::ThumbLU;
                    c = 12800;
                } else if (bufs[0] == 'l' && bufs[1] == 'd') {
                    t = GAMEPAD_KEYTYPE::ThumbLD;
                    c = -12800;
                } else if (bufs[0] == 'r' && bufs[1] == 'l') {
                    t = GAMEPAD_KEYTYPE::ThumbRL;
                    c = -12800;
                } else if (bufs[0] == 'r' && bufs[1] == 'r') {
                    t = GAMEPAD_KEYTYPE::ThumbRR;
                    c = 12800;
                } else if (bufs[0] == 'r' && bufs[1] == 'u') {
                    t = GAMEPAD_KEYTYPE::ThumbRU;
                    c = 12800;
                } else if (bufs[0] == 'r' && bufs[1] == 'd') {
                    t = GAMEPAD_KEYTYPE::ThumbRD;
                    c = -12800;
                } else if (bufs[0] == 's' && bufs[1] == 't') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_START;
                } else if (bufs[0] == 'b' && bufs[1] == 'c') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_BACK;
                } else if (bufs[0] == 'l' && bufs[1] == '1') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_LEFT_SHOULDER;
                } else if (bufs[0] == 'r' && bufs[1] == '1') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_RIGHT_SHOULDER;
                } else if (bufs[0] == 'l' && bufs[1] == '3') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_LEFT_THUMB;
                } else if (bufs[0] == 'r' && bufs[1] == '3') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_RIGHT_THUMB;
                } else if (bufs[0] == 'u') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_DPAD_UP;
                } else if (bufs[0] == 'd') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_DPAD_DOWN;
                } else if (bufs[0] == 'l') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_DPAD_LEFT;
                } else if (bufs[0] == 'r') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_DPAD_RIGHT;
                } else if (bufs[0] == 'a') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_A;
                } else if (bufs[0] == 'b') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_B;
                } else if (bufs[0] == 'x') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_X;
                } else if (bufs[0] == 'y') {
                    t = GAMEPAD_KEYTYPE::Buttons;
                    c = XINPUT_GAMEPAD_Y;
                } else
                    throw "Invalid keycode.";
                if (!p_inf->imanager.addKeycode(cnt_key + 1, map_key[cnt_key], t, c))
                    throw "Failed to add key.";
                cnt_key++;
                cnt_buf = 0;
                bufs[0] = 0;
                bufs[1] = 0;
                continue;
            }
            bufs[cnt_buf] = buf;
            cnt_buf++;
        }
        fclose(p_file_cfg);
        debug(" - Keys : Success\n");

        p_inf->p_scene = new SceneTitle(this);
        if (!p_inf->p_scene->init())
            throw "Failed to initialize title.";
        p_inf->no_scene_nex = kSceneEscape;
        debug(" - Title Scene : Success\n");

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
    unsigned long time_start = timeGetTime();
    if (time_start - p_inf->time_last > 1000L) {
        p_inf->fps = (float)(p_inf->cnt_fps * 1000U) / (float)(time_start - p_inf->time_last);
        p_inf->time_last = time_start;
        p_inf->cnt_fps = 0U;
    }
    ++p_inf->cnt_fps;

    p_inf->imanager.inspect();
    p_inf->dmanager.drawBegin(nullptr);
    p_inf->p_scene->update();

    if (p_inf->no_scene_nex != kSceneEscape) {
        delete p_inf->p_scene;
        if (p_inf->no_scene_nex == kSceneTitle) 
            p_inf->p_scene = new SceneTitle(this);
        else if (p_inf->no_scene_nex == kSceneExit)
            return true;
        p_inf->no_scene_cur = p_inf->no_scene_nex;
        p_inf->no_scene_nex = kSceneEscape;
        if (!p_inf->p_scene->init())
            return true;
    }

    Model model = Model();
    model.pos_x = 1275.0f;
    model.pos_y = 933.0f;
    model.scl_x = 14.0f;
    model.scl_y = 22.0f;
    char buf[64] = "";
    snprintf(buf, 64, "%3.1ffps", p_inf->fps);
    drawString(&model, 1, kIdxNormal, buf);
    p_inf->dmanager.drawEnd();
    return false;
}

void App::changeScene(unsigned int no_scene_nex) {
    p_inf->no_scene_nex = no_scene_nex;
}

void App::drawIdea() {
    p_inf->dmanager.drawModel(&p_inf->idea);
}

void App::drawString(const Model* p_model, int align, unsigned int idx_bank, const char* str) {
    Model model = *p_model;
    const int kLenStr = strlen(str);
    if (align == 0)
        model.pos_x -= model.scl_x * (float)kLenStr / 2.0f;
    else if (align == 1)
        model.pos_x -= model.scl_x * (float)kLenStr;
    for (int i = 0; i < kLenStr; ++i) {
        unsigned int code = 0U;
        if (IsDBCSLeadByte(str[i])) {
            code = (unsigned char)str[i] << 8 | (unsigned char)str[i + 1];
            ++i;
        } else
            code = (unsigned int)str[i];
        applyModelUI(&model);
        applyFont(idx_bank, code);
        drawIdea();
        model.pos_x += model.scl_x;
    }
}

void App::drawStringWithBorder(const Model* p_model, int align, unsigned int col,
        unsigned int idx_bank1, unsigned int idx_bank2, const char* str) {
    const float nr = (float)(col & 0x000000ff) / 255.0f;
    const float ng = (float)((col & 0x0000ff00) >> 8) / 255.0f;
    const float nb = (float)((col & 0x00ff0000) >> 16) / 255.0f;
    const float na = (float)((col & 0xff000000) >> 24) / 255.0f;
    Model model = *p_model;
    model.pos_x += model.scl_x / 2.0f;
    model.pos_y -= model.scl_y / 2.0f;
    const int kLenStr = strlen(str);
    if (align == 0)
        model.pos_x -= model.scl_x * (float)kLenStr / 2.0f;
    else if (align == 1)
        model.pos_x -= model.scl_x * (float)kLenStr;
    for (int i = 0; i < kLenStr; ++i) {
        unsigned int code = 0U;
        if (IsDBCSLeadByte(str[i])) {
            code = (unsigned char)str[i] << 8 | (unsigned char)str[i + 1];
            ++i;
        } else
            code = (unsigned int)str[i];
        model.col_r = nr;
        model.col_g = ng;
        model.col_b = nb;
        model.col_a = na;
        applyModel(&model);
        applyFont(idx_bank2, code);
        drawIdea();
        model.col_r = p_model->col_r;
        model.col_g = p_model->col_g;
        model.col_b = p_model->col_b;
        model.col_a = p_model->col_a;
        applyModel(&model);
        applyFont(idx_bank1, code);
        drawIdea();
        model.pos_x += model.scl_x;
    }
}

void App::applyModel(Model* p_model) {
    if (p_model == nullptr)
        return;
    p_inf->dmanager.setMatrixScale(p_model->scl_x, p_model->scl_y, p_model->scl_z);
    p_inf->dmanager.setMatrixRotate(p_model->deg_x, p_model->deg_y, p_model->deg_z);
    p_inf->dmanager.setMatrixTranslate(p_model->pos_x, p_model->pos_y, p_model->pos_z);
    p_inf->dmanager.setVectorColor(p_model->col_r, p_model->col_g, p_model->col_b, p_model->col_a);
}

void App::applyModelUI(Model* p_model) {
    if (p_model == nullptr)
        return;
    Model model = *p_model;
    model.pos_x -= (float)(kSceWidth / 2) - model.scl_x / 2.0f;
    model.pos_y = (float)(kSceHeight / 2) - model.scl_y / 2.0f - model.pos_y;
    applyModel(&model);
}

void App::applyCamera(Camera* p_camera) {
    if (p_camera == nullptr) {
        p_inf->dmanager.enableDepthStencil(false);
        applyCamera(&p_inf->cmr_ui);
        return;
    }
    p_inf->dmanager.setMatrixView(p_camera->pos_x, p_camera->pos_y, p_camera->pos_z, p_camera->dir_x, p_camera->dir_y,
        p_camera->dir_z, p_camera->upp_x, p_camera->upp_y, p_camera->upp_z);
    p_inf->dmanager.setMatrixProject(
        p_camera->width, p_camera->height, p_camera->angle, p_camera->near_z, p_camera->far_z, p_camera->parse);
}

void App::applyImage(unsigned int id) {
    for (int i = 0; i < kNumImage; ++i) {
        if (p_inf->imgs[i].id == id)
            return p_inf->dmanager.applyImage(&p_inf->imgs[i]);
    }
    p_inf->dmanager.applyImage(nullptr);
}

void App::applyFont(unsigned int idx_bank, unsigned int code) {
    p_inf->dmanager.applyImage(p_inf->fnts[idx_bank].getFont(code));
}

FrameBuffer* App::createFrameBuffer(unsigned int width, unsigned int height) {
    FrameBuffer* res = new FrameBuffer();
    p_inf->dmanager.createFrameBuffer(width, height, res);
    return res;
}

void App::applyFrameBuffer(FrameBuffer* p_fbuf) {
    p_inf->dmanager.drawBegin(p_fbuf);
}

bool App::getKey(KEY_CODE code, KEY_STATE state) {
    char res = p_inf->imanager.getKey(static_cast<char>(code));
    if (state == KEY_STATE::Nutral)
        return (res & 0b011) == 0;
    debug((int)res);
    if (state == KEY_STATE::Down)
        return (res & 0b010) > 0;
    if (state == KEY_STATE::Pressed)
        return (res & 0b001) > 0;
    if (state == KEY_STATE::Up)
        return (res & 0b100) > 0; 
    return false;
}

bool App::createConsole() {
    if (!AllocConsole())
        return false;
    if (freopen("CONOUT$", "w", __acrt_iob_func(1)) == nullptr)
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
