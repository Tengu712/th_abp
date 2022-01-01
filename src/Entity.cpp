#include "_app.hpp"

void Entity::run() {
    moving = true;
    existing = true;
}

void Entity::move() {
    if (!(moving && existing))
        return;
    const double rad = Deg2Rad(deg);
    x += cos(rad) * spd;
    y += sin(rad) * spd;
}

void Entity::setModelPosDeg(Model* p_model) {
    p_model->pos_x = (float)x;
    p_model->pos_y = (float)y;
    p_model->deg_z = (float)deg - 90.0f;
}

bool Entity::isExisting() {
    return existing;
}

double Entity::getX() {
    return x;
}

double Entity::getY() {
    return y;
}

double Entity::getR() {
    return r;
}

double Entity::getDeg() {
    return deg;
}
double Entity::getSpd() {
    return spd;
}

void Entity::setPos(double x, double y) {
    this->x = x;
    this->y = y;
}

void Entity::setDeg(double deg) {
    this->deg = deg;
}

void Entity::setSpd(double spd) {
    this->spd = spd;
}

void Entity::transPos(double dx, double dy) {
    x += dx;
    y += dy;
}

void Entity::transDeg(double ddeg) {
    deg += ddeg;
}

void Entity::transSpd(double dspd) {
    spd += dspd;
}
