#pragma once
#ifndef _DX11_PRIVATE_
#define _DX11_PRIVATE_

#include <DirectXMath.h>
#include <d3d11.h>
#include <windows.h>
#include <wrl/client.h>
#include <xinput.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "xinput.lib")

using Microsoft::WRL::ComPtr;

#define ErrorMessage(m) MessageBoxA(nullptr, m, "Error", MB_OK)

struct Vertex {
    float pcnu[12];
};

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 matScl;
    DirectX::XMFLOAT4X4 matRot;
    DirectX::XMFLOAT4X4 matTrs;
    DirectX::XMFLOAT4X4 matView;
    DirectX::XMFLOAT4X4 matProj;
    DirectX::XMFLOAT4 vecColor;
    DirectX::XMFLOAT4 vecLight;
    DirectX::XMFLOAT4 params;
};

struct Image {
    unsigned int id;
    ComPtr<ID3D11ShaderResourceView> pSRView;
};

struct FrameBuffer {
    ComPtr<ID3D11RenderTargetView> pRTView;
    Image image;
};

struct ModelInf {
    unsigned int numIdx;
    ComPtr<ID3D11Buffer> pVBuffer;
    ComPtr<ID3D11Buffer> pIBuffer;
};

class D3DManager {
private:
    HWND _hWnd;
    bool _isDepth;
    ComPtr<ID3D11Device> _pDevice;
    ComPtr<ID3D11DeviceContext> _pImContext;
    ComPtr<IDXGISwapChain> _pSwapChain;
    ComPtr<ID3D11RenderTargetView> _pRTView;
    ComPtr<ID3D11DepthStencilView> _pDSView;
    ComPtr<ID3D11VertexShader> _pVShader;
    ComPtr<ID3D11PixelShader> _pPShader;
    ComPtr<ID3D11InputLayout> _pILayout;
    ComPtr<ID3D11Buffer> _pCBuffer;
    ConstantBuffer _cbuffer;

public:
    D3DManager()
        : _hWnd(nullptr),
          _pDevice(nullptr),
          _pImContext(nullptr),
          _pSwapChain(nullptr),
          _isDepth(true),
          _pRTView(nullptr),
          _pDSView(nullptr),
          _pVShader(nullptr),
          _pPShader(nullptr),
          _pILayout(nullptr),
          _pCBuffer(nullptr),
          _cbuffer(ConstantBuffer()) {
    }
    bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, unsigned int width,
        unsigned int height, bool windowed);
    bool createModelBuffers(unsigned int numVtx, Vertex* data, unsigned int* dataIdx, ModelInf* minf);
    bool createFrameBuffer(unsigned int width, unsigned int height, FrameBuffer* pFBuffer);
    bool createImage(HMODULE hModule, unsigned int id, Image* pImage);
    void applyImage(Image* pImage);
    void enableDepthStencil(bool enable);
    void setViewport(D3D11_VIEWPORT* pViewport);
    void setMatrixScale(float sclX, float sclY, float sclZ);
    void setMatrixRotate(float degX, float degY, float degZ);
    void setMatrixTranslate(float posX, float posY, float posZ);
    void setMatrixView(
        float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float uppX, float uppY, float uppZ);
    void setMatrixProject(float width, float height, float angle, float nearZ, float farZ, bool parse);
    void setVectorColor(float colR, float colG, float colB, float colA);
    void setVectorLight(float x, float y, float z, float w);
    void setVectorParams(float x, float y, float z, float w);
    void drawBegin(FrameBuffer* pFBuffer);
    void drawModel(ModelInf* minf);
    void drawEnd();
};

enum struct GAMEPAD_KEYTYPE : char {
    Buttons,
    LTrigger,
    RTrigger,
    ThumbLL,
    ThumbLR,
    ThumbLU,
    ThumbLD,
    ThumbRL,
    ThumbRR,
    ThumbRU,
    ThumbRD,
};

struct KeyInf {
    char state;
    char statePrev;
    char codeKey;
    char codeKeyboard;
    GAMEPAD_KEYTYPE typeGamepadKey;
    short codeGamepad;
};

class InputManager {
private:
    int maxNumKey;
    int numRegistered;
    KeyInf* infs;

public:
    InputManager() : maxNumKey(0), numRegistered(0), infs(nullptr) {
    }
    ~InputManager();
    bool init(int maxNumKey);
    bool addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad);
    void inspect();
    char getKey(char codeKey);
};

#endif
