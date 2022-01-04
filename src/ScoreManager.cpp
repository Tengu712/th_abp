#include "_app.hpp"

void ScoreManager::init() {
    score = 0LL;
    graze = 0U;
}

void ScoreManager::draw() {
    // Score
    Model model = Model();
    char buf[64];
    model.pos_x = 250.0f;
    model.pos_y = 30.0f;
    model.scl_y = 45.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%011lld", score);
    p_app->drawString(buf, &model, kIdxNormal);
    // Hiscore
    model.pos_x = 1030.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%011lld", hiscore);
    p_app->drawString(buf, &model, kIdxNormal, 1);
    // Rank
    model.pos_x = 250.0f;
    model.pos_y += 40.0f;
    model.scl_y = 40.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%04.1lf", rank);
    p_app->drawString(buf, &model, kIdxNormal);
    // Graze
    model.pos_x = 1030.0f;
    memset(buf, 0, sizeof(char) * 64);
    snprintf(buf, 64, "%d", graze);
    p_app->drawString(buf, &model, kIdxNormal, 1);
}

unsigned long long ScoreManager::getHiScore() {
    return hiscore;
}

unsigned long long ScoreManager::getScore() {
    return score;
}

unsigned int ScoreManager::getGraze() {
    return graze;
}

double ScoreManager::getRank() {
    return rank;
}

void ScoreManager::setHiScore(unsigned long long hiscore) {
    this->hiscore = hiscore;
}

void ScoreManager::setRank(double rank) {
    this->rank = rank;
}

void ScoreManager::transScore(unsigned long long d_score) {
    score += d_score;
}

void ScoreManager::transGraze(unsigned int d_graze) {
    graze = min(graze + d_graze, 9999);
}

void ScoreManager::transRank(double d_rank) {
    rank = max(min(rank + d_rank, 50.0), 0.0);
}
