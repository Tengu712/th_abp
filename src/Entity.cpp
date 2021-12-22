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

const int kDegs[3][3] = {{225, 180, 135}, {270, 999, 90}, {315, 0, 45}};

void Player::init(App* p_app, unsigned int id_weapon) {
    this->p_app = p_app;
    this->id_weapon = id_weapon;
    moving = true;
    existing = true;
}

void Player::setInputInf(InputInfPlayer* p_iinf) {
    iinf.dx = p_iinf->dx;
    iinf.dy = p_iinf->dy;
    iinf.z = p_iinf->z > 0 ? iinf.z + 1 : 0;
    iinf.x = p_iinf->x > 0 ? 1 : 0;
    iinf.s = p_iinf->s > 0 ? iinf.s + 1 : 0;
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
    ++cnt_all;
}

void Player::draw() {
    Model model = Model();
    model.pos_x = (float)x;
    model.pos_y = (float)y;
    model.scl_x = 86.0f;
    model.scl_y = 86.0f;
    p_app->applyModel(&model);
    p_app->applyImage(IMG_CH_KOSUZU_B0 + ((cnt_all / 6) % 4));
    p_app->drawIdea();
}

void Player::drawSlow() {
    Model model = Model();
    model.pos_x = (float)x;
    model.pos_y = (float)y;
    if (iinf.s > 10) {
        model.scl_x = 120.0f;
        model.scl_y = 120.0f;
        model.deg_z = iinf.s * 4;
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_SLOWCIRCLE);
        p_app->drawIdea();
        model.deg_z *= -1;
        p_app->applyModel(&model);
        p_app->drawIdea();
    } else if (iinf.s > 0) {
        model.scl_x = 420.0f - 300.0f * (float)iinf.s / 10.0f;
        model.scl_y = model.scl_x;
        model.col_a = (float)iinf.s / 10.0f;
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_SLOWCIRCLE);
        p_app->drawIdea();
    }
    if (iinf.s > 0) {
        model.scl_x = 16.0f;
        model.scl_y = 16.0f;
        model.deg_z = iinf.s * 3;
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_ATARI);
        p_app->drawIdea();
    }
}

// ================================================================================================================= //
//                                              Bullet                                                               //
// ================================================================================================================= //
