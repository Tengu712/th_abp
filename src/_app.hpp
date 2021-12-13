#pragma once
#ifndef _APP_
#define _APP_

#include <stdio.h>

#include "./_resource.hpp"
#include "../directx/_dx11.hpp"
#pragma comment(lib, "mydx.lib")
#pragma comment(lib, "Winmm.lib")

constexpr unsigned int kSceWidth = 1280U;
constexpr unsigned int kSceHeight = 960U;
constexpr unsigned int kNumImage = 30U;
constexpr unsigned int kNumFont = 80U;

class App;

struct Camera {
    float width, height;
    float angle, near_z, far_z;
    float pos_x, pos_y, pos_z;
    float dir_x, dir_y, dir_z;
    float upp_x, upp_y, upp_z;
    bool parse;
    Camera() :
        width(kSceWidth),
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
        parse(false)
    {}
};

struct Model {
    float pos_x, pos_y, pos_z;
    float deg_x, deg_y, deg_z;
    float scl_x, scl_y, scl_z;
    float col_r, col_g, col_b, col_a;
    Model() :
        pos_x(0.0f),
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
        col_a(1.0f)
    {}
};

class Scene {
protected:
    App* p_app;
public:
    Scene(App* p_app) : p_app(p_app) {}
    virtual bool init() = 0;
    virtual void update() = 0;
};

class SceneTitle : public Scene {
private:
    unsigned int cnt;
public:
    SceneTitle(App* p_app) : Scene(p_app), cnt(0U) {}
    bool init();
    void update();
};

struct AppInf;

class App {
private:
    AppInf* p_inf;
public:
    App() : p_inf(nullptr) {}
    ~App() {
        if (p_inf != nullptr)
            delete p_inf;
    }
    bool init(HINSTANCE h_inst, LPSTR p_cmd, int cmd_show);
    bool isIconic();
    bool update();
    void drawIdea();
    void drawString(const char* str, Model* p_model, int align);
    void applyModel(Model* p_model);
    void applyModelUI(Model* p_model);
    void applyCamera(Camera* p_camera);
    void applyImage(unsigned int id);
    void applyFont(unsigned int code);
    FrameBuffer* createFrameBuffer(unsigned int width, unsigned int height);
    void applyFrameBuffer(FrameBuffer* p_fbuf);
    bool createConsole();
    void debug(const char* msg);
    void debug(const int msg);
};

#endif
