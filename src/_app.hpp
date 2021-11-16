#pragma once
#ifndef _APP_
#define _APP_

#include "../directx/_dx11public.hpp"
#pragma comment(lib, "mydx.lib")

constexpr unsigned int kSceWidth = 1280U;
constexpr unsigned int kSceHeight = 960U;

class App;

struct Fact {
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
    Fact() :
        posX(0.0f),
        posY(0.0f),
        posZ(0.0f),
        degX(0.0f),
        degY(0.0f),
        degZ(0.0f),
        sclX(1.0f),
        sclY(1.0f),
        sclZ(1.0f),
        colR(1.0f),
        colG(1.0f),
        colB(1.0f),
        colA(1.0f)
    {}
};

class Scene {
protected:
    App* pApp;
public:
    Scene(App* pApp) : pApp(pApp) {}
    virtual bool init() = 0;
    virtual void update() = 0;
};

class SceneTitle : public Scene {
private:
    Camera camera;
public:
    SceneTitle(App* pApp) : Scene(pApp), camera(Camera()) {}
    bool init();
    void update();
};

struct AppInf;

class App {
private:
    AppInf* pInf;
public:
    App();
    ~App();
    bool init(HINSTANCE hInst, LPSTR pCmd, int cmdShow);
    bool update();
    void drawIdea();
    void applyFact(Fact* pFact);
    void createCamera(float width, float height, Camera* pCamera);
    void applyCamera(Camera* pCamera, bool parse);
    bool createConsole();
    void debug(const char* msg);
    void debug(const int msg);
};

#endif
