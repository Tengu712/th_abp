#include "_app.hpp"

bool SceneTitle::init() {
    return true;
}

void SceneTitle::update() {
    p_app->applyCamera(nullptr);
    Model f = Model();
    f.scl_x = 1280.0f;
    f.scl_y = 1280.0f;
    p_app->applyModelUI(&f);
    p_app->applyImage(IMG_BG_TITLE);
    p_app->drawIdea();
    cnt++;
}
