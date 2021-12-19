#include "_app.hpp"

bool SceneCharacterSelect::init() {
    return true;
}

void SceneCharacterSelect::update() {
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        p_app->changeScene(kSceneGame);
    }
    if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
        p_app->changeScene(kSceneTitle);
    }

    Model model = Model();
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_BG_CSELECT);
    p_app->drawIdea();
    model.pos_x = 380.0f;
    model.pos_y = 20.0f;
    model.scl_y = 80.0f;
    p_app->drawString(&model, kIdxElephant, "Weapon Select");
    
    if (cnt >= 0 && cnt < 30) {
        model.pos_x = 0.0f;
        model.pos_y = 0.0f;
        model.scl_x = 1280.0f;
        model.scl_y = 1280.0f;
        ModelColorCode2RGBA(&model, 0xff000000);
        model.col_a = 1.0f - (float)cnt / 30.0f;
        p_app->applyModel(&model);
        p_app->applyImage(0);
        p_app->drawIdea();
    }

    ++cnt;
}
