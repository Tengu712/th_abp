#include "_app.hpp"

void SceneGame::update() {
    InputInf iinf = InputInf();
    iinf.dx = p_app->getKey(KEY_CODE::Right, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Left, KEY_STATE::Pressed);
    iinf.dy = p_app->getKey(KEY_CODE::Up, KEY_STATE::Pressed) - p_app->getKey(KEY_CODE::Down, KEY_STATE::Pressed);
    iinf.z = p_app->getKey(KEY_CODE::Z, KEY_STATE::Pressed);
    iinf.x = p_app->getKey(KEY_CODE::X, KEY_STATE::Down);
    iinf.s = p_app->getKey(KEY_CODE::Shift, KEY_STATE::Pressed);
    p_app->setInputInf(&iinf);
    updateGame();
    drawGame();
    drawUI();
}