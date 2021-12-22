#include "_app.hpp"

bool SceneGame::init() {
    return true;
}

void SceneGame::update() {
    Model model = Model();
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_UI_FRAME);
    p_app->drawIdea();
}