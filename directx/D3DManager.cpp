#include "_dx11private.hpp"

// ================================================================================================================= //
//                                          General Funcs                                                            //
// ================================================================================================================= //

bool D3DManager::isIconic() {
    return IsIconic(_h_wnd);
}

// ================================================================================================================= //
//                                          Setting Funcs                                                            //
// ================================================================================================================= //

void D3DManager::applyImage(Image* p_image) {
    if (p_image == nullptr || p_image->p_srview == nullptr) {
        _cbuf.params.x = 0.0f;
    } else {
        _p_context->PSSetShaderResources(0U, 1U, p_image->p_srview.GetAddressOf());
        _cbuf.params.x = 1.0f;
    }
}

void D3DManager::enableDepthStencil(bool enable) {
    _is_depth = enable;
}

void D3DManager::setViewport(D3D11_VIEWPORT* p_viewport) {
    if (p_viewport == nullptr)
        return;
    _p_context->RSSetViewports(1U, p_viewport);
}

void D3DManager::setMatrixScale(float scl_x, float scl_y, float scl_z) {
    DirectX::XMStoreFloat4x4(&_cbuf.mat_scl, DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(scl_x, scl_y, scl_z)));
}

void D3DManager::setMatrixRotate(float deg_x, float deg_y, float deg_z) {
    DirectX::XMStoreFloat4x4(&_cbuf.mat_rot,
        DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(DirectX::XMConvertToRadians(deg_x),
            DirectX::XMConvertToRadians(deg_y), DirectX::XMConvertToRadians(deg_z))));
}

void D3DManager::setMatrixTranslate(float pos_x, float pos_y, float pos_z) {
    DirectX::XMStoreFloat4x4(
        &_cbuf.mat_trs, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(pos_x, pos_y, pos_z)));
}

void D3DManager::setMatrixView(float pos_x, float pos_y, float pos_z, float dir_x, float dir_y, float dir_z,
    float upp_x, float upp_y, float upp_z) {
    DirectX::XMStoreFloat4x4(&_cbuf.mat_view,
        DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(pos_x, pos_y, pos_z, 0.0f),
            DirectX::XMVectorSet(dir_x, dir_y, dir_z, 0.0f), DirectX::XMVectorSet(upp_x, upp_y, upp_z, 0.0f))));
}

void D3DManager::setMatrixProject(float width, float height, float angle, float near_z, float far_z, bool parse) {
    DirectX::XMStoreFloat4x4(&_cbuf.mat_proj,
        DirectX::XMMatrixTranspose(parse ? DirectX::XMMatrixPerspectiveFovLH(angle, width / height, near_z, far_z)
                                         : DirectX::XMMatrixOrthographicLH(width, height, near_z, far_z)));
}

void D3DManager::setVectorColor(float col_r, float col_g, float col_b, float col_a) {
    DirectX::XMStoreFloat4(&_cbuf.vec_color, DirectX::XMVectorSet(col_r, col_g, col_b, col_a));
}

void D3DManager::setVectorLight(float x, float y, float z, float w) {
    DirectX::XMStoreFloat4(&_cbuf.vec_light, DirectX::XMVectorSet(x, y, z, w));
}

void D3DManager::setVectorParams(float x, float y, float z, float w) {
    DirectX::XMStoreFloat4(&_cbuf.params, DirectX::XMVectorSet(x, y, z, w));
}

// ================================================================================================================= //
//                                          Drawing Funcs                                                            //
// ================================================================================================================= //

void D3DManager::drawBegin(FrameBuffer* p_fbuf) {
    _p_context->OMSetRenderTargets(1U, p_fbuf == nullptr ? _p_rtview.GetAddressOf() : p_fbuf->p_rtview.GetAddressOf(),
        _is_depth ? _p_dsview.Get() : nullptr);
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    _p_context->ClearRenderTargetView(p_fbuf == nullptr ? _p_rtview.Get() : p_fbuf->p_rtview.Get(), clearColor);
    if (_is_depth)
        _p_context->ClearDepthStencilView(_p_dsview.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);
}

void D3DManager::drawModel(ModelInf* p_minf) {
    unsigned int strides = sizeof(Vertex);
    unsigned int offsets = 0U;
    _p_context->IASetVertexBuffers(0U, 1U, p_minf->p_vbuf.GetAddressOf(), &strides, &offsets);
    _p_context->IASetIndexBuffer(p_minf->p_ibuf.Get(), DXGI_FORMAT_R32_UINT, 0U);
    _p_context->UpdateSubresource(_p_cbuf.Get(), 0U, nullptr, &_cbuf, 0U, 0U);
    _p_context->VSSetConstantBuffers(0U, 1U, _p_cbuf.GetAddressOf());
    _p_context->DrawIndexed(p_minf->num_idx, 0U, 0U);
}

void D3DManager::drawEnd() {
    _p_swapchain->Present(1U, 0U);
}

// ================================================================================================================= //
//                                          Creating Funcs                                                           //
// ================================================================================================================= //

bool D3DManager::createModelBuffers(
    unsigned int num_vtx, Vertex* data_pcnu, unsigned int* data_indx, ModelInf* p_minf) {
    D3D11_BUFFER_DESC desc_vbuf = {sizeof(Vertex) * num_vtx, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0U, 0U, 0U};
    D3D11_SUBRESOURCE_DATA dataVBuffer = {data_pcnu, 0U, 0U};
    if (FAILED(_p_device->CreateBuffer(&desc_vbuf, &dataVBuffer, p_minf->p_vbuf.GetAddressOf())))
        return false;

    D3D11_BUFFER_DESC desc_index = {
        (unsigned int)sizeof(unsigned int) * p_minf->num_idx, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0U, 0U, 0U};
    D3D11_SUBRESOURCE_DATA dataIndex = {data_indx, 0U, 0U};
    if (FAILED(_p_device->CreateBuffer(&desc_index, &dataIndex, p_minf->p_ibuf.GetAddressOf())))
        return false;

    return true;
}

bool D3DManager::createFrameBuffer(unsigned int width, unsigned int height, FrameBuffer* p_fbuf) {
    try {
        if (p_fbuf == nullptr)
            throw "Nullptr gained when create a frame buf.";

        D3D11_TEXTURE2D_DESC desc_tex = {
            width,
            height,
            1U,
            1U,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            {1U, 0U},
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,
            0U,
            0U,
        };
        ComPtr<ID3D11Texture2D> p_tex = nullptr;
        if (FAILED(_p_device->CreateTexture2D(&desc_tex, nullptr, p_tex.GetAddressOf())))
            throw "Failed to create texture2D for frame buf.";

        D3D11_RENDER_TARGET_VIEW_DESC desc_rtview;
        ZeroMemory(&desc_rtview, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        desc_rtview.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc_rtview.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        if (FAILED(_p_device->CreateRenderTargetView(p_tex.Get(), &desc_rtview, p_fbuf->p_rtview.GetAddressOf())))
            throw "Failed to create render target view for frame buf.";

        D3D11_SHADER_RESOURCE_VIEW_DESC desc_srview;
        ZeroMemory(&desc_srview, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        desc_srview.Format = desc_rtview.Format;
        desc_srview.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc_srview.Texture2D.MipLevels = 1U;
        if (FAILED(
                _p_device->CreateShaderResourceView(p_tex.Get(), &desc_srview, p_fbuf->image.p_srview.GetAddressOf())))
            throw "Failed to create shader resource view for frame buf.";

    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }

    return true;
}

#include <wincodec.h>

bool D3DManager::createImage(HMODULE h_module, unsigned int id, Image* p_image) {
    try {
        if (p_image == nullptr)
            throw "Nullptr gained when creating an image.";
        p_image->id = id;

        HRSRC h_res_image = FindResourceA(h_module, MAKEINTRESOURCEA(id), "IMAGE");
        if (!h_res_image)
            throw "Failed to find resource.";

        HGLOBAL h_data_image = LoadResource(h_module, h_res_image);
        if (!h_data_image)
            throw "Failed to load resource.";

        void* pLock = LockResource(h_data_image);
        if (!pLock)
            throw "Failed to lock resource.";

        DWORD size_res = SizeofResource(h_module, h_res_image);
        if (size_res == 0)
            throw "Failed to get size of resource.";

        ComPtr<IWICImagingFactory> p_factory = nullptr;
        if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,
                (LPVOID*)p_factory.GetAddressOf())))
            throw "Failed to create wic factory.";

        ComPtr<IWICStream> p_stream = nullptr;
        if (FAILED(p_factory->CreateStream(p_stream.GetAddressOf())))
            throw "Failed to create stream.";

        if (FAILED(p_stream->InitializeFromMemory((byte*)pLock, size_res)))
            throw "Failed to initialize stream.";

        ComPtr<IWICBitmapDecoder> p_decoder = nullptr;
        if (FAILED(p_factory->CreateDecoderFromStream(
                p_stream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, p_decoder.GetAddressOf())))
            throw "Failed to load file.";

        ComPtr<IWICBitmapFrameDecode> p_frame = nullptr;
        if (FAILED(p_decoder->GetFrame(0U, p_frame.GetAddressOf())))
            throw "Failed to get frame.";

        ComPtr<IWICFormatConverter> p_format_converter = nullptr;
        if (FAILED(p_factory->CreateFormatConverter(p_format_converter.GetAddressOf())))
            throw "Failed to create format converter.";

        if (FAILED(p_format_converter->Initialize(p_frame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone,
                nullptr, 1.0f, WICBitmapPaletteTypeMedianCut)))
            throw "Failed to initialize format.";

        unsigned int width, height;
        if (FAILED(p_format_converter->GetSize(&width, &height)))
            throw "Failed to get texture size.";

        D3D11_TEXTURE2D_DESC desc_tex = {width, height, 1U, 1U, DXGI_FORMAT_R8G8B8A8_UNORM, {1U, 0U},
            D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0U};
        ComPtr<ID3D11Texture2D> p_tex = nullptr;
        if (FAILED(_p_device->CreateTexture2D(&desc_tex, nullptr, p_tex.GetAddressOf())))
            throw "Failed to create texture resource.";

        D3D11_MAPPED_SUBRESOURCE res_mapped;
        ZeroMemory(&res_mapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
        if (FAILED(_p_context->Map(p_tex.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &res_mapped)))
            throw "Failed to map.";
        if (FAILED(p_format_converter->CopyPixels(nullptr, width * 4U, width * height * 4U, (BYTE*)res_mapped.pData)))
            throw "Failed to copy pixels.";
        _p_context->Unmap(p_tex.Get(), 0U);

        D3D11_SHADER_RESOURCE_VIEW_DESC desc_shader;
        ZeroMemory(&desc_shader, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        desc_shader.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        desc_shader.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc_shader.Texture2D.MipLevels = 1U;
        if (FAILED(_p_device->CreateShaderResourceView(p_tex.Get(), &desc_shader, p_image->p_srview.GetAddressOf())))
            throw "Failed to create shader resource view.";
    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }

    return true;
}

bool D3DManager::createFontImage(LOGFONTA* p_logfont, unsigned int code, Image* p_image) {
    try {
        if (p_image == nullptr)
            throw "Nullptr gained when creating a font image.";
        p_image->id = code;

        HFONT h_font = CreateFontIndirectA(p_logfont);
        HDC hdc = GetDC(nullptr);
        HFONT h_font_old = (HFONT)SelectObject(hdc, h_font);

        TEXTMETRICA met_text;
        GetTextMetricsA(hdc, &met_text);
        GLYPHMETRICS met_glyph;
        const MAT2 mat = {{0, 1}, {0, 0}, {0, 0}, {0, 1}};
        DWORD dw_res = GetGlyphOutlineA(hdc, code, GGO_GRAY4_BITMAP, &met_glyph, 0, nullptr, &mat);

        unsigned char* p_mono = new unsigned char[dw_res];
        GetGlyphOutlineA(hdc, code, GGO_GRAY4_BITMAP, &met_glyph, dw_res, p_mono, &mat);

        SelectObject(hdc, h_font_old);
        DeleteObject(h_font);
        ReleaseDC(nullptr, hdc);

        const unsigned int kWidth = (unsigned int)met_glyph.gmCellIncX;
        const unsigned int kHeight = (unsigned int)met_text.tmHeight;
        const int kOfsX = met_glyph.gmptGlyphOrigin.x;
        const int kOfsY = met_text.tmAscent - met_glyph.gmptGlyphOrigin.y;
        const int kWidthBmp = met_glyph.gmBlackBoxX + (4 - (met_glyph.gmBlackBoxX % 4)) % 4;
        const int kHeightBmp = met_glyph.gmBlackBoxY;

        D3D11_TEXTURE2D_DESC desc_tex = {
            kWidth,
            kHeight,
            1,
            1,
            DXGI_FORMAT_R8G8B8A8_UNORM,
            {1, 0},
            D3D11_USAGE_DYNAMIC,
            D3D11_BIND_SHADER_RESOURCE,
            D3D11_CPU_ACCESS_WRITE,
            0,
        };
        ComPtr<ID3D11Texture2D> p_layer = nullptr;
        if (FAILED(_p_device->CreateTexture2D(&desc_tex, nullptr, p_layer.GetAddressOf())))
            throw "Failed to create font texture.";

        D3D11_MAPPED_SUBRESOURCE res_mapped;
        _p_context->Map(p_layer.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &res_mapped);
        unsigned char* p_bits = (unsigned char*)res_mapped.pData;
        for (int y = 0; y < kHeight; ++y) {
            for (int x = 0; x < kWidth; ++x) {
                if (x < kOfsX || y < kOfsY || x >= kOfsX + kWidthBmp || y >= kOfsY + kHeightBmp) {
                    DWORD col = 0x00000000;
                    memcpy(p_bits + res_mapped.RowPitch * y + 4 * x, &col, sizeof(DWORD));
                } else {
                    DWORD alp = (255 * p_mono[x - kOfsX + kWidthBmp * (y - kOfsY)]) / 16;
                    DWORD col = 0x00ffffff | (alp << 24);
                    memcpy(p_bits + res_mapped.RowPitch * y + 4 * x, &col, sizeof(DWORD));
                }
            }
        }
        _p_context->Unmap(p_layer.Get(), 0U);

        delete p_mono;

        D3D11_SHADER_RESOURCE_VIEW_DESC desc_srview;
        ZeroMemory(&desc_srview, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        desc_srview.Format = desc_tex.Format;
        desc_srview.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc_srview.Texture2D.MostDetailedMip = 0;
        desc_srview.Texture2D.MipLevels = desc_tex.MipLevels;
        if (FAILED(_p_device->CreateShaderResourceView(p_layer.Get(), &desc_srview, p_image->p_srview.GetAddressOf())))
            throw "Failed to create shader resource view.";

    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }
    return true;
}

// ================================================================================================================= //
//                                          Initialize                                                               //
// ================================================================================================================= //

#include "_pshader.h"
#include "_vshader.h"

LRESULT WINAPI WndProc(HWND h_wnd, unsigned int msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(h_wnd, msg, wParam, lParam);
}

bool D3DManager::init(HINSTANCE h_inst, int cmd_show, LPCWSTR name_wnd, LPCWSTR name_wndclass, unsigned int width,
    unsigned int height, bool windowed) {
    try {
        // Create Window
        {
            const DWORD kDwStyle = windowed ? WS_OVERLAPPED | WS_SYSMENU | WS_MINIMIZEBOX : WS_POPUP;
            const int kCmdShow = windowed ? cmd_show : SW_SHOWMAXIMIZED;

            WNDCLASSEXW wcex = {sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, h_inst, nullptr, nullptr, nullptr,
                nullptr, name_wndclass, nullptr};
            if (!RegisterClassExW(&wcex))
                throw "Failed to register window class.";

            RECT rect;
            rect = {0, 0, (long)width, (long)height};
            AdjustWindowRectEx(&rect, kDwStyle, false, 0);

            _h_wnd = CreateWindowExW(0, name_wndclass, name_wnd, kDwStyle, CW_USEDEFAULT, CW_USEDEFAULT,
                rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, h_inst, nullptr);
            if (!_h_wnd)
                throw "Failed to create window handle.";

            ShowWindow(_h_wnd, kCmdShow);
            UpdateWindow(_h_wnd);
            ShowCursor(windowed);
        }

        // Create Factory
        ComPtr<IDXGIFactory> p_factory = nullptr;
        if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)p_factory.GetAddressOf())))
            throw "Failed to create factory.";

        // Create Device
        {
            D3D_FEATURE_LEVEL feature_levels[] = {
                D3D_FEATURE_LEVEL_11_1,
                D3D_FEATURE_LEVEL_11_0,
            };
            if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT,
                    feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, _p_device.GetAddressOf(), nullptr,
                    _p_context.GetAddressOf())))
                throw "Failed to create Direct3D11 device.";
        }

        // Create SwapChain
        {
            DXGI_SWAP_CHAIN_DESC desc_swapchain = {
                {width, height, {60U, 1U}, DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
                    DXGI_MODE_SCALING_UNSPECIFIED},
                {1, 0}, DXGI_USAGE_RENDER_TARGET_OUTPUT, 1U, _h_wnd, true, DXGI_SWAP_EFFECT_DISCARD,
                DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH};

            if (FAILED(p_factory->CreateSwapChain(_p_device.Get(), &desc_swapchain, _p_swapchain.GetAddressOf())))
                throw "Failed to create swapchain.";
        }

        // Create Render Target
        {
            ComPtr<ID3D11Texture2D> p_backbuffer = nullptr;
            if (FAILED(_p_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)p_backbuffer.GetAddressOf())))
                throw "Failed to get back buf.";

            if (FAILED(_p_device->CreateRenderTargetView(p_backbuffer.Get(), nullptr, _p_rtview.GetAddressOf())))
                throw "Failed to create render target view.";
        }

        // Create Depth Stencil Buffer
        {
            D3D11_DEPTH_STENCIL_DESC desc_dsbuf = {true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS, false,
                D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK, D3D11_STENCIL_OP_KEEP,
                D3D11_STENCIL_OP_KEEP};

            ComPtr<ID3D11DepthStencilState> p_dsstate = nullptr;
            if (FAILED(_p_device->CreateDepthStencilState(&desc_dsbuf, p_dsstate.GetAddressOf())))
                throw "Failed to create depth stancil stencil.";
            _p_context->OMSetDepthStencilState(p_dsstate.Get(), 0U);

            D3D11_TEXTURE2D_DESC desc_dstex = {width, height, 1U, 1U, DXGI_FORMAT_R24G8_TYPELESS, {1, 0},
                D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE, 0U, 0U};

            ComPtr<ID3D11Texture2D> p_dstex = nullptr;
            if (FAILED(_p_device->CreateTexture2D(&desc_dstex, nullptr, p_dstex.GetAddressOf())))
                throw "Failed to create depth stencil buf texture.";

            D3D11_DEPTH_STENCIL_VIEW_DESC desc_view = {
                DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_DSV_DIMENSION_TEXTURE2D, 0U, {0U}};
            if (FAILED(_p_device->CreateDepthStencilView(p_dstex.Get(), &desc_view, _p_dsview.GetAddressOf())))
                throw "Failed to create depth stencil buf view.";
        }

        // Create Shader
        if (FAILED(_p_device->CreateVertexShader(g_vs_main, sizeof(g_vs_main), nullptr, _p_vshader.GetAddressOf())))
            throw "Failed to create vertex shader.";
        if (FAILED(_p_device->CreatePixelShader(g_ps_main, sizeof(g_ps_main), nullptr, _p_pshader.GetAddressOf())))
            throw "Failed to create pixel shader.";

        // Create input element desc
        {
            D3D11_INPUT_ELEMENT_DESC desc_elem[] = {
                {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT,
                    D3D11_INPUT_PER_VERTEX_DATA, 0},
                {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
                    0},
                {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,
                    0},
            };
            if (FAILED(_p_device->CreateInputLayout(
                    desc_elem, ARRAYSIZE(desc_elem), g_vs_main, sizeof(g_vs_main), _p_ilayout.GetAddressOf())))
                throw "Failed to create input element.";
        }

        // Create constant buf
        {
            D3D11_BUFFER_DESC desc_cb = {
                sizeof(ConstantBuffer), D3D11_USAGE_DEFAULT, D3D11_BIND_CONSTANT_BUFFER, 0U, 0U, 0U};
            if (FAILED(_p_device->CreateBuffer(&desc_cb, nullptr, _p_cbuf.GetAddressOf())))
                throw "Failed to create constant buf.";

            ZeroMemory(&_cbuf, sizeof(ConstantBuffer));
            setMatrixScale(1.0f, 1.0f, 1.0f);
            setMatrixRotate(0.0f, 0.0f, 0.0f);
            setMatrixTranslate(0.0f, 0.0f, 0.0f);
            setMatrixView(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
            setMatrixProject(width, height, DirectX::XM_PIDIV4, 1.0f, 2000.0f, true);
            setVectorColor(1.0f, 1.0f, 1.0f, 1.0f);
            setVectorLight(0.0f, 0.0f, 1.0f, 0.0f);
            setVectorParams(0.0f, 0.0f, 0.0f, 0.0f);
        }

        // Set render configure
        {
            // Viewport
            D3D11_VIEWPORT viewport = {0.0f, 0.0f, (float)width, (float)height, 0.0f, 1.0f};
            _p_context->RSSetViewports(1U, &viewport);

            // Blend mode
            D3D11_BLEND_DESC desc_blend;
            ZeroMemory(&desc_blend, sizeof(D3D11_BLEND_DESC));
            desc_blend.AlphaToCoverageEnable = FALSE;
            desc_blend.IndependentBlendEnable = FALSE;
            desc_blend.RenderTarget[0].BlendEnable = TRUE;
            desc_blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
            desc_blend.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
            desc_blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
            desc_blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
            desc_blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
            desc_blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
            desc_blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
            ComPtr<ID3D11BlendState> p_bstate = nullptr;
            if (FAILED(_p_device->CreateBlendState(&desc_blend, p_bstate.GetAddressOf())))
                throw "Failed to create blend state.";
            float blend_factor[4] = {D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO};
            _p_context->OMSetBlendState(p_bstate.Get(), blend_factor, 0xffffffff);

            // Drawing
            _p_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            _p_context->IASetInputLayout(_p_ilayout.Get());
            _p_context->VSSetShader(_p_vshader.Get(), nullptr, 0U);
            _p_context->PSSetShader(_p_pshader.Get(), nullptr, 0U);
        }

    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }

    return true;
}
