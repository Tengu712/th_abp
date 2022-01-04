#include "_app.hpp"

bool SceneGame::init() {
    p_fbuf = p_app->createFrameBuffer(kSceWidth, kSceHeight);
    if (p_fbuf == nullptr)
        return false;
    camera.pos_z = -10.0f;
    return true;
}

void SceneGame::update() {
    if (p_app->getKey(KEY_CODE::Escape, KEY_STATE::Down)) {
        is_pause = !is_pause;
    }
    if (is_pause) {
        int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
        if (ud == 1)
            ++cur;
        else if (ud == -1)
            cur += 2;
        if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
            if (cur % 3 == 0)
                is_pause = false;
            else if (cur % 3 == 1) {
                p_app->changeScene(kSceneTitle);
                return;
            }
            else {
                p_app->restartGame();
                p_app->changeScene(kSceneGame);
                return;
            }
        } else if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
            is_pause = false;
        }
    } else
        updateGaming();

    Model model = Model();
    // ============= Objects ============= //
    p_app->drawBeginWithFrameBuffer(p_fbuf);
    drawBackGround();
    p_app->getEnemy()->draw();
    p_app->getPlayer()->draw();
    p_app->drawBulletPlayer();
    p_app->getPlayer()->drawSlow();
    p_app->getEnemy()->drawHPBar();
    p_app->getScoreManager()->draw();
    p_app->drawBeginWithFrameBuffer(nullptr);
    if (is_pause) {
        p_app->enableMosaic(true);
        SetModelColor(&model, 0xff555555);
    }
    model.scl_x = 1280.0f;
    model.scl_y = 960.0f;
    p_app->applyModel(&model);
    p_app->applyFrameBuffer(p_fbuf);
    p_app->drawIdea();
    p_app->enableMosaic(false);
    // ============= UI ============= //
    // Frame
    SetModelColor(&model, 0xffffffff);
    model.pos_x = 0.0f;
    model.pos_y = 0.0f;
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_UI_FRAME);
    p_app->drawIdea();
    // Pause option
    if (is_pause) {
        auto drawOption = [&](unsigned int mod_cur, unsigned int idx_str) {
            if (cur % 3 == mod_cur)
                SetModelColor(&model, 0xffffffff);
            else
                SetModelColor(&model, 0xff888888);
            p_app->drawString(p_app->getStr(kStrOption, idx_str), &model, kIdxNormal, 0);
            model.pos_y += 60.0f;
        };
        model.pos_x = 640.0f;
        model.pos_y = 380.0f;
        model.scl_y = 80.0f;
        p_app->drawString(p_app->getStr(kStrOption, 16), &model, kIdxNormal, 0);
        model.pos_y = 500.0f;
        model.scl_y = 60.0f;
        drawOption(0, 17);
        drawOption(1, 18);
        drawOption(2, 19);
        return;
    }
    // Logue
    lmanager.draw();
}

void SceneGame::updateGaming() {
    bool shotable = false;
    const unsigned int kChapter = p_app->getChapter();
    // Chapter 1 [0-10] : moving
    if (kChapter == 0) {

    }
    if (kChapter == 1)
        lmanager.set(0, -1, true);
    else if (kChapter == 2)
        lmanager.set(1, -1, false);
    else if (kChapter == 3)
        lmanager.set(2, 3, true);
    else if (kChapter == 4)
        lmanager.set(4, -1, false);
    else if (kChapter == 5)
        lmanager.set(5, -1, true);
    else if (kChapter == 6)
        lmanager.set(6, 7, true);
    else if (kChapter == 7)
        lmanager.set(8, 9, true);
    else if (kChapter == 8)
        lmanager.set(10, -1, false);
    else if (kChapter == 9) {
        p_app->getEnemy()->setHP(1000);
        p_app->getEnemy()->setMaxHP(1000);
        p_app->nextChapter();
        shotable = true;
    } else if (kChapter == 10) {
        shotable = true;
    }
    if (!shotable && p_app->getKey(KEY_CODE::Z, KEY_STATE::Down))
        p_app->nextChapter();
    InputInf iinf = InputInf();
    if (shotable) {
        iinf.z = p_app->getKey(KEY_CODE::Z, KEY_STATE::Pressed);
        iinf.x = p_app->getKey(KEY_CODE::X, KEY_STATE::Down);
    }
    iinf.dx = p_app->getKey(KEY_CODE::Right, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Left, KEY_STATE::Pressed);
    iinf.dy = p_app->getKey(KEY_CODE::Up, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Down, KEY_STATE::Pressed);
    iinf.s = p_app->getKey(KEY_CODE::Shift, KEY_STATE::Pressed);
    p_app->setInputInf(&iinf);
    p_app->getPlayer()->update();
    p_app->updateBulletPlayer();
    ++cnt;
}

void SceneGame::drawBackGround() {
    Model model = Model();
    const unsigned int kChapter = p_app->getChapter();
    if (kChapter < 50) {
        if (!is_pause) {
            camera.pos_y += 2.0f;
            if (camera.pos_y > 900.0f)
                camera.pos_y = 0.0f;
            camera.pos_x = 100.0f * (float)cos(Deg2Rad((double)cnt / 10.0));
        }
        p_app->applyCamera(&camera);
        p_app->applyImage(IMG_BG_MOUNTAIN);
        model.scl_x = 1000.0f;
        model.scl_y = 1000.0f;
        p_app->applyModel(&model);
        p_app->drawIdea();
        model.pos_y = 900.0f;
        p_app->applyModel(&model);
        p_app->drawIdea();
        model.pos_y = 1800.0f;
        p_app->applyModel(&model);
        p_app->drawIdea();
        p_app->applyCamera(nullptr);
    }
}
