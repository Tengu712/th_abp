#include "_app.hpp"

bool SceneGame::init() {
    p_fbuf = p_app->createFrameBuffer(kSceWidth, kSceHeight);
    if (p_fbuf == nullptr)
        return false;
    p_app->setScore(0LL);
    p_app->setGraze(0U);
    p_app->getPlayer()->init(-1, 0.0, -260.0, -380.0, 380.0, -460.0, 350.0);
    p_app->getEnemy()->setPos(0.0, 300.0);
    p_app->clearBulletPlayer();
    return true;
}

void SceneGame::drawGame() {
    p_app->getEnemy()->draw();
    p_app->getPlayer()->draw();
    p_app->enableOverlay(true);
    p_app->drawBulletPlayer();
    p_app->enableOverlay(false);
    p_app->getPlayer()->drawSlow();
}

void SceneGame::drawUI() {
    Model model = Model();
    model.pos_x = 250.0f;
    model.pos_y = 30.0f;
    model.scl_y = 45.0f;
    char buf[64];
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%011lld", p_app->getScore());
    p_app->drawString(buf, &model, kIdxNormal);
    model.pos_x = 1030.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%011lld", p_app->getHiScore());
    p_app->drawString(buf, &model, kIdxNormal, 1);
    model.pos_x = 250.0f;
    model.pos_y += 40.0f;
    model.scl_y = 40.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%04.1lf", p_app->getRank());
    p_app->drawString(buf, &model, kIdxNormal);
    model.pos_x = 1030.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%d", p_app->getGraze());
    p_app->drawString(buf, &model, kIdxNormal, 1);
}

void SceneGame::drawOption() {
    Model model = Model();
    auto drawOption = [&](unsigned int idx) {
        if (cur % 3 == idx)
            ModelColorCode2RGBA(&model, 0xffffffff);
        else
            ModelColorCode2RGBA(&model, 0xff888888);
        p_app->drawString(p_app->getStr(kStrGame, idx + 1), &model, kIdxNormal, 0);
        model.pos_y += 60.0f;
    };
    model.pos_x = 640.0f;
    model.pos_y = 380.0f;
    model.scl_y = 80.0f;
    p_app->drawString(p_app->getStr(kStrGame, 0), &model, kIdxNormal, 0);
    model.pos_y = 500.0f;
    model.scl_y = 60.0f;
    drawOption(0);
    drawOption(1);
    drawOption(2);
}

void SceneGame::drawFrameBuffer() {
    if (is_pause)
        p_app->enableMosaic(true);
    Model model = Model();
    model.scl_x = 1280.0f;
    model.scl_y = 960.0f;
    p_app->applyModel(&model);
    p_app->applyFrameBuffer(p_fbuf);
    p_app->drawIdea();
    if (is_pause) {
        p_app->enableMosaic(false);
        ModelColorCode2RGBA(&model, 0x44000000);
        p_app->applyModel(&model);
        p_app->applyImage(0);
        p_app->drawIdea();
    }
}

void SceneGame::drawFrame() {
    Model model = Model();
    model.pos_x = 0.0f;
    model.pos_y = 0.0f;
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_UI_FRAME);
    p_app->drawIdea();
}

void SceneGame::updatePausing() {
    int ud = p_app->getKey(KEY_CODE::Down, KEY_STATE::Down) - p_app->getKey(KEY_CODE::Up, KEY_STATE::Down);
    if (ud == 1)
        ++cur;
    else if (ud == -1)
        cur += 2;
    if (p_app->getKey(KEY_CODE::Z, KEY_STATE::Down)) {
        if (cur % 3 == 0)
            is_pause = false;
        else if (cur % 3 == 1)
            p_app->changeScene(kSceneTitle);
        else
            p_app->changeScene(kSceneGame);
    } else if (p_app->getKey(KEY_CODE::X, KEY_STATE::Down)) {
        is_pause = false;
    }
}

void SceneGame::updateGaming() {
    InputInf iinf = InputInf();
    iinf.dx = p_app->getKey(KEY_CODE::Right, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Left, KEY_STATE::Pressed);
    iinf.dy = p_app->getKey(KEY_CODE::Up, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Down, KEY_STATE::Pressed);
    iinf.z = p_app->getKey(KEY_CODE::Z, KEY_STATE::Pressed);
    iinf.x = p_app->getKey(KEY_CODE::X, KEY_STATE::Down);
    iinf.s = p_app->getKey(KEY_CODE::Shift, KEY_STATE::Pressed);
    p_app->setInputInf(&iinf);
    p_app->getPlayer()->update();
    p_app->updateBulletPlayer();
}

void SceneGame::update() {
    if (p_app->getKey(KEY_CODE::Escape, KEY_STATE::Down)) {
        is_pause = !is_pause;
    }
    if (is_pause)
        updatePausing();
    else 
        updateGaming();
    p_app->drawBeginWithFrameBuffer(p_fbuf);
    drawGame();
    drawUI();
    p_app->drawBeginWithFrameBuffer(nullptr);
    drawFrameBuffer();
    if (is_pause)
        drawOption();
    drawFrame();
}
