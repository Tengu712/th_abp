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

void Entity::setModelPosDeg(Model* p_model) {
    p_model->pos_x = (float)x;
    p_model->pos_y = (float)y;
    p_model->deg_z = (float)deg - 90.0f;
}

// ================================================================================================================= //
//                                              Option                                                               //
// ================================================================================================================= //

void OptionManager::update(unsigned int id_weapon) {
    const int kSlow = p_app->getInputInf()->s;
    const double kPX = p_app->getPlayer()->x;
    const double kPY = p_app->getPlayer()->y;
    if (id_weapon == 0) {
        prevs[0] = max(min(kSlow * 20.0 + 60.0, 100.0), max(prevs[0] - 20.0, 60.0));
        prevs[1] = max(min(kSlow * 10.0 + 30.0, 50.0), max(prevs[1] - 10.0, 30.0));
        prevs[2] = max(min(kSlow * 30.0, 60.0), max(prevs[2] - 30.0, 0.0));
        options[0].x = kPX - prevs[0];
        options[3].x = kPX + prevs[0];
        options[0].y = kPY;
        options[3].y = kPY;
        options[1].x = kPX - prevs[1];
        options[2].x = kPX + prevs[1];
        options[1].y = kPY + prevs[2] - 50.0;
        options[2].y = kPY + prevs[2] - 50.0;
        if (kSlow > 0) {
        } else {
            options[0].deg = 110.0;
            options[1].deg = 100.0;
            options[2].deg = 80.0;
            options[3].deg = 70.0;
        }
    } else if (id_weapon == 1) {
        prevs[1] = max(min(kSlow / 5.0, 1.0), max(prevs[1] - 0.2, 0.0));
        prevs[2] = max(min(kSlow / 5.0, 1.0), max(prevs[2] - 0.2, 0.0));
        options[0].x = kPX;
        options[0].y = kPY + 70.0;
        options[0].deg = 90.0;
        for (int i = 1; i < 4; ++i) {
            options[i].x = kPX + 80.0 * sin(Deg2Rad((double)cnt * 4.0 + (double)i * 120.0)) * (1.0 - prevs[1]);
            options[i].y = kPY + 60.0 + 10.0 * prevs[2];
            options[i].deg = 90.0;
        }
    } else {
        prevs[1] = max(min(kSlow * 10.0, 20.0), max(prevs[1] - 10.0, 0.0));
        prevs[2] = max(min(kSlow * 10.0, 20.0), max(prevs[2] - 10.0, 0.0));
        options[0].x = kPX + prevs[1] - 60.0;
        options[3].x = kPX - prevs[1] + 60.0;
        options[0].y = kPY + prevs[2];
        options[3].y = kPY + prevs[2];
        options[1].x = kPX + prevs[1] - 40.0;
        options[2].x = kPX - prevs[1] + 40.0;
        options[1].y = kPY + 50.0;
        options[2].y = kPY + 50.0;
        if (kSlow > 0) {
            for (int i = 0; i < 4; ++i) {
                options[i].deg = 90.0;
            }
        } else {
            options[0].deg = 100.0;
            options[1].deg = 90.0;
            options[2].deg = 90.0;
            options[3].deg = 80.0;
        }
    }
    ++cnt;
}

void OptionManager::draw() {
    Model model = Model();
    model.scl_x = 24.0f;
    model.scl_y = 24.0f;
    for (int i = 0; i < 4; ++i) {
        options[i].setModelPosDeg(&model);
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_OPTION0 + ((cnt / 4) % 2));
        p_app->drawIdea();
    }
}

// ================================================================================================================= //
//                                              Player                                                               //
// ================================================================================================================= //

const int kDegs[3][3] = {{225, 180, 135}, {270, 999, 90}, {315, 0, 45}};

void Player::update() {
    int dxdy = abs(p_app->getInputInf()->dx) + abs(p_app->getInputInf()->dy);
    deg = kDegs[p_app->getInputInf()->dx + 1][p_app->getInputInf()->dy + 1];
    if (dxdy == 0)
        spd = 0.0;
    else {
        spd = id_weapon == 1 ? 6.0 : 4.0;
        spd *= dxdy == 1 ? 1.0 : sqrt(2.0);
        spd *= p_app->getInputInf()->s > 0 ? 0.5 : 1.0;
    }
    move();
    omanager.update(id_weapon);
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
    omanager.draw();
}

void Player::drawSlow() {
    const int kSlow = p_app->getInputInf()->s;
    Model model = Model();
    model.pos_x = (float)x;
    model.pos_y = (float)y;
    if (kSlow > 10) {
        model.scl_x = 120.0f;
        model.scl_y = 120.0f;
        model.deg_z = kSlow * 4;
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_SLOWCIRCLE);
        p_app->drawIdea();
        model.deg_z *= -1;
        p_app->applyModel(&model);
        p_app->drawIdea();
    } else if (kSlow > 0) {
        model.scl_x = 420.0f - 300.0f * (float)kSlow / 10.0f;
        model.scl_y = model.scl_x;
        model.col_a = (float)kSlow / 10.0f;
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_SLOWCIRCLE);
        p_app->drawIdea();
    }
    if (kSlow > 0) {
        model.scl_x = 16.0f;
        model.scl_y = 16.0f;
        model.deg_z = kSlow * 3;
        p_app->applyModel(&model);
        p_app->applyImage(IMG_CH_ATARI);
        p_app->drawIdea();
    }
}

// ================================================================================================================= //
//                                              Bullet                                                               //
// ================================================================================================================= //
