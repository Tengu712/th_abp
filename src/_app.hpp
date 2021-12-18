#pragma once
#ifndef _APP_
#define _APP_

#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643

#include "../directx/_dx11.hpp"
#include "./_resource.hpp"

#pragma comment(lib, "mydx.lib")
#pragma comment(lib, "Winmm.lib")

constexpr unsigned int kSceWidth = 1280U;
constexpr unsigned int kSceHeight = 960U;
constexpr unsigned int kNumImage = 30U;
constexpr unsigned int kNumFontBank = 2U;
constexpr unsigned int kIdxNormal = 0U;
constexpr unsigned int kIdxElephant = 1U;
constexpr unsigned int kSceneTitle = 0U;
constexpr unsigned int kSceneCSelect = 1U;
constexpr unsigned int kSceneGame = 2U;
constexpr unsigned int kSceneExit = 128U;
constexpr unsigned int kSceneEscape = 255U;

class App;

struct Camera {
    float width, height;
    float angle, near_z, far_z;
    float pos_x, pos_y, pos_z;
    float dir_x, dir_y, dir_z;
    float upp_x, upp_y, upp_z;
    bool parse;
    Camera()
        : width(kSceWidth),
          height(kSceHeight),
          angle(0.7853981853f),
          near_z(1.0f),
          far_z(2000.0f),
          pos_x(0.0f),
          pos_y(0.0f),
          pos_z(0.0f),
          dir_x(0.0f),
          dir_y(0.0f),
          dir_z(1.0f),
          upp_x(0.0f),
          upp_y(1.0f),
          upp_z(0.0f),
          parse(false) {
    }
};

struct Model {
    float pos_x, pos_y, pos_z;
    float deg_x, deg_y, deg_z;
    float scl_x, scl_y, scl_z;
    float col_r, col_g, col_b, col_a;
    Model()
        : pos_x(0.0f),
          pos_y(0.0f),
          pos_z(0.0f),
          deg_x(0.0f),
          deg_y(0.0f),
          deg_z(0.0f),
          scl_x(1.0f),
          scl_y(1.0f),
          scl_z(1.0f),
          col_r(1.0f),
          col_g(1.0f),
          col_b(1.0f),
          col_a(1.0f) {
    }
};

struct Entity {
    bool moving, existing;
    double x, y;
    double deg, spd;
    Entity() : moving(false), existing(false), x(0.0), y(0.0), deg(0.0), spd(0.0) {
    }
    void move();
};

class Player : public Entity {
private:
    App* p_app;
    unsigned int id_weapon;
    unsigned int cnt_all;
    int lr;

public:
    Player() : Entity(), p_app(nullptr), id_weapon(0U), cnt_all(0U), lr(0) {
    }
    void init(App* p_app);
    void update();
    void draw();
};

class Scene {
protected:
    App* p_app;

public:
    Scene(App* p_app) : p_app(p_app) {
    }
    virtual bool init() = 0;
    virtual void update() = 0;
};

class SceneTitle : public Scene {
private:
    int cnt;
    int cur;

public:
    SceneTitle(App* p_app) : Scene(p_app), cnt(0), cur(0) {
    }
    bool init();
    void update();
};

class SceneCharacterSelect : public Scene {
private:
    unsigned int cnt;
    unsigned int cur;

public:
    SceneCharacterSelect(App* p_app) : Scene(p_app), cnt(0), cur(0) {
    }
    bool init();
    void update();
};

class SceneGame : public Scene {
private:
    unsigned int cnt;

public:
    SceneGame(App* p_app) : Scene(p_app), cnt(0U) {
    }
    bool init();
    void update();
};

enum struct KEY_CODE {
    None,
    Up,
    Down,
    Left,
    Right,
    Z,
    X,
    Shift,
    Escape,
};

enum struct KEY_STATE {
    Nutral,
    Down,
    Pressed,
    Up,
};

struct AppInf;

struct GameInf {
    Player player;
    GameInf() : player(Player()) {
    }
};

class App {
private:
    AppInf* p_inf;
    GameInf ginf;

public:
    App() : p_inf(nullptr) {
    }
    ~App() {
        if (p_inf != nullptr)
            delete p_inf;
    }
    bool init(HINSTANCE h_inst, LPSTR p_cmd, int cmd_show);
    bool isIconic();
    bool update();
    void changeScene(unsigned int no_scene_nex);
    void drawIdea();
    void drawString(const Model* p_model, int align, unsigned int idx_bank, const char* str);
    void drawStringWithBorder(const Model* p_model, int align, unsigned int col, unsigned int idx_bank1,
        unsigned int idx_bank2, const char* str);
    void applyModel(Model* p_model);
    void applyModelUI(Model* p_model);
    void applyCamera(Camera* p_camera);
    void applyImage(unsigned int id);
    void applyFont(unsigned int idx_bank, unsigned int code);
    FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height);
    void applyFrameBuffer(FrameBuffer* p_fbuf);
    bool getKey(KEY_CODE code, KEY_STATE state);
    void initGameInf();
    void updatePlayer();
    void drawPlayer();
    bool createConsole();
    void debug(const char* msg);
    void debug(const int msg);
};

void ModelColorCode2RGBA(Model* p_model, unsigned int col);
double Deg2Rad(double deg);

#endif
