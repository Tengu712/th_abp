﻿#include "_app.hpp"

bool SceneTitle::init() {
    return true;
}

void SceneTitle::update() {

    p_app->applyCamera(nullptr);
    Model model = Model();
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_BG_TITLE);
    p_app->drawIdea();
    model.pos_x = 30.0f;
    model.pos_y = 500.0f;
    model.scl_x = 40.0f;
    model.scl_y = 60.0f;
    auto drawOption = [&](const char* str, int i) {
        ModelColorCode2RGBA(&model, 0xbb434343);
        if (cur % 5 == i) {
            const unsigned int cnt_new = cnt * 2;
            model.col_r = (float)(0.36 + 0.4 * fabs(sin(Deg2Rad(cnt_new))));
            model.col_g = (float)(0.09 + 0.4 * fabs(sin(Deg2Rad(cnt_new))));
            model.col_b = (float)(0.09 + 0.4 * fabs(sin(Deg2Rad(cnt_new))));
            model.col_a = 1.0f;
        }
        p_app->drawString(&model, -1, kIdxElephant, str);
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
        model.scl_y = 960.0f;
        ModelColorCode2RGBA(&model, 0xffffffff);
        model.col_a = 1.0f - (float)cnt / 30.0f;
        p_app->applyImage(0);
        p_app->applyModel(&model);
        p_app->drawIdea();
    }

    ++cnt;
}
