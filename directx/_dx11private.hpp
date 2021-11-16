#pragma once
#ifndef _DX11_PRIVATE_
#define _DX11_PRIVATE_

#include <DirectXMath.h>
#include <d3d11.h>
#include <wrl/client.h>
#include <xinput.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment (lib, "xinput.lib")

using Microsoft::WRL::ComPtr;

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
    float posX, posY, posZ;
    float degX, degY, degZ;
    float sclX, sclY, sclZ;
    float colR, colG, colB, colA;
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
    D3DManager();
    bool init(HINSTANCE hInst, int cmdShow, LPCWSTR wndName, LPCWSTR wndClassName, unsigned int width,
        unsigned int height, bool windowed);
    bool createModelBuffers(unsigned int numVtx, Vertex* data, unsigned int* dataIdx, ModelInf* minf);
    bool createImage(HMODULE hModule, unsigned int id, Image* pImage);
    bool createFrameBuffer(unsigned int width, unsigned int height, FrameBuffer* pFBuffer);
    void createCamera(float width, float height, Camera* camera);
    void enableDepthStencil(bool enable);
    void setViewport(D3D11_VIEWPORT* pViewport);
    void applyCamera(Camera* camera, bool parse);
    void applyImage(Image* pImage);
    void drawModel(ModelInf* minf);
    void drawBegin(FrameBuffer* pFBuffer);
    void drawEnd();
};


enum struct GAMEPAD_KEYTYPE : char {
    Buttons, LTrigger, RTrigger,
    ThumbLL, ThumbLR, ThumbLU, ThumbLD,
    ThumbRL, ThumbRR, ThumbRU, ThumbRD,
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
    InputManager();
    ~InputManager();
    bool init(int maxNumKey);
    bool addKeycode(char codeKey, char codeKeyboard, GAMEPAD_KEYTYPE typeGamepadKey, short codeGamepad);
    void inspect();
    char getKey(char codeKey);
};

#endif
