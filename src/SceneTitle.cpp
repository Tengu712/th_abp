#include "_app.hpp"

bool SceneTitle::init() {
    camera.posZ = -10.0f;
    return true;
}

void SceneTitle::update() {
    pApp->applyCamera(&camera);
    Model f = Model();
    f.sclX = 10.0f;
    f.sclY = 10.0f;
    f.sclZ = 10.0f;
    pApp->applyModel(&f);
    pApp->drawIdea();
}
