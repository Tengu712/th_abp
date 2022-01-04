#include "_app.hpp"

void LogueManager::set(unsigned int idx1, unsigned int idx2, bool is_right) {
    this->idx1 = idx1;
    this->idx2 = idx2;
    this->is_right = is_right;
    is_active = true;
}

void LogueManager::draw() {
    if (!is_active || idx1 == -1)
        return;
    is_active = false;
    Model model = Model();
    model.pos_x = 0.0f;
    model.pos_y = -320.0f;
    model.scl_x = 700.0f;
    model.scl_y = 120.0f;
    ModelColorCode2RGBA(&model, 0x88000000);
    p_app->applyModel(&model);
    p_app->applyImage(0);
    p_app->drawIdea();
    model.pos_x = 320.0f;
    model.pos_y = 750.0f;
    model.scl_y = 42.0f;
    ModelColorCode2RGBA(&model, 0xffffffff);
    p_app->drawString(p_app->getStr(kStrLogue, idx1), &model, kIdxNormal);
    if (idx2 == -1)
        return;
    model.pos_y += 42.0f;
    p_app->drawString(p_app->getStr(kStrLogue, idx2), &model, kIdxNormal);
}
