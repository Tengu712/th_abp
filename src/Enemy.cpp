#include "_app.hpp"

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
