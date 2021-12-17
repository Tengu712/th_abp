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
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Windowscodecs.lib")

using Microsoft::WRL::ComPtr;

#define ErrorMessage(m) MessageBoxA(nullptr, m, "Error", MB_OK)

struct Vertex {
    float pcnu[12];
};

struct ConstantBuffer {
    DirectX::XMFLOAT4X4 mat_scl;
    DirectX::XMFLOAT4X4 mat_rot;
    DirectX::XMFLOAT4X4 mat_trs;
    DirectX::XMFLOAT4X4 mat_view;
    DirectX::XMFLOAT4X4 mat_proj;
    DirectX::XMFLOAT4 vec_color;
    DirectX::XMFLOAT4 vec_light;
    DirectX::XMFLOAT4 params;
};

struct Image {
    unsigned int id;
    ComPtr<ID3D11ShaderResourceView> p_srview;
};

struct FrameBuffer {
    ComPtr<ID3D11RenderTargetView> p_rtview;
    Image image;
};

struct ModelInf {
    unsigned int num_idx;
    ComPtr<ID3D11Buffer> p_vbuf;
    ComPtr<ID3D11Buffer> p_ibuf;
};

class D3DManager {
private:
    HWND _h_wnd;
    bool _is_depth;
    ComPtr<ID3D11Device> _p_device;
    ComPtr<ID3D11DeviceContext> _p_context;
    ComPtr<IDXGISwapChain> _p_swapchain;
    ComPtr<ID3D11RenderTargetView> _p_rtview;
    ComPtr<ID3D11DepthStencilView> _p_dsview;
    ComPtr<ID3D11VertexShader> _p_vshader;
    ComPtr<ID3D11PixelShader> _p_pshader;
    ComPtr<ID3D11InputLayout> _p_ilayout;
    ComPtr<ID3D11Buffer> _p_cbuf;
    ConstantBuffer _cbuf;

public:
    D3DManager()
        : _h_wnd(nullptr),
          _p_device(nullptr),
          _p_context(nullptr),
          _p_swapchain(nullptr),
          _is_depth(true),
          _p_rtview(nullptr),
          _p_dsview(nullptr),
          _p_vshader(nullptr),
          _p_pshader(nullptr),
          _p_ilayout(nullptr),
          _p_cbuf(nullptr),
          _cbuf(ConstantBuffer()) {
    }
    bool init(HINSTANCE h_inst, int cmd_show, LPCWSTR name_wnd, LPCWSTR name_wndclass, unsigned int width,
        unsigned int height, bool windowed);
    bool isIconic();
    bool createModelBuffers(unsigned int num_vtx, Vertex* data, unsigned int* data_indx, ModelInf* p_minf);
    bool createFrameBuffer(unsigned int width, unsigned int height, FrameBuffer* p_fbuf);
    bool createImage(HMODULE h_module, unsigned int id, Image* p_image);
    bool createFontImage(LOGFONTA* p_logfont, unsigned int code, Image* p_image);
    void applyImage(Image* p_image);
    void enableDepthStencil(bool enable);
    void setViewport(D3D11_VIEWPORT* p_viewport);
    void setMatrixScale(float scl_x, float scl_y, float scl_z);
    void setMatrixRotate(float deg_x, float deg_y, float deg_z);
    void setMatrixTranslate(float pos_x, float pos_y, float pos_z);
    void setMatrixView(float pos_x, float pos_y, float pos_z, float dir_x, float dir_y, float dir_z, float upp_x,
        float upp_y, float upp_z);
    void setMatrixProject(float width, float height, float angle, float near_z, float far_z, bool parse);
    void setVectorColor(float col_r, float col_g, float col_b, float col_a);
    void setVectorLight(float x, float y, float z, float w);
    void setVectorParams(float x, float y, float z, float w);
    void setVectorParamsX(float x);
    void setVectorParamsY(float y);
    void setVectorParamsZ(float z);
    void setVectorParamsW(float w);
    void drawBegin(FrameBuffer* p_fbuf);
    void drawModel(ModelInf* p_minf);
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
    char state_prev;
    char code_key;
    char code_keyboard;
    GAMEPAD_KEYTYPE type_gamepad;
    short code_gamepad;
};

class InputManager {
private:
    int max_num_key;
    int num_registered;
    KeyInf* infs;

public:
    InputManager() : max_num_key(0), num_registered(0), infs(nullptr) {
    }
    ~InputManager();
    bool init(int max_num_key);
    bool addKeycode(char code_key, char code_keyboard, GAMEPAD_KEYTYPE type_gamepad, short code_gamepad);
    void inspect();
    char getKey(char code_key);
};

#endif
