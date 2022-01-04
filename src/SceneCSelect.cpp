#include "_app.hpp"

bool SceneCSelect::init() {
    p_app->getPlayer()->init(0U, 330.0, -220.0, -640.0, 640.0, -480.0, 480.0);
    p_app->getEnemy()->setPos(330.0, 120.0);
    return true;
}

void SceneCSelect::update() {
    int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
    if (cnt_all >= 0 && ud == 1) 
        ++cur;
    if (cnt_all >= 0 && ud == -1)
        cur += 2;
    if (cnt_all >= 0 && ud != 0) {
        p_app->getPlayer()->init(cur % 3, 330.0, -220.0, -640.0, 640.0, -480.0, 480.0);
        p_app->getPlayer()->setPos(330.0, -230.0);
        p_app->getPlayer()->run();
        cnt_player = 0U;
    }
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        cnt_all = -1;
    }
    if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
        p_app->changeScene(kSceneTitle);
    }

    if (cnt_all < -10) {
        p_app->initGame(0, 20.0, false);
        p_app->changeScene(kSceneGame);
        return;
    }

    InputInf iinf = InputInf();
    iinf.z = 1;
    if (cnt_player % 100 < 20)
        iinf.dx = 0;
    else if (cnt_player % 100 < 40)
        iinf.dx = 1;
    else if (cnt_player % 100 < 80)
        iinf.dx = -1;
    else
        iinf.dx = 1;
    if (cnt_player % 200 < 100)
        iinf.s = 0;
    else
        iinf.s = 1;
    p_app->setInputInf(&iinf);
    p_app->getPlayer()->update();
    p_app->updateBulletPlayer();

    Model model = Model();
    // Back ground
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
    // Player
    p_app->getPlayer()->draw();
    // Enemy
    model.pos_x = 330.0f;
    model.pos_y = 120.0f;
    model.scl_x = 64.0f;
    model.scl_y = 64.0f;
    model.deg_z = cnt_all;
    p_app->applyModel(&model);
    p_app->applyImage(IMG_CH_ENEMY);
    p_app->drawIdea();
    model.deg_z = 0.0f;
    // Bullets
    p_app->enableOverlay(true);
    p_app->drawBulletPlayer();
    p_app->enableOverlay(false);
    // Slow
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
    p_app->drawString(p_app->getStr(kStrOption, 5), &model, kIdxOption, 0);
    model.pos_y = 100.0f;
    model.scl_y = 50.0f;
    p_app->drawString(p_app->getStr(kStrOption, 6), &model, kIdxNormal, 0);
    model.pos_x = 100.0f;
    model.pos_y = 250.0f;
    auto drawOption = [&](unsigned int mod_cur, unsigned int idx_str) {
        if (cur % 3 == mod_cur) {
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
            SetModelColor(&model, 0xff666666);
        model.scl_y = 70.0f;
        p_app->drawString(p_app->getStr(kStrOption, idx_str), &model, kIdxNormal);
        model.scl_y = 40.0f;
        model.pos_x += 30.0f;
        model.pos_y += 60.0f;
        p_app->drawString(p_app->getStr(kStrOption, idx_str + 1), &model, kIdxNormal);
        model.pos_y += 30.0f;
        p_app->drawString(p_app->getStr(kStrOption, idx_str + 2), &model, kIdxNormal);
        model.pos_x -= 30.0f;
        model.pos_y += 110.0f;
    };
    drawOption(0, 7);
    drawOption(1, 10);
    drawOption(2, 13);
    
    if (cnt_all < 30) {
        model.pos_x = 0.0f;
        model.pos_y = 0.0f;
        model.scl_x = 1280.0f;
        model.scl_y = 1280.0f;
        SetModelColor(&model, 0xff000000);
        model.col_a = cnt_all < 0 ? (float)(-cnt_all) / 10.0f : 1.0f - (float)cnt_all / 30.0f;
        p_app->applyModel(&model);
        p_app->applyImage(0);
        p_app->drawIdea();
    }

    if (cnt_all < 0)
        --cnt_all;
    else
        ++cnt_all;
    ++cnt_player;
}
