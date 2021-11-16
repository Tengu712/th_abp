#include "_app.hpp"

bool SceneTitle::init() {
    pApp->createCamera((float)kSceWidth, (float)kSceHeight, &camera);
    camera.posZ = -10.0f;
    return true;
}

void SceneTitle::update() {
    pApp->applyCamera(&camera, false);
    Fact f = Fact();
    f.sclX = 10.0f;
    f.sclY = 10.0f;
    f.sclZ = 10.0f;
    pApp->applyFact(&f);
    pApp->drawIdea();
}
