#include "_app.hpp"

void Bullet::init(App* p_app, unsigned int knd, unsigned int mov, unsigned int dmg, unsigned int col, int cnt) {
    this->p_app = p_app;
    this->knd = knd;
    this->mov = mov;
    this->dmg = dmg;
    this->col = col;
    this->cnt = cnt;
    existing = true;
    if (knd == IMG_BU_JIKI_HARI) {
        r = 45.0;
        scl_x = 45.0f;
        scl_y = 45.0f;
    } else if (knd == IMG_BU_JIKI_BIGHARI) {
        r = 50.0;
        scl_x = 64.0f;
        scl_y = 64.0f;
    } else if (knd == IMG_BU_LAZER) {
        r = 50.0;
        scl_x = 64.0f;
        scl_y = 1000.0f;
    }
}

void Bullet::update() {
    if (del) {
        existing = false;
        return;
    }
    if (cnt == 0)
        moving = true;
    if (!moving)
        return;
    move();
    if (x < -520.0 || x > 520.0 || y < -520.0 || y > 520.0)
        existing = false;
    if (knd == IMG_BU_LAZER && cnt > 1)
        existing = false;
    ++cnt;
}

void Bullet::draw() {
    Model model = Model();
    setModelPosDeg(&model);
    if (knd == IMG_BU_LAZER) {
        model.pos_x = (float)(x + scl_y / 2.0 * cos(Deg2Rad(deg)));
        model.pos_y = (float)(y + scl_y / 2.0 * sin(Deg2Rad(deg)));
    }
    model.scl_x = scl_x;
    model.scl_y = scl_y;
    SetModelColor(&model, col);
    p_app->applyModel(&model);
    p_app->applyImage(knd);
    p_app->drawIdea();
}

void RotVec(double* p_x, double* p_y, double deg) {
    const double kRad = Deg2Rad(deg);
    const double kX = cos(kRad) * *p_x - sin(kRad) * *p_y;
    const double kY = sin(kRad) * *p_x + cos(kRad) * *p_y;
    *p_x = kX;
    *p_y = kY;
}

unsigned int Bullet::getDamage() {
    return dmg;
}

int Bullet::isHit(Entity* p_trg) {
    if (p_trg == nullptr)
        return 0;
    if (knd == IMG_BU_LAZER) {
        double nx = p_trg->getX() - x;
        double ny = p_trg->getY() - y;
        RotVec(&nx, &ny, deg - 90.0);
        if (ny < 0)
            return 0;
        if (fabs(nx) < r) {
            scl_y = ny;
            return 1;
        } else if (fabs(nx) < 2 * r)
            return 2;
        return 0;
    }
    const double kDis = (x - p_trg->getX()) * (x - p_trg->getX()) + (y - p_trg->getY()) * (y - p_trg->getY());
    if (kDis < (r + p_trg->getR()) * (r + p_trg->getR()))
        return 1;
    else if (kDis < (2 * r + p_trg->getR()) * (2 * r + p_trg->getR()))
        return 2;
    return 0;
}
