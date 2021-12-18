#include "_app.hpp"

bool SceneCharacterSelect::init() {
    return true;
}

void SceneCharacterSelect::update() {
    p_app->initGameInf();
    p_app->changeScene(kSceneGame);
}
