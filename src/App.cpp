#include <set>

#include "../directx/_dx11private.hpp"
#include "_app.hpp"

// ================================================================================================================= //
//                                          Generals                                                                 //
// ================================================================================================================= //

constexpr unsigned int kNumFontBank = 2U;
constexpr unsigned int kNumStringBank = 2U;

void ModelColorCode2RGBA(Model* p_model, unsigned int col) {
    p_model->col_r = (float)(col & 0x000000ff) / 255.0f;
    p_model->col_g = (float)((col & 0x0000ff00) >> 8) / 255.0f;
    p_model->col_b = (float)((col & 0x00ff0000) >> 16) / 255.0f;
    p_model->col_a = (float)((col & 0xff000000) >> 24) / 255.0f;
}

double Deg2Rad(double deg) {
    return (double)deg * PI / 180.0;
}

int compareString(char* p1, char* p2) {
    for (; *p1 == *p2; ++p1, ++p2) {
        if (*p1 == '\0')
            return 0;
    }
    return *p1 - *p2;
}

unsigned int GetCode(const char* str, int* cnt) {
    if (IsDBCSLeadByte(str[*cnt])) {
        *cnt = *cnt + 1;
        return (unsigned char)str[*cnt - 1] << 8 | (unsigned char)str[*cnt];
    } else
        return (unsigned int)str[*cnt];
}

HANDLE AddFontFromPrivateResource(unsigned int code) {
    HRSRC h_font_res = FindResourceA(nullptr, MAKEINTRESOURCEA(code), "IMAGE");
    if (!h_font_res)
        return nullptr;
    HGLOBAL h_font_data = LoadResource(nullptr, h_font_res);
    if (!h_font_data)
        return nullptr;
    void* p_lock = LockResource(h_font_data);
    if (!p_lock)
        return nullptr;
    DWORD size_res = SizeofResource(nullptr, h_font_res);
    if (size_res == 0)
        return nullptr;
    DWORD cnt_font = 0;
    return AddFontMemResourceEx(p_lock, size_res, NULL, &cnt_font);
}

// ================================================================================================================= //
//                                          Additional Objects                                                       //
// ================================================================================================================= //

class FontBank {
private:
    unsigned int num_font;
    Image* imgs;

public:
    FontBank() : num_font(0U), imgs(nullptr) {
    }
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

class StringBank {
private:
    unsigned int num_str;
    char** strs;

public:
    StringBank() : num_str(0U), strs(nullptr) {
    }
    ~StringBank() {
        if (strs != nullptr)
            delete[] strs;
    }
    int load(HMODULE h_module, unsigned int id) {
        HRSRC h_res = FindResourceA(h_module, MAKEINTRESOURCEA(id), "TEXT");
        if (!h_res)
            return -1;
        HGLOBAL h_data = LoadResource(h_module, h_res);
        if (!h_data)
            return -1;
        void* p_lock = LockResource(h_data);
        if (p_lock == nullptr)
            return -1;
        const DWORD kSizeRes = SizeofResource(h_module, h_res);
        if (kSizeRes == 0)
            return -1;
        unsigned char* str = new unsigned char[kSizeRes];
        memcpy(str, p_lock, sizeof(unsigned char) * kSizeRes);
        unsigned int num = 0U;
        for (int i = 0; i < kSizeRes; ++i) {
            if (str[i] == 13)
                ++num;
        }
        strs = new char*[num];
        unsigned int cnt = 0U;
        unsigned int start = 0U;
        for (int i = 0; i < kSizeRes; ++i) {
            if (str[i] == 13) {
                unsigned int len = i - start;
                if (len == 0)
                    strs[cnt] = "\0";
                else {
                    char* nstr = new char[len + 1];
                    memcpy(nstr, str + start, len);
                    nstr[len] = '\0';
                    strs[cnt] = nstr;
                }
                start = i + 2;
                ++cnt;
                ++i;
            }
        }
        num_str = cnt;
        return num_str;
    }
    void addNecCode(std::set<unsigned int>* set_code, unsigned int idx) {
        const unsigned int kLen = strlen(strs[idx]);
        for (int j = 0; j < kLen; ++j) {
            set_code->insert(GetCode(strs[idx], &j));
        }
    }
    char* getStr(unsigned int idx) {
        return strs[idx];
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
    StringBank* strs;
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
          strs(nullptr),
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

// ================================================================================================================= //
//                                                System Funcs                                                       //
// ================================================================================================================= //

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
        debug(" - DLLResource : Success\n");

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
        flg = flg && loadImage(IMG_BG_CSELECT);
        flg = flg && loadImage(IMG_UI_CSBOX);
        flg = flg && loadImage(IMG_UI_FRAME);
        flg = flg && loadImage(IMG_CH_KOSUZU_B0);
        if (!flg)
            throw "Failed to load some images.";
        debug(" - Images : Success\n");

        p_inf->strs = new StringBank[kNumStringBank];
        if (p_inf->strs == nullptr)
            throw "Failed to create array of string bank.";
        memset(p_inf->strs, 0, sizeof(StringBank) * kNumStringBank);
        if (p_inf->strs[kStrTitle].load(h_module, IDS_TITLE) < 5)
            throw "Failed to load title texts.";
        if (p_inf->strs[kStrCSelect].load(h_module, IDS_CSELECT) < 11)
            throw "Failed to load cselect texts.";
        debug(" - Strings : Success\n");

        p_inf->fnts = new FontBank[kNumFontBank];
        if (p_inf->fnts == nullptr)
            throw "Failed to create array of fonts.";
        memset(p_inf->fnts, 0, sizeof(FontBank) * kNumFontBank);
        HANDLE h_normal = AddFontFromPrivateResource(FNT_NORMAL);
        if (!h_normal)
            throw "Failed to load '源ノ明朝' font.";
        HANDLE h_option = AddFontFromPrivateResource(FNT_OPTION);
        if (!h_option)
            throw "Failed to load 'Copperplate' font.";
        auto loadString = [&](LOGFONTA* logfont, unsigned int idx_bank, std::set<unsigned int>* set_code) {
            for (auto itr = set_code->begin(); itr != set_code->end(); ++itr) {
                flg = flg && p_inf->dmanager.createFontImage(logfont, *itr, p_inf->fnts[idx_bank].getFontNext());
            }
        };
        LOGFONTA logfont_msg = {64, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "源ノ明朝 Heavy"};
        std::set<unsigned int> set_code_normal{48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 102, 112, 115, 46};
        for (int i = 1; i < 11; ++i) {
            p_inf->strs[kStrCSelect].addNecCode(&set_code_normal, i);
        }
        p_inf->fnts[kIdxNormal].init(set_code_normal.size() + 1);
        loadString(&logfont_msg, kIdxNormal, &set_code_normal);
        LOGFONTA logfont_elp = {64, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, OUT_TT_ONLY_PRECIS, CLIP_DEFAULT_PRECIS,
            PROOF_QUALITY, DEFAULT_PITCH | FF_MODERN, "Elephant"};
        std::set<unsigned int> set_code_elp{48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 102, 112, 115};
        for (int i = 0; i < 5; ++i) {
            p_inf->strs[kStrTitle].addNecCode(&set_code_elp, i);
        }
        p_inf->strs[kStrCSelect].addNecCode(&set_code_elp, 0);
        p_inf->fnts[kIdxOption].init(set_code_elp.size() + 1);
        loadString(&logfont_elp, kIdxOption, &set_code_elp);
        flg = flg && RemoveFontMemResourceEx(h_normal);
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
            if (buf != 'u' && buf != 'd' && buf != 'l' && buf != 'r' && buf != 's' && buf != 't' && buf != 'b' &&
                buf != 'c' && buf != '1' && buf != '2' && buf != '3' && buf != 'a' && buf != 'b' && buf != 'x' &&
                buf != 'y' && buf != ',')
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

char* App::getStr(unsigned int idx_bank, unsigned int idx_str) {
    return p_inf->strs[idx_bank].getStr(idx_str);
}

// ================================================================================================================= //
//                                           Drawing Funcs                                                           //
// ================================================================================================================= //

void App::drawIdea() {
    p_inf->dmanager.drawModel(&p_inf->idea);
}

void App::drawString(const char* str, const Model* p_model, unsigned int idx_bank, int align) {
    Model model = *p_model;
    model.scl_x = model.scl_y;
    const int kLenStr = strlen(str);
    unsigned int* codes = new unsigned int[kLenStr];
    float* asps = new float[kLenStr];
    memset(codes, 0, sizeof(unsigned int) * kLenStr);
    memset(asps, 0, sizeof(float) * kLenStr);
    unsigned int cnt = 0U;
    float sum = 0.0f;
    for (int i = 0; i < kLenStr; ++i) {
        if (str[i] == '\0')
            break;
        codes[cnt] = GetCode(str, &i);
        Image* p_image = p_inf->fnts[idx_bank].getFont(codes[cnt]);
        asps[cnt] = p_image != nullptr ? (float)p_image->width / (float)p_image->height : 1.0f;
        sum += model.scl_y * asps[cnt];
        ++cnt;
    }
    if (align == 0)
        model.pos_x -= sum / 2.0f;
    else if (align == 1)
        model.pos_x -= sum;
    for (int i = 0; i < kLenStr; ++i) {
        if (codes[i] == 0)
            break;
        model.scl_x = asps[i] * model.scl_y;
        p_inf->dmanager.applyImage(p_inf->fnts[idx_bank].getFont(codes[i]));
        applyModelUI(&model);
        drawIdea();
        model.pos_x += model.scl_x;
    }
    delete codes;
    delete asps;
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

FrameBuffer* App::createFrameBuffer(unsigned int width, unsigned int height) {
    FrameBuffer* res = new FrameBuffer();
    p_inf->dmanager.createFrameBuffer(width, height, res);
    return res;
}

void App::applyFrameBuffer(FrameBuffer* p_fbuf) {
    p_inf->dmanager.drawBegin(p_fbuf);
}

// ================================================================================================================= //
//                                             Input Funcs                                                           //
// ================================================================================================================= //

bool App::getKey(KEY_CODE code, KEY_STATE state) {
    char res = p_inf->imanager.getKey(static_cast<char>(code));
    if (state == KEY_STATE::Nutral)
        return (res & 0b011) == 0;
    if (state == KEY_STATE::Down)
        return (res & 0b010) > 0;
    if (state == KEY_STATE::Pressed)
        return (res & 0b001) > 0;
    if (state == KEY_STATE::Up)
        return (res & 0b100) > 0;
    return false;
}

// ================================================================================================================= //
//                                             Debug Funcs                                                           //
// ================================================================================================================= //

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

// ================================================================================================================= //
//                                              Game Funcs                                                           //
// ================================================================================================================= //

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
    applyCamera(nullptr);
    p_inf->p_scene->update();

    if (p_inf->no_scene_nex != kSceneEscape) {
        delete p_inf->p_scene;
        if (p_inf->no_scene_nex == kSceneCSelect)
            p_inf->p_scene = new SceneCSelect(this);
        else if (p_inf->no_scene_nex == kSceneGame)
            p_inf->p_scene = new SceneGame(this);
        else if (p_inf->no_scene_nex == kSceneExit)
            return true;
        else
            p_inf->p_scene = new SceneTitle(this);
        p_inf->no_scene_cur = p_inf->no_scene_nex;
        p_inf->no_scene_nex = kSceneEscape;
        if (!p_inf->p_scene->init())
            return true;
    }

    Model model = Model();
    model.pos_x = 1275.0f;
    model.pos_y = 925.0f;
    model.scl_y = 30.0f;
    char buf[16] = "";
    snprintf(buf, 64, "%3.1ffps", p_inf->fps);
    drawString(buf, &model, kIdxNormal, 1);
    p_inf->dmanager.drawEnd();
    return false;
}

void App::changeScene(unsigned int no_scene_nex) {
    p_inf->no_scene_nex = no_scene_nex;
}

void App::updatePlayer() {
    player.update();
}

void App::drawPlayer() {
    player.draw();
}
