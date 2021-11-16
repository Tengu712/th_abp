#include "_dx11private.hpp"

bool D3DManager::createModelBuffers(unsigned int numVtx, Vertex* dataPCU, unsigned int* dataIdx, ModelInf* minf) {
    D3D11_BUFFER_DESC descVBuffer = {
        sizeof(Vertex) * numVtx, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0U, 0U, 0U};
    D3D11_SUBRESOURCE_DATA dataVBuffer = {dataPCU, 0U, 0U};
    if (FAILED(_pDevice->CreateBuffer(&descVBuffer, &dataVBuffer, minf->pVBuffer.GetAddressOf())))
        return false;

    D3D11_BUFFER_DESC descIndex = {
        (unsigned int)sizeof(unsigned int) * minf->numIdx, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0U, 0U, 0U};
    D3D11_SUBRESOURCE_DATA dataIndex = {dataIdx, 0U, 0U};
    if (FAILED(_pDevice->CreateBuffer(&descIndex, &dataIndex, minf->pIBuffer.GetAddressOf())))
        return false;

    return true;
}

bool D3DManager::createFrameBuffer(unsigned int width, unsigned int height, FrameBuffer* pFBuffer) {
    try {
        if (pFBuffer == nullptr)
            throw "Nullptr gained when create a frame buffer.";

        D3D11_TEXTURE2D_DESC descTex = {
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
        ComPtr<ID3D11Texture2D> pTex = nullptr;
        if (FAILED(_pDevice->CreateTexture2D(&descTex, nullptr, pTex.GetAddressOf())))
            throw "Failed to create texture2D for frame buffer.";

        D3D11_RENDER_TARGET_VIEW_DESC descRTView;
        ZeroMemory(&descRTView, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
        descRTView.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        descRTView.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        if (FAILED(_pDevice->CreateRenderTargetView(pTex.Get(), &descRTView, pFBuffer->pRTView.GetAddressOf())))
            throw "Failed to create render target view for frame buffer.";

        D3D11_SHADER_RESOURCE_VIEW_DESC descSRView;
        ZeroMemory(&descSRView, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
        descSRView.Format = descRTView.Format;
        descSRView.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        descSRView.Texture2D.MipLevels = 1U;
        if (FAILED(_pDevice->CreateShaderResourceView(pTex.Get(), &descSRView, pFBuffer->image.pSRView.GetAddressOf())))
            throw "Failed to create shader resource view for frame buffer.";

    } catch (const char* error) {
        ErrorMessage(error);
        return false;
    }

    return true;
}
