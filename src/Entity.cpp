#include "_app.hpp"

// ================================================================================================================= //
//                                              Entity                                                               //
// ================================================================================================================= //

void Entity::move() {
    if (!(moving && existing))
        return;
    const double rad = (double)deg / 180.0 * PI;
    x += cos(rad) * spd;
    y += sin(rad) * spd;
}

// ================================================================================================================= //
//                                              Player                                                               //
// ================================================================================================================= //

const int kDegs[3][3] = {
    {225, 180, 135},
    {270, 999, 90},
    {315, 0, 45}
};

void Player::init(App* p_app) {
    this->p_app = p_app;
    moving = true;
    existing = true;
}

void Player::update() {
    const int dx = p_app->getKey(KEY_CODE::Right, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Left, KEY_STATE::Pressed);
    const int dy = p_app->getKey(KEY_CODE::Up, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Down, KEY_STATE::Pressed);
    int dxdy = abs(dx) + abs(dy);
    deg = kDegs[dx + 1][dy + 1];
    if (dxdy == 0)
        spd = 0.0;
    else if (dxdy == 1)
        spd = 4.0; //!
    else
        spd = 4.0 * sqrt(2.0); //!
    move();
}

void Player::draw() {
    Model model = Model();
    model.pos_x = (float)x;
    model.pos_y = (float)y;
    model.scl_x = 80.0f;
    model.scl_y = 80.0f;
    p_app->applyModel(&model);
    p_app->applyImage(IMG_CH_KOSUZU_B0);
    p_app->drawIdea();
}

// ================================================================================================================= //
//                                              Bullet                                                               //
// ================================================================================================================= //
