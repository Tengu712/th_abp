cbuffer mats {
    float4x4 matScl;
    float4x4 matRot;
    float4x4 matTrs;
    float4x4 matView;
    float4x4 matProj;
    float4 vecColor;
    float4 vecLight;
}

struct VS_IN {
    float4 pos : POSITION0;
    float4 col : COLOR0;
    float4 nor : NORMAL0;
    float2 img : TEXCOORD0;
};

struct VS_OUT {
    float4 pos : SV_Position;
    float4 col : COLOR0;
    float2 img : TEXCOORD0;
};

VS_OUT vs_main(VS_IN input) {
    VS_OUT output;

    output.pos = mul(input.pos, matScl);
    output.pos = mul(output.pos, matRot);
    output.pos = mul(output.pos, matTrs);
    output.pos = mul(output.pos, matView);
    output.pos = mul(output.pos, matProj);

    float4 col = input.col * vecColor;
    float3 nor = normalize(mul(input.nor, matRot).xyz);
    output.col = float4(col.xyz * (dot(-nor, vecLight.xyz) * 0.5 + 0.5), col.w);

    output.img = input.img;

    return output;
}
