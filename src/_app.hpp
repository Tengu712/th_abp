#pragma once
#ifndef _APP_
#define _APP_

#include <math.h>
#include <stdio.h>
#include <windows.h>

#define PI 3.141592653589793238462643

#include "./_resource.hpp"
#include "./_resource_private.hpp"

#pragma comment(lib, "mydx.lib")
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "Winmm.lib")

constexpr unsigned int kSceWidth = 1280U;
constexpr unsigned int kSceHeight = 960U;
constexpr unsigned int kNumImage = 30U;
constexpr unsigned int kNumBulletPlayer = 128U;
constexpr unsigned int kIdxNormal = 0U;
constexpr unsigned int kIdxOption = 1U;
constexpr unsigned int kStrTitle = 0U;
constexpr unsigned int kStrCSelect = 1U;
constexpr unsigned int kStrGame = 2U;
constexpr unsigned int kSceneTitle = 0U;
constexpr unsigned int kSceneCSelect = 1U;
constexpr unsigned int kSceneGame = 2U;
constexpr unsigned int kSceneExit = 128U;
constexpr unsigned int kSceneEscape = 255U;

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

class App;
struct FrameBuffer;

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

struct InputInf {
    int dx, dy;
    int z, x, s;
    InputInf() : dx(0), dy(0), z(0), x(0), s(0) {
    }
};

class Entity {
protected:
    bool moving, existing;
    double x, y, r;
    double deg, spd;

public:
    Entity() : moving(false), existing(false), x(0.0), y(0.0), r(0.0), deg(0.0), spd(0.0) {
    }
    void run();
    void move();
    void setModelPosDeg(Model* p_model);
    bool isExisting();
    double getX();
    double getY();
    double getR();
    double getDeg();
    double getSpd();
    void setPos(double x, double y);
    void setDeg(double deg);
    void setSpd(double spd);
    void transPos(double dx, double dy);
    void transDeg(double ddeg);
    void transSpd(double dspd);
};

class Bullet : public Entity {
private:
    App* p_app;
    int cnt;
    unsigned int knd, mov, dmg;
    unsigned int col;
    float scl_x, scl_y;

public:
    bool del;
    Bullet()
        : Entity(),
          p_app(nullptr),
          cnt(0U),
          knd(0U),
          mov(0U),
          dmg(0U),
          col(0xffffffff),
          scl_x(0.0f),
          scl_y(0.0f),
          del(false) {
    }
    void init(App* p_app, unsigned int knd, unsigned int mov, unsigned int dmg, unsigned int col, int cnt);
    void update();
    void draw();
    int isHit(Entity* p_trg);
};

class OptionManager {
private:
    App* p_app;
    unsigned int id_weapon;
    unsigned int cnt;
    double prevs[3];

public:
    Entity options[4];
    OptionManager(App* p_app, unsigned int id_weapon)
        : p_app(p_app),
          cnt(0U),
          id_weapon(id_weapon),
          prevs{0.0, 0.0, 0.0},
          options{Entity(), Entity(), Entity(), Entity()} {
    }
    void update();
    void draw();
};

class Player : public Entity {
private:
    App* p_app;
    unsigned int id_weapon;
    unsigned int cnt;
    double x_max, x_min, y_max, y_min;
    OptionManager omanager;

public:
    Player(App* p_app)
        : Entity(),
          p_app(p_app),
          id_weapon(0U),
          cnt(0U),
          x_max(0.0),
          x_min(0.0),
          y_max(0.0),
          y_min(0.0),
          omanager(OptionManager(p_app, 0U)) {
    }
    void init(int id_weapon, double x, double y, double x_min, double x_max, double y_min, double y_max);
    void update();
    void draw();
    void drawSlow();
};

class Enemy : public Entity {
private:
    App* p_app;

public:
    Enemy(App* p_app) : Entity(), p_app(p_app) {
        r = 10.0f;
    }
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

class SceneCSelect : public Scene {
private:
    int cnt_all, cnt_player;
    unsigned int cur;

public:
    SceneCSelect(App* p_app) : Scene(p_app), cnt_all(0), cnt_player(0), cur(0U) {
    }
    bool init();
    void update();
};

class SceneGame : public Scene {
private:
    int cnt, cur;
    bool is_pause;
    FrameBuffer* p_fbuf;

public:
    SceneGame(App* p_app) : Scene(p_app), cnt(0), cur(0), is_pause(false), p_fbuf(nullptr) {
    }
    ~SceneGame() {
        if (p_fbuf != nullptr)
            delete p_fbuf;
    }
    bool init();
    void updatePausing();
    void updateGaming();
    void update();
    void drawGame();
    void drawUI();
    void drawOption();
    void drawFrameBuffer();
    void drawFrame();
};

struct AppInf;

class App {
private:
    AppInf* p_inf;
    InputInf iinf;
    unsigned long long hiscore, score;
    unsigned int graze;
    double rank;
    Player player;
    Enemy enemy;
    Bullet* buls_p;

public:
    App()
        : p_inf(nullptr),
          iinf(InputInf()),
          hiscore(0LL),
          score(0LL),
          graze(0U),
          rank(0.0),
          player(Player(this)),
          enemy(Enemy(this)),
          buls_p(nullptr) {
    }
    ~App() {
        if (p_inf != nullptr)
            delete p_inf;
        if (buls_p != nullptr)
            delete buls_p;
    }
    // System
    bool init(HINSTANCE h_inst, LPSTR p_cmd, int cmd_show);
    bool isIconic();
    char* getStr(unsigned int idx_bank, unsigned int idx_str);
    // Drawing
    void drawIdea();
    void drawString(const char* str, const Model* p_model, unsigned int idx_bank, int align = -1);
    void applyModel(Model* p_model);
    void applyModelUI(Model* p_model);
    void applyCamera(Camera* p_camera);
    void applyImage(unsigned int id);
    void applyFrameBuffer(FrameBuffer* p_fbuf);
    FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height);
    void drawBeginWithFrameBuffer(FrameBuffer* p_fbuf);
    void enableOverlay(bool is_enable);
    void enableMosaic(bool is_enable);
    // Input
    bool getKey(KEY_CODE code, KEY_STATE state);
    // Debug
    bool createConsole();
    void debug(const char* msg);
    void debug(const int msg);
    // Game
    bool update();
    void changeScene(unsigned int no_scene_nex);
    InputInf* getInputInf();
    unsigned long long getHiScore();
    unsigned long long getScore();
    unsigned int getGraze();
    double getRank();
    Player* getPlayer();
    Enemy* getEnemy();
    void setInputInf(InputInf* p_iinf);
    void setHiScore(unsigned long long hiscore);
    void setScore(unsigned long long score);
    void setGraze(unsigned int graze);
    void setRank(double rank);
    void transScore(unsigned long long d_score);
    void transGraze(unsigned int d_graze);
    void transRank(double d_rank);
    void pushBulletPlayer(Bullet* p_bul);
    void updateBulletPlayer();
    void drawBulletPlayer();
    void clearBulletPlayer();
};

void ModelColorCode2RGBA(Model* p_model, unsigned int col);
double Deg2Rad(double deg);
double Homing(double sx, double sy, double gx, double gy);

#endif
