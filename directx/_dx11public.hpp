#pragma once
#ifndef _DX11_PUBLIC_
#define _DX11_PUBLIC_

#include <windows.h>
#pragma comment(lib, "user32.lib")

#define ErrorMessage(m) MessageBoxA(nullptr, m, "Error", MB_OK)

struct Camera {
    float width, height;
    float angle, nearZ, farZ;
    float posX, posY, posZ;
    float dirX, dirY, dirZ;
    float uppX, uppY, uppZ;
};

struct FrameBuffer;

#endif