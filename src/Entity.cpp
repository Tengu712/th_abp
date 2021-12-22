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

void Player::init(App* p_app, unsigned int id_weapon) {
    this->p_app = p_app;
    this->id_weapon = id_weapon;
    moving = true;
    existing = true;
}

void Player::setInputInf(InputInfPlayer* p_iinf) {
    iinf = *p_iinf;
}

void Player::update() {
    int dxdy = abs(iinf.dx) + abs(iinf.dy);
    deg = kDegs[iinf.dx + 1][iinf.dy + 1];
    if (dxdy == 0)
        spd = 0.0;
    else {
        spd = id_weapon == 1 ? 6.0 : 4.0;
        spd *= dxdy == 1 ? 1.0 : sqrt(2.0);
        spd *= iinf.s > 0 ? 0.5 : 1.0;
    }
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
