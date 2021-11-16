#include <wincodec.h>

#include "_dx11private.hpp"

#pragma comment(lib, "Windowscodecs.lib")
#pragma comment(lib, "Ole32.lib")

bool D3DManager::createImage(HMODULE hModule, unsigned int id, Image* pImage) {
    try {
        if (pImage == nullptr)
            throw "Nullptr gained when creating an image.";
        pImage->id = id;

        HRSRC hImageRes = FindResourceA(hModule, MAKEINTRESOURCEA(id), "IMAGE");
        if (!hImageRes)
            throw "Failed to find resource.";

        HGLOBAL hImageData = LoadResource(hModule, hImageRes);
        if (!hImageData)
            throw "Failed to load resource.";

        void* pLock = LockResource(hImageData);
        if (!pLock)
            throw "Failed to lock resource.";

        DWORD sizeRes = SizeofResource(hModule, hImageRes);
        if (sizeRes == 0)
            throw "Failed to get size of resource.";

        ComPtr<IWICImagingFactory> pFactory = nullptr;
        if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory,
                (LPVOID*)pFactory.GetAddressOf())))
            throw "Failed to create wic factory.";

        ComPtr<IWICStream> pStream = nullptr;
        if (FAILED(pFactory->CreateStream(pStream.GetAddressOf())))
            throw "Failed to create stream.";

        if (FAILED(pStream->InitializeFromMemory((byte*)pLock, sizeRes)))
            throw "Failed to initialize stream.";

        ComPtr<IWICBitmapDecoder> pDecoder = nullptr;
        if (FAILED(pFactory->CreateDecoderFromStream(
                pStream.Get(), nullptr, WICDecodeMetadataCacheOnLoad, pDecoder.GetAddressOf())))
            throw "Failed to load file.";

        ComPtr<IWICBitmapFrameDecode> pFrame = nullptr;
        if (FAILED(pDecoder->GetFrame(0U, pFrame.GetAddressOf())))
            throw "Failed to get frame.";

        ComPtr<IWICFormatConverter> pFormatConverter = nullptr;
        if (FAILED(pFactory->CreateFormatConverter(pFormatConverter.GetAddressOf())))
            throw "Failed to create format converter.";

        if (FAILED(pFormatConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone,
                nullptr, 1.0f, WICBitmapPaletteTypeMedianCut)))
            throw "Failed to initialize format.";

        unsigned int width, height;
        if (FAILED(pFormatConverter->GetSize(&width, &height)))
            throw "Failed to get texture size.";

        D3D11_TEXTURE2D_DESC descTex = {width, height, 1U, 1U, DXGI_FORMAT_R8G8B8A8_UNORM, {1U, 0U},
            D3D11_USAGE_DYNAMIC, D3D11_BIND_SHADER_RESOURCE, D3D11_CPU_ACCESS_WRITE, 0U};
        ComPtr<ID3D11Texture2D> pTex = nullptr;
        if (FAILED(_pDevice->CreateTexture2D(&descTex, nullptr, pTex.GetAddressOf())))
            throw "Failed to create texture resource.";

        D3D11_MAPPED_SUBRESOURCE resMapped;
        ZeroMemory(&resMapped, sizeof(D3D11_MAPPED_SUBRESOURCE));
        if (FAILED(_pImContext->Map(pTex.Get(), 0U, D3D11_MAP_WRITE_DISCARD, 0U, &resMapped)))
            throw "Failed to map.";
        if (FAILED(pFormatConverter->CopyPixels(nullptr, width * 4U, width * height * 4U, (BYTE*)resMapped.pData)))
            throw "Failed to copy pixels.";
        _pImContext->Unmap(pTex.Get(), 0U);

        D3D11_SHADER_RESOURCE_VIEW_DESC descShader;
        ZeroMemory(&descShader, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        descShader.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        descShader.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        descShader.Texture2D.MipLevels = 1U;
        if (FAILED(_pDevice->CreateShaderResourceView(pTex.Get(), &descShader, pImage->pSRView.GetAddressOf())))
            throw "Failed to create shader resource view.";
    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }

    return true;
}

void D3DManager::applyImage(Image* pImage) {
    if (pImage == nullptr || pImage->pSRView == nullptr) {
        _cbuffer.params.x = 0.0f;
    } else {
        _pImContext->PSSetShaderResources(0U, 1U, pImage->pSRView.GetAddressOf());
        _cbuffer.params.x = 1.0f;
    }
}
