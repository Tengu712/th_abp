#include "_dx11public.hpp"
#include "_dx11private.hpp"

InputManager::InputManager() : maxNumKey(0), numRegistered(0), infs(nullptr) {}

InputManager::~InputManager() {
    if (infs != nullptr)
        delete[] infs;
}

bool InputManager::init(int maxNumKey) {
    this->maxNumKey = maxNumKey;
    infs = new KeyInf[maxNumKey];
    if (infs == nullptr)
        return false;
    for (int i = 0; i < maxNumKey; ++i) {
        infs[i] = {0, 0, -1, 0, GAMEPAD_KEYTYPE::Buttons, 0};
    }
    return true;
}

bool InputManager::addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad) {
    if (numRegistered >= maxNumKey)
        return false;
    infs[numRegistered].codeKey = codeKey;
    infs[numRegistered].codeKeyboard = codeKeyboard;
    infs[numRegistered].typeGamepadKey = typeGamepadKey;
    infs[numRegistered].codeGamepad = codeGamepad;
    numRegistered++;
    return true;
}

void InputManager::inspect() {
    XINPUT_STATE stateXInp;
    DWORD dwRes = XInputGetState(0, &stateXInp);
    for (int i = 0; i < numRegistered; ++i) {
        bool flagKB = GetAsyncKeyState(infs[i].codeKeyboard) & 0x8000;
        bool flagXI = false;
        if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::Buttons)
            flagXI = stateXInp.Gamepad.wButtons & infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::LTrigger)
            flagXI = stateXInp.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::RTrigger)
            flagXI = stateXInp.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLL)
            flagXI = stateXInp.Gamepad.sThumbLX < infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLR)
            flagXI = stateXInp.Gamepad.sThumbLX > infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLU)
            flagXI = stateXInp.Gamepad.sThumbLY > infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLD)
            flagXI = stateXInp.Gamepad.sThumbLY < infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRL)
            flagXI = stateXInp.Gamepad.sThumbRX < infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRR)
            flagXI = stateXInp.Gamepad.sThumbRX > infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRU)
            flagXI = stateXInp.Gamepad.sThumbRY > infs[i].codeGamepad;
        else if (infs[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRD)
            flagXI = stateXInp.Gamepad.sThumbRY < infs[i].codeGamepad;
        if (dwRes != ERROR_SUCCESS)
            flagXI = false;
        if (flagKB || flagXI)
            infs[i].state = ((infs[i].statePrev << 1) ^ 0b011) & 0b011;
        else
            infs[i].state = (infs[i].statePrev << 2) & 0b100;
        infs[i].statePrev = infs[i].state;
    }
}

char InputManager::getKey(char codeKey) {
    for (int i = 0; i < numRegistered; ++i) {
        if (infs[i].codeKey == codeKey)
            return infs[i].state;
    }
    return 0;
}
