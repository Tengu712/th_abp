#include "_dx11private.hpp"

void D3DManager::setMatrixScale(float sclX, float sclY, float sclZ) {
    DirectX::XMStoreFloat4x4(&_cbuffer.matScl, DirectX::XMMatrixTranspose(DirectX::XMMatrixScaling(sclX, sclY, sclZ)));
}

void D3DManager::setMatrixRotate(float degX, float degY, float degZ) {
    DirectX::XMStoreFloat4x4(&_cbuffer.matRot,
        DirectX::XMMatrixTranspose(DirectX::XMMatrixRotationRollPitchYaw(
            DirectX::XMConvertToRadians(degX), DirectX::XMConvertToRadians(degY), DirectX::XMConvertToRadians(degZ))));
}

void D3DManager::setMatrixTranslate(float posX, float posY, float posZ) {
    DirectX::XMStoreFloat4x4(
        &_cbuffer.matTrs, DirectX::XMMatrixTranspose(DirectX::XMMatrixTranslation(posX, posY, posZ)));
}

void D3DManager::setMatrixView(
    float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float uppX, float uppY, float uppZ) {
    DirectX::XMStoreFloat4x4(&_cbuffer.matView,
        DirectX::XMMatrixTranspose(DirectX::XMMatrixLookToLH(DirectX::XMVectorSet(posX, posY, posZ, 0.0f),
            DirectX::XMVectorSet(dirX, dirY, dirZ, 0.0f), DirectX::XMVectorSet(uppX, uppY, uppZ, 0.0f))));
}

void D3DManager::setMatrixProject(float width, float height, float angle, float nearZ, float farZ, bool parse) {
    DirectX::XMStoreFloat4x4(&_cbuffer.matProj,
        DirectX::XMMatrixTranspose(parse ? DirectX::XMMatrixPerspectiveFovLH(angle, width / height, nearZ, farZ)
                                         : DirectX::XMMatrixOrthographicLH(width, height, nearZ, farZ)));
}

void D3DManager::setVectorColor(float colR, float colG, float colB, float colA) {
    DirectX::XMStoreFloat4(&_cbuffer.vecColor, DirectX::XMVectorSet(colR, colG, colB, colA));
}

void D3DManager::setVectorLight(float x, float y, float z, float w) {
    DirectX::XMStoreFloat4(&_cbuffer.vecLight, DirectX::XMVectorSet(x, y, z, w));
}

void D3DManager::setVectorParams(float x, float y, float z, float w) {
    DirectX::XMStoreFloat4(&_cbuffer.params, DirectX::XMVectorSet(x, y, z, w));
}
