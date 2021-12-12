#include "_app.hpp"

bool SceneTitle::init() {
    return true;
}

void SceneTitle::update() {
    pApp->applyCamera(nullptr);
    Model f = Model();
    f.sclX = 1280.0f;
    f.sclY = 960.0f;
    pApp->applyModel(&f);
    pApp->applyImage(IMG_BG_TITLE);
    pApp->drawIdea();
    cnt++;
}
