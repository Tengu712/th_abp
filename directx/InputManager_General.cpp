#include "../include/HeaderDX11.hpp"

InputManager::InputManager() : numRegistered(0) {
    for(int i = 0; i < 64; ++i) {
        inf[i] = {0, 0, 0, GAMEPAD_KEYTYPE::Buttons, 0};
        statePrev[i] = 0;
    }
}

bool InputManager::addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad) {
    if (numRegistered >= 64)
        return false;
    inf[numRegistered].codeKey = codeKey;
    inf[numRegistered].codeKeyboard = codeKeyboard;
    inf[numRegistered].typeGamepadKey = typeGamepadKey;
    inf[numRegistered].codeGamepad = codeGamepad;
    numRegistered++;
    return true;
}

void InputManager::inspect() {
    XINPUT_STATE stateXInp;
    DWORD dwRes = XInputGetState(0, &stateXInp);
    for (int i = 0; i < numRegistered; ++i) {
        bool flagKB = GetAsyncKeyState(inf[i].codeKeyboard) & 0x8000;
        bool flagXI = false;
        if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::Buttons)
            flagXI = stateXInp.Gamepad.wButtons & inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::LTrigger)
            flagXI = stateXInp.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::RTrigger)
            flagXI = stateXInp.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLL)
            flagXI = stateXInp.Gamepad.sThumbLX < -inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLR)
            flagXI = stateXInp.Gamepad.sThumbLX > inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLU)
            flagXI = stateXInp.Gamepad.sThumbLY < -inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbLD)
            flagXI = stateXInp.Gamepad.sThumbLY > inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRL)
            flagXI = stateXInp.Gamepad.sThumbRX < -inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRR)
            flagXI = stateXInp.Gamepad.sThumbRX > inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRU)
            flagXI = stateXInp.Gamepad.sThumbRY < -inf[i].codeGamepad;
        else if (inf[i].typeGamepadKey == GAMEPAD_KEYTYPE::ThumbRD)
            flagXI = stateXInp.Gamepad.sThumbRY > inf[i].codeGamepad;
        if (dwRes != ERROR_SUCCESS)
            flagXI = false;
        if (flagKB || flagXI)
            inf[i].state = ((statePrev[i] << 1) ^ 0b011) & 0b011;
        else
            inf[i].state = (statePrev[i] << 2) & 0b100;
        statePrev[i] = inf[i].state;
    }
}

char InputManager::getKey(char codeKey) {
    for (int i = 0; i < numRegistered; ++i) {
        if (inf[i].codeKey == codeKey)
            return inf[i].state;
    }
    return 0;
}
