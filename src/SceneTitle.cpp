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
    f.scl_x = 60.0f;
    f.scl_y = 80.0f;
    ModelColorCode2RGBA(&f, 0xff09073f);
    p_app->drawString(&f, -1, kIdxElephant, "Start");
    cnt++;
}
