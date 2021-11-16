#include "_dx11private.hpp"

void D3DManager::drawBegin(FrameBuffer* pFBuffer) {
    _pImContext->OMSetRenderTargets(1U,
        pFBuffer == nullptr ? _pRTView.GetAddressOf() : pFBuffer->pRTView.GetAddressOf(),
        _isDepth ? _pDSView.Get() : nullptr);
    float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    _pImContext->ClearRenderTargetView(pFBuffer == nullptr ? _pRTView.Get() : pFBuffer->pRTView.Get(), clearColor);
    if (_isDepth) _pImContext->ClearDepthStencilView(_pDSView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0U);
}

void D3DManager::drawModel(ModelInf* minf) {
    unsigned int strides = sizeof(Vertex);
    unsigned int offsets = 0U;
    _pImContext->IASetVertexBuffers(0U, 1U, minf->pVBuffer.GetAddressOf(), &strides, &offsets);
    _pImContext->IASetIndexBuffer(minf->pIBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);
    _pImContext->UpdateSubresource(_pCBuffer.Get(), 0U, nullptr, &_cbuffer, 0U, 0U);
    _pImContext->VSSetConstantBuffers(0U, 1U, _pCBuffer.GetAddressOf());
    _pImContext->DrawIndexed(minf->numIdx, 0U, 0U);
}

void D3DManager::drawEnd() {
    _pSwapChain->Present(1U, 0U);
}

void D3DManager::enableDepthStencil(bool enable) {
    _isDepth = enable;
}

void D3DManager::setViewport(D3D11_VIEWPORT* pViewport) {
    if (pViewport == nullptr) return;
    _pImContext->RSSetViewports(1U, pViewport);
}
