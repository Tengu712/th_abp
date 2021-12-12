#include "_dx11private.hpp"

InputManager::~InputManager() {
    if (infs != nullptr)
        delete[] infs;
}

bool InputManager::init(int max_num_key) {
    this->max_num_key = max_num_key;
    infs = new KeyInf[max_num_key];
    if (infs == nullptr)
        return false;
    for (int i = 0; i < max_num_key; ++i) {
        infs[i] = {0, 0, -1, 0, GAMEPAD_KEYTYPE::Buttons, 0};
    }
    return true;
}

bool InputManager::addKeycode(char code_key, char code_keyboard, GAMEPAD_KEYTYPE type_gamepad, short code_gamepad) {
    if (num_registered >= max_num_key)
        return false;
    infs[num_registered].code_key = code_key;
    infs[num_registered].code_keyboard = code_keyboard;
    infs[num_registered].type_gamepad = type_gamepad;
    infs[num_registered].code_gamepad = code_gamepad;
    num_registered++;
    return true;
}

void InputManager::inspect() {
    XINPUT_STATE stateXInp;
    DWORD dwRes = XInputGetState(0, &stateXInp);
    for (int i = 0; i < num_registered; ++i) {
        bool flagKB = GetAsyncKeyState(infs[i].code_keyboard) & 0x8000;
        bool flagXI = false;
        if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::Buttons)
            flagXI = stateXInp.Gamepad.wButtons & infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::LTrigger)
            flagXI = stateXInp.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::RTrigger)
            flagXI = stateXInp.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbLL)
            flagXI = stateXInp.Gamepad.sThumbLX < infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbLR)
            flagXI = stateXInp.Gamepad.sThumbLX > infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbLU)
            flagXI = stateXInp.Gamepad.sThumbLY > infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbLD)
            flagXI = stateXInp.Gamepad.sThumbLY < infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbRL)
            flagXI = stateXInp.Gamepad.sThumbRX < infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbRR)
            flagXI = stateXInp.Gamepad.sThumbRX > infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbRU)
            flagXI = stateXInp.Gamepad.sThumbRY > infs[i].code_gamepad;
        else if (infs[i].type_gamepad == GAMEPAD_KEYTYPE::ThumbRD)
            flagXI = stateXInp.Gamepad.sThumbRY < infs[i].code_gamepad;
        if (dwRes != ERROR_SUCCESS)
            flagXI = false;
        if (flagKB || flagXI)
            infs[i].state = ((infs[i].state_prev << 1) ^ 0b011) & 0b011;
        else
            infs[i].state = (infs[i].state_prev << 2) & 0b100;
        infs[i].state_prev = infs[i].state;
    }
}

char InputManager::getKey(char code_key) {
    for (int i = 0; i < num_registered; ++i) {
        if (infs[i].code_key == code_key)
            return infs[i].state;
    }
    return 0;
}
