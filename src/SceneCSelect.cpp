#include "_app.hpp"

bool SceneCSelect::init() {
    player.init(p_app, 0);
    player.x = 330.0;
    player.y = -220.0;
    return true;
}

void SceneCSelect::update() {
    int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
    if (cnt_all >= 0 && ud == 1) 
        ++cur;
    if (cnt_all >= 0 && ud == -1)
        cur += 2;
    if (cnt_all >= 0 && ud != 0) {
        player = Player();
        player.init(p_app, cur % 3);
        player.x = 330.0;
        player.y = -220.0;
        cnt_player = 0U;
    }
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        //!
    }
    if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
        p_app->changeScene(kSceneTitle);
    }

    InputInfPlayer iinf = InputInfPlayer();
    if (cnt_player % 180 < 60)
        iinf.dx = 0;
    else if (cnt_player % 180 < 90)
        iinf.dx = 1;
    else if (cnt_player % 180 < 150)
        iinf.dx = -1;
    else
        iinf.dx = 1;
    if (cnt_player % 360 < 180)
        iinf.s = 0;
    else
        ++iinf.s;
    player.setInputInf(&iinf);
    player.update();
    player.draw();

    Model model = Model();
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_BG_CSELECT);
    p_app->drawIdea();
    model.pos_x = 640.0f;
    model.pos_y = 30.0f;
    model.scl_y = 80.0f;
    p_app->drawString(p_app->getStr(kStrCSelect, 0), &model, kIdxOption, 0);
    model.pos_y = 100.0f;
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
            box.col_a = (float)(0.6 + 0.3 * sin(Deg2Rad((double)cnt_all / 4.0)));
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
    
    if (cnt_all >= 0 && cnt_all < 30) {
        model.pos_x = 0.0f;
        model.pos_y = 0.0f;
        model.scl_x = 1280.0f;
        model.scl_y = 1280.0f;
        ModelColorCode2RGBA(&model, 0xff000000);
        model.col_a = 1.0f - (float)cnt_all / 30.0f;
        p_app->applyModel(&model);
        p_app->applyImage(0);
        p_app->drawIdea();
    }

    ++cnt_all;
    ++cnt_player;
}
