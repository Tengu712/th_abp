#include "_app.hpp"

bool SceneTitle::init() {
    return true;
}

void SceneTitle::update() {
    pApp->applyCamera(nullptr);
    Model f = Model();
    f.sclX = 10.0f;
    f.sclY = 10.0f;
    pApp->applyModel(&f);
    pApp->drawIdea();
    cnt++;
}
