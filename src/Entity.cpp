#include "_app.hpp"

// ================================================================================================================= //
//                                              Entity                                                               //
// ================================================================================================================= //

void Entity::run() {
    moving = true;
    existing = true;
}

void Entity::move() {
    if (!(moving && existing))
        return;
    const double rad = (double)deg / 180.0 * PI;
    x += cos(rad) * spd;
    y += sin(rad) * spd;
}

// ================================================================================================================= //
//                                              Option                                                               //
// ================================================================================================================= //

void Option::draw() {
    Model model = Model();
    model.pos_x = (float)x;
    model.pos_y = (float)y;
    model.scl_x = 24.0f;
    model.scl_y = 24.0f;
    model.deg_z = (float)deg - 90.0f;
    p_app->applyModel(&model);
    p_app->applyImage(IMG_CH_OPTION0 + ((cnt / 4) % 2));
    p_app->drawIdea();
    ++cnt;
}

// ================================================================================================================= //
//                                              Player                                                               //
// ================================================================================================================= //

const int kDegs[3][3] = {{225, 180, 135}, {270, 999, 90}, {315, 0, 45}};

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
    if (id_weapon == 0) {
        opt_x_03 = max(min(iinf.s * 20.0 + 60.0, 100.0), max(opt_x_03 - 20.0, 60.0));
        opt_x_12 = max(min(iinf.s * 10.0 + 30.0, 50.0), max(opt_x_12 - 10.0, 30.0));
        opt_y_12 = max(min(iinf.s * 30.0, 60.0), max(opt_y_12 - 30.0, 0.0));
        options[0].x = x - opt_x_03;
        options[0].y = y;
        options[3].x = x + opt_x_03;
        options[3].y = y;
        options[1].x = x - opt_x_12;
        options[1].y = y + opt_y_12 - 50.0;
        options[2].x = x + opt_x_12;
        options[2].y = y + opt_y_12 - 50.0;
        if (iinf.s > 0) {
        } else {
            options[0].deg = 110.0;
            options[1].deg = 100.0;
            options[2].deg = 80.0;
            options[3].deg = 70.0;
        }
    } else if (id_weapon == 1) {
        opt_x_12 = max(min(iinf.s / 5.0, 1.0), max(opt_x_12 - 0.2, 0.0));
        opt_y_12 = max(min(iinf.s / 5.0, 1.0), max(opt_y_12 - 0.2, 0.0));
        options[0].x = x;
        options[0].y = y + 70.0;
        options[0].deg = 90.0;
        for (int i = 1; i < 4; ++i) {
            options[i].x = x + 80.0 * sin(Deg2Rad((double)cnt_all * 4.0 + (double)i * 120.0)) * (1.0 - opt_x_12);
            options[i].y = y + 60.0 + 10.0 * opt_y_12;
            options[i].deg = 90.0;
        }
    }
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
    for (int i = 0; i < 4; ++i) {
        options[i].draw();
    }
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
