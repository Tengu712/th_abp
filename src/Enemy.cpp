#include "_app.hpp"

int Enemy::getHP() {
    return hp;
}

int Enemy::getMaxHP() {
    return hp_max;
}

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
