float screenWidth;
float screenHeight;
texture tex;

sampler tipSampler = sampler_state
{
    texture = <tex>;
    MipFilter = LINEAR;
    MinFilter = POINT;
    MagFilter = POINT;
};

struct VS_IN
{
    float2 pos : POSITION;
    float2 localUV : TEXCOORD0;
    float2 worldPos : TEXCOORD1;
    float2 tipUV : TEXCOORD2;
    float4 col : COLOR0;
};

struct VS_OUT
{
    float4 pos : POSITION;
    float4 col : COLOR0;
    float2 uv : TEXCOORD0;
};

VS_OUT vsMain(VS_IN In)
{
    VS_OUT Out;
    Out.pos = float4(
        (In.pos.x + In.worldPos.x - screenWidth) / screenWidth,
        -(In.pos.y + In.worldPos.y - screenHeight) / screenHeight,
        0.0f,
        1.0f
    );
    Out.col = In.col;
    Out.uv = In.tipUV + In.localUV;

    return Out;
}

float4 psMain(VS_OUT In) : COLOR0
{
    return In.col * tex2D(tipSampler, In.uv);
}

technique InstancingTech
{
    pass p0
    {
        VertexShader = compile vs_2_0 vsMain();
        PixelShader = compile ps_2_0 psMain();
    }
}