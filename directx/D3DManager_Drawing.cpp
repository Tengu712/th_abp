#include "HeaderDX11.hpp"

void D3DManager::drawBegin(FrameBuffer* pFBuffer) {
    _pImContext->OMSetRenderTargets(1U, 
            pFBuffer == nullptr ? _pRTView.GetAddressOf() : pFBuffer->pRTView.GetAddressOf(),
            _isDepth ? _pDSView.Get() : nullptr);
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    _pImContext->ClearRenderTargetView(pFBuffer == nullptr ? _pRTView.Get() : pFBuffer->pRTView.Get(), clearColor);
    if (_isDepth)
        _pImContext->ClearDepthStencilView(_pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);
}

void D3DManager::drawEnd() {
    _pSwapChain->Present(1U, 0U);
}

void D3DManager::enableDepthStencil(bool enable) {
    _isDepth = enable;
}

void D3DManager::setViewport(D3D11_VIEWPORT* pViewport) {
    if (pViewport == nullptr)
        return;
    _pImContext->RSSetViewports(1U, pViewport);
}

void D3DManager::createCamera(float width, float height, Camera* camera){
    *camera = {width, height, DirectX::XM_PIDIV4, 1.0f, 2000.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
}

void D3DManager::applyCamera(Camera* camera, bool parse) {
    DirectX::XMStoreFloat4x4(&_cbuffer.matView, DirectX::XMMatrixTranspose(
                DirectX::XMMatrixLookToLH(
                    DirectX::XMVectorSet(camera->posX, camera->posY, camera->posZ, 0.0f),
                    DirectX::XMVectorSet(camera->dirX, camera->dirY, camera->dirZ, 0.0f),
                    DirectX::XMVectorSet(camera->uppX, camera->uppY, camera->uppZ, 0.0f))));
    DirectX::XMStoreFloat4x4(&_cbuffer.matProj, DirectX::XMMatrixTranspose(
                parse ? 
                DirectX::XMMatrixPerspectiveFovLH(camera->angle, camera->width / camera->height, camera->nearZ, camera->farZ)
                :
                DirectX::XMMatrixOrthographicLH(camera->width, camera->height, camera->nearZ, camera->farZ)));
}
