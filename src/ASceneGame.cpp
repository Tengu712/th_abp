#include "_app.hpp"

bool ASceneGame::init() {
    p_app->getPlayer()->init(-1, 0.0, -260.0, -380.0, 380.0, -460.0, 300.0);
    p_app->getEnemy()->setPos(0.0, 300.0);
    return true;
}

void ASceneGame::updateGame() {
    p_app->getPlayer()->update();
    p_app->updateBulletPlayer();
}

void ASceneGame::drawGame() {
    Model model = Model();
    // Enemy
    p_app->getEnemy()->draw();
    // Player
    p_app->getPlayer()->draw();
    // Bullets
    p_app->enableOverlay(true);
    p_app->drawBulletPlayer();
    p_app->enableOverlay(false);
    // Slow
    p_app->getPlayer()->drawSlow();
}

void ASceneGame::drawUI() {
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
    model.pos_x = 0.0f;
    model.pos_y = 0.0f;
    model.scl_x = 1280.0f;
    model.scl_y = 1280.0f;
    p_app->applyModelUI(&model);
    p_app->applyImage(IMG_UI_FRAME);
    p_app->drawIdea();
}