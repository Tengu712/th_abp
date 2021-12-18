﻿#include "_app.hpp"

bool SceneTitle::init() {
    return true;
}

void SceneTitle::update() {
    int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
    if (cnt >= 0 && ud == 1)
        ++cur;
    if (cnt >= 0 && ud == -1)
        cur += 4;
    if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
        cur = 4;
    }
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        cnt = -1;
    }

    if (cnt < -10) {
        if (cur % 5 == 0)
            p_app->changeScene(kSceneCSelect);
        if (cur % 5 == 4)
            p_app->changeScene(kSceneExit);
    }

    Model model = Model();
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_BG_TITLE);
    p_app->drawIdea();
    model.pos_x = 30.0f;
    model.pos_y = 500.0f;
    auto drawOption = [&](const char* str, int i) {
        model.scl_x = 40.0f;
        model.scl_y = 60.0f;
        ModelColorCode2RGBA(&model, 0xaa303030);
        if (cur % 5 == i) {
            model.pos_y -= 5.0f;
            model.scl_x += 8.0f;
            model.scl_y += 10.0f;
            const unsigned int cnt_new = cnt * 3;
            model.col_r = (float)(0.40 + 0.6 * fabs(sin(Deg2Rad(cnt_new))));
            model.col_g = (float)(0.06 + 0.2 * fabs(sin(Deg2Rad(cnt_new))));
            model.col_b = (float)(0.06 + 0.2 * fabs(sin(Deg2Rad(cnt_new))));
            model.col_a = 1.0f;
        }
        p_app->drawString(&model, -1, kIdxElephant, str);
        if (cur % 5 == i)
            model.pos_y += 5.0f;
        model.pos_x += 20.0f;
        model.pos_y += 70.0f;
    };
    drawOption("Start", 0);
    drawOption("Chapter", 1);
    drawOption("Score", 2);
    drawOption("Replay", 3);
    drawOption("Quit", 4);
    model.pos_x = 640.0f;
    model.pos_y = 933.0f;
    model.scl_x = 14.0f;
    model.scl_y = 22.0f;
    ModelColorCode2RGBA(&model, 0xffffffff);
    p_app->drawString(&model, 0, kIdxElephant, "SkyDog Assoc Work 2022");
    if (cnt < 30) {
        model.pos_x = 0.0f;
        model.pos_y = 0.0f;
        model.scl_x = 1280.0f;
        model.scl_y = 1280.0f;
        ModelColorCode2RGBA(&model, cnt < 0 ? 0x00000000 : 0xffffffff);
        model.col_a = cnt < 0 ? (float)(-cnt) / 10.0f : 1.0f - (float)cnt / 30.0f;
        p_app->applyModel(&model);
        p_app->applyImage(0);
        p_app->drawIdea();
    }

    if (cnt < 0)
        --cnt;
    else
        ++cnt;
}
