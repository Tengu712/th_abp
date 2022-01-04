#include "_app.hpp"

void Enemy::setHP(int hp) {
    this->hp = hp;
}

void Enemy::setMaxHP(int hp_max) {
    this->hp_max = hp_max;
}

void Enemy::transHP(int dhp) {
    hp += dhp;
}

void Enemy::draw() {
    Model model = Model();
    model.pos_x = (float)x;
    model.pos_y = (float)y;
    model.scl_x = 100.0f;
    model.scl_y = 100.0f;
    p_app->applyModel(&model);
    p_app->applyImage(IMG_CH_ENEMY);
    p_app->drawIdea();
}

void Enemy::drawHPBar() {
    if (hp_max <= 0)
        return;
    Model model = Model();
    model.scl_x = 700.0f * max(hp, 0.0f) / hp_max;
    model.scl_y = 5.0f;
    model.pos_x = model.scl_x / 2.0f - 350.0f;
    model.pos_y = 450.0f;
    p_app->applyModel(&model);
    p_app->applyImage(0);
    p_app->drawIdea();
}
