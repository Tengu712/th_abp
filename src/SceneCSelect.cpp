#include "_app.hpp"

bool SceneCSelect::init() {
    p_app->getPlayer()->x = 330.0;
    p_app->getPlayer()->y = -220.0;
    p_app->getPlayer()->run();
    p_app->getEnemy()->x = 330.0;
    p_app->getEnemy()->y = 120.0;
    return true;
}

void SceneCSelect::update() {
    int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
    if (cnt_all >= 0 && ud == 1) 
        ++cur;
    if (cnt_all >= 0 && ud == -1)
        cur += 2;
    if (cnt_all >= 0 && ud != 0) {
        p_app->initPlayer(cur % 3);
        p_app->getPlayer()->x = 330.0;
        p_app->getPlayer()->y = -230.0;
        p_app->getPlayer()->run();
        cnt_player = 0U;
    }
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        //!
    }
    if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
        p_app->changeScene(kSceneTitle);
    }

    InputInf iinf = InputInf();
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
        iinf.s = 1;
    if (cnt_player % 60 < 55)
        iinf.z = 1;
    else
        iinf.z = 0;

    p_app->setInputInf(&iinf);
    p_app->getPlayer()->update();
    p_app->updateBulletPlayer();


    Model model = Model();
    p_app->applyImage(IMG_BG_CS_SCROLL);
    model.scl_x = 700.0f;
    model.scl_y = 700.0f;
    model.pos_x = 330.0f;
    model.pos_y = (float)(cnt_all % 140) * -5.0f;
    p_app->applyModel(&model);
    p_app->drawIdea();
    model.pos_y += 700.0f;
    p_app->applyModel(&model);
    p_app->drawIdea();
    p_app->getPlayer()->draw();
    p_app->enableOverlay(true);
    p_app->drawBulletPlayer();
    p_app->enableOverlay(false);
    p_app->getPlayer()->drawSlow();

    model.pos_x = 0.0f;
    model.pos_y = 0.0f;
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
            box.pos_x = model.pos_x;
            box.pos_y = model.pos_y + 10.0f;
            box.scl_x = 512.0f;
            box.scl_y = 128.0f * 0.95f;
            p_app->applyModelUI(&box);
            p_app->applyImage(IMG_UI_CSBOX);
            p_app->drawIdea();
            const float col = (float)(0.7 + 0.3 * fabs(sin(Deg2Rad((double)cnt_all * 4.0))));
            model.col_r = col;
            model.col_g = col;
            model.col_b = col;
            model.col_a = 1.0f;
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
        model.pos_x -= 30.0f;
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
