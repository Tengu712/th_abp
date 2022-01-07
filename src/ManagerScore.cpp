#include "_app.hpp"

void ManagerScore::init() {
    score = 0LL;
    graze = 0U;
}

void ManagerScore::draw() {
    // Score
    Model model = Model();
    char buf[64];
    model.pos_x = 250.0f;
    model.pos_y = 30.0f;
    model.scl_y = 45.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%011lld", score);
    p_app->drawString(buf, &model, kFontDigit);
    // Hiscore
    model.pos_x = 1030.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%011lld", hiscore);
    p_app->drawString(buf, &model, kFontDigit, 1);
    // Rank
    model.pos_x = 250.0f;
    model.pos_y += 40.0f;
    model.scl_y = 40.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%04.1lf", rank);
    p_app->drawString(buf, &model, kFontDigit);
    // Graze
    model.pos_x = 1030.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%d", graze);
    p_app->drawString(buf, &model, kFontDigit, 1);
}

unsigned long long ManagerScore::getHiScore() {
    return hiscore;
}

unsigned long long ManagerScore::getScore() {
    return score;
}

unsigned int ManagerScore::getGraze() {
    return graze;
}

double ManagerScore::getRank() {
    return rank;
}

void ManagerScore::setHiScore(unsigned long long hiscore) {
    this->hiscore = hiscore;
}

void ManagerScore::setRank(double rank) {
    this->rank = rank;
}

void ManagerScore::transScore(unsigned long long d_score) {
    score += d_score;
}

void ManagerScore::transGraze(unsigned int d_graze) {
    graze = min(graze + d_graze, 9999);
}

void ManagerScore::transRank(double d_rank) {
    rank = max(min(rank + d_rank, 50.0), 0.0);
}
