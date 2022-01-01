#include "_app.hpp"

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
    if (p_app->getInputInf()->z > 0) {
        if (id_weapon == 0 && cnt_all % 6 == 0) {
            Bullet bul = Bullet();
            bul.setSpd(30.0);
            if (p_app->getInputInf()->s > 0)
                bul.init(p_app, IMG_BU_JIKI_HARI, 0, 10, 0xff8888ff, 0);
            else
                bul.init(p_app, IMG_BU_JIKI_HARI, 0, 10, 0xffffffff, 0);
            for (int i = 0; i < 4; ++i) {
                bul.setPos(omanager.options[i].getX(), omanager.options[i].getY() + 10.0);
                bul.setDeg(omanager.options[i].getDeg());
                p_app->pushBulletPlayer(&bul);
            }
        } else if (id_weapon == 1 && p_app->getInputInf()->s > 0) {
            Bullet bul = Bullet();
            bul.setDeg(90.0);
            bul.setPos(x, y + 90.0);
            bul.init(p_app, IMG_BU_LAZER, 0, 1, 0xff65d5ff, 0);
            p_app->pushBulletPlayer(&bul);
        } else if (id_weapon == 1 && cnt_all % 4 == 0) {
            Bullet bul = Bullet();
            bul.setPos(omanager.options[0].getX(), omanager.options[0].getY() + 10.0);
            bul.setSpd(34.0);
            bul.setDeg(90.0);
            if (cnt_all % 8 == 0) {
                bul.init(p_app, IMG_BU_JIKI_BIGHARI, 0, 8, 0xff8888ff, 0);
                p_app->pushBulletPlayer(&bul);
            }
            bul.init(p_app, IMG_BU_JIKI_BIGHARI, 0, 4, 0xffffffff, 0);
            for (int i = 1; i < 4; ++i) {
                bul.setPos(omanager.options[i].getX(), omanager.options[i].getY() + 10.0);
                p_app->pushBulletPlayer(&bul);
            }
        } else if (id_weapon == 2 && cnt_all % 6 == 0) {
            Bullet bul = Bullet();
            bul.setSpd(30.0);
            if (p_app->getInputInf()->s > 0)
                bul.init(p_app, IMG_BU_JIKI_HARI, 0, 14, 0xff8888ff, 0);
            else
                bul.init(p_app, IMG_BU_JIKI_HARI, 0, 12, 0xffffffff, 0);
            for (int i = 0; i < 4; ++i) {
                bul.setPos(omanager.options[i].getX(), omanager.options[i].getY() + 10.0);
                bul.setDeg(omanager.options[i].getDeg());
                p_app->pushBulletPlayer(&bul);
            }
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
    if (p_app->getInputInf()->dx == 1)
        p_app->applyImage(IMG_CH_KOSUZU_R0 + ((cnt_all / 4) % 2));
    else if (p_app->getInputInf()->dx == -1)
        p_app->applyImage(IMG_CH_KOSUZU_L0 + ((cnt_all / 4) % 2));
    else
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
