cbuf mats : register (b0) {
    float4x4 mat_scl;
    float4x4 mat_rot;
    float4x4 mat_trs;
    float4x4 mat_view;
    float4x4 mat_proj;
    float4 vec_color;
    float4 vec_light;
    float4 params;
}

struct VS_IN {
    float4 pos : POSITION0;
    float4 col : COLOR0;
    float4 nor : NORMAL0;
    float2 tex : TEXCOORD0;
};

struct VS_OUT {
    float4 pos : SV_Position;
    float4 col : COLOR0;
    float2 tex : TEXCOORD0;
    float4 prm : COLOR1;
};

VS_OUT vs_main(VS_IN input) {
    VS_OUT output;

    output.pos = mul(input.pos, mat_scl);
    output.pos = mul(output.pos, mat_rot);
    output.pos = mul(output.pos, mat_trs);
    output.pos = mul(output.pos, mat_view);
    output.pos = mul(output.pos, mat_proj);

    float4 nor = mul(input.nor, mat_rot);
    float3 norRes = normalize(nor.xyz);
    float4 col = input.col * vec_color;
    output.col = float4(col.xyz * (dot(-norRes, vec_light.xyz) * 0.5 + 0.5), col.w);

    output.tex = input.tex;
    output.prm = params;

    return output;
}