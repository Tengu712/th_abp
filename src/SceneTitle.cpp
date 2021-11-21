#include "_app.hpp"

bool SceneTitle::init() {
    camera.posZ = -10.0f;
    return true;
}

void SceneTitle::update() {
    pApp->applyCamera(&camera);
    Model f = Model();
    pApp->applyModel(&f);
    pApp->drawIdea();
    cnt++;
}
