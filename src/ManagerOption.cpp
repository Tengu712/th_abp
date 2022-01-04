#include "_app.hpp"

void ManagerOption::update() {
    const int kSlow = p_app->getInputInf()->s;
    const double kPX = p_app->getPlayer()->getX();
    const double kPY = p_app->getPlayer()->getY();
    if (id_weapon == 0) {
        prevs[0] = max(min(kSlow * 20.0 + 60.0, 100.0), max(prevs[0] - 20.0, 60.0));
        prevs[1] = max(min(kSlow * 10.0 + 30.0, 50.0), max(prevs[1] - 10.0, 30.0));
        prevs[2] = max(min(kSlow * 30.0, 60.0), max(prevs[2] - 30.0, 0.0));
        options[0].setPos(kPX - prevs[0], kPY);
        options[1].setPos(kPX - prevs[1], kPY + prevs[2] - 50.0);
        options[2].setPos(kPX + prevs[1], kPY + prevs[2] - 50.0);
        options[3].setPos(kPX + prevs[0], kPY);
        if (kSlow > 0) {
            for (int i = 0; i < 4; ++i) {
                options[i].setDeg(
                    Homing(options[i].getX(), options[i].getY(), p_app->getEnemy()->getX(), p_app->getEnemy()->getY()));
            }
        } else {
            const double kDX = -p_app->getInputInf()->dx * 8.0;
            options[0].setDeg(95.0 + kDX);
            options[1].setDeg(90.0 + kDX);
            options[2].setDeg(90.0 + kDX);
            options[3].setDeg(85.0 + kDX);
        }
    } else if (id_weapon == 1) {
        prevs[1] = max(min(kSlow / 4.0, 1.0), max(prevs[1] - 0.25, 0.0));
        prevs[2] = max(min(kSlow / 4.0, 1.0), max(prevs[2] - 0.25, 0.0));
        const double kY123 = kPY + 60.0 + 10.0 * prevs[2];
        options[0].setPos(kPX, kPY + 70.0);
        options[0].setDeg(90.0);
        for (int i = 1; i < 4; ++i) {
            options[i].setPos(
                kPX + 70.0 * sin(Deg2Rad((double)cnt * 6.0 + (double)i * 120.0)) * (1.0 - prevs[1]), kY123);
            options[i].setDeg(90.0);
        }
    } else {
        prevs[1] = max(min(kSlow * 12.0, 24.0), max(prevs[1] - 12.0, 0.0));
        prevs[2] = max(min(kSlow * 10.0, 20.0), max(prevs[2] - 10.0, 0.0));
        options[0].setPos(kPX + prevs[1] - 60.0, kPY + prevs[2]);
        options[1].setPos(kPX + prevs[1] - 40.0, kPY + 50.0);
        options[2].setPos(kPX - prevs[1] + 40.0, kPY + 50.0);
        options[3].setPos(kPX - prevs[1] + 60.0, kPY + prevs[2]);
        if (kSlow > 0) {
            for (int i = 0; i < 4; ++i) {
                options[i].setDeg(90.0);
            }
        } else {
            options[0].setDeg(92.5);
            options[1].setDeg(87.5);
            options[2].setDeg(92.5);
            options[3].setDeg(87.5);
        }
    }
    ++cnt;
}

void ManagerOption::draw() {
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
