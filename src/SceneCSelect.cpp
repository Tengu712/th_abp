#include "_app.hpp"

bool SceneCSelect::init() {
    return true;
}

void SceneCSelect::update() {
    int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
    if (cnt >= 0 && ud == 1)
        ++cur;
    if (cnt >= 0 && ud == -1)
        cur += 2;
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        //!
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
    model.pos_x = 640.0f;
    model.pos_y = 20.0f;
    model.scl_y = 80.0f;
    p_app->drawString(p_app->getStr(kStrCSelect, 0), &model, kIdxOption, 0);
    model.pos_y = 90.0f;
    model.scl_y = 50.0f;
    p_app->drawString(p_app->getStr(kStrCSelect, 1), &model, kIdxNormal, 0);
    model.pos_x = 100.0f;
    model.pos_y = 250.0f;
    auto drawOption = [&](unsigned int idx) {
        if (cur % 3 == idx / 3) {
            Model box = Model();
            box.pos_x = model.pos_x - 50.0f;
            box.pos_y = model.pos_y - 20.0f;
            box.scl_x = 512.0f * 1.4f;
            box.scl_y = 128.0f * 1.4f;
            box.col_a = (float)(0.6 + 0.3 * sin(Deg2Rad((double)cnt / 4.0)));
            p_app->applyModelUI(&box);
            p_app->applyImage(IMG_UI_CSBOX);
            p_app->drawIdea();
            ModelColorCode2RGBA(&model, 0xffffffff);
        }
        else
            ModelColorCode2RGBA(&model, 0xff666666);
        model.scl_y = 70.0f;
        p_app->drawString(p_app->getStr(kStrCSelect, idx), &model, kIdxNormal);
        model.scl_y = 40.0f;
        model.pos_x += 30.0f;
        model.pos_y += 60.0f;
        p_app->drawString(p_app->getStr(kStrCSelect, idx + 1), &model, kIdxNormal);
        model.pos_y += 30.0f;
        p_app->drawString(p_app->getStr(kStrCSelect, idx + 2), &model, kIdxNormal);
        model.pos_x -= 20.0f;
        model.pos_y += 110.0f;
    };
    drawOption(2);
    drawOption(5);
    drawOption(8);
    
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
