#include "../directx/HeaderDX11.hpp"
#pragma comment(lib, "mydx.lib")

#include "resource.hpp"

class App;

class Scene {
protected:
    App* pApp;
public:
    Scene(App* pApp) : pApp(pApp) {}
    virtual void update() = 0;
};

class SceneTitle : public Scene {
public:
    SceneTitle(App* pApp) : Scene(pApp) {}
    void update();
};

class App {
private:
    D3DManager dmanager;
    InputManager imanager;
    bool ableDebug;
    Scene* pScene;

public:
    App() : 
        dmanager(D3DManager()),
        imanager(InputManager()),
        ableDebug(false),
        pScene(nullptr)
    {}
    ~App()
    {
        if (pScene != nullptr)
            delete pScene;
    }
    bool init(HINSTANCE hInst, LPSTR pCmd, int cmdShow);
    bool update();
    bool createConsole();
    void debug(const char* msg);
};
