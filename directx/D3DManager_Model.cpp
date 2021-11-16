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

void D3DManager::drawModel(ModelInf* minf) {
    unsigned int strides = sizeof(Vertex);
    unsigned int offsets = 0U;
    _pImContext->IASetVertexBuffers(0U, 1U, minf->pVBuffer.GetAddressOf(), &strides, &offsets);
    _pImContext->IASetIndexBuffer(minf->pIBuffer.Get(), DXGI_FORMAT_R32_UINT, 0U);

    DirectX::XMStoreFloat4x4(&_cbuffer.matScl, DirectX::XMMatrixTranspose(
        DirectX::XMMatrixScaling(minf->sclX, minf->sclY, minf->sclZ)));
    DirectX::XMStoreFloat4x4(&_cbuffer.matRot, DirectX::XMMatrixTranspose(
        DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(minf->degX),
            DirectX::XMConvertToRadians(minf->degY),
            DirectX::XMConvertToRadians(minf->degZ))));
    DirectX::XMStoreFloat4x4(&_cbuffer.matTrs, DirectX::XMMatrixTranspose(
            DirectX::XMMatrixTranslation(minf->posX, minf->posY, minf->posZ)));
    DirectX::XMStoreFloat4(&_cbuffer.vecColor,
        DirectX::XMVectorSet(minf->colR, minf->colG, minf->colB, minf->colA));

    _pImContext->UpdateSubresource(_pCBuffer.Get(), 0U, nullptr, &_cbuffer, 0U, 0U);
    _pImContext->VSSetConstantBuffers(0U, 1U, _pCBuffer.GetAddressOf());

    _pImContext->DrawIndexed(minf->numIdx, 0U, 0U);
}

