texture tex;
matrix mtxView;
matrix mtxProj;

sampler tipSampler = sampler_state
{
    texture = <tex>;
    MipFilter = LINEAR;
    MinFilter = POINT;
    MagFilter = POINT;
};

struct VS_IN
{
    //�匳
    float3 pos : POSITION;
    float3 nor : NORMAL0;
    float2 localUV : TEXCOORD0;
    
    //�C���X�^���V���O_�}�g��
    float4 mtxWorld0 : TEXCOORD1;
    float4 mtxWorld1 : TEXCOORD2;
    float4 mtxWorld2 : TEXCOORD3;
    float4 mtxWorld3 : TEXCOORD4;
    
    //�C���X�^���V���O_UV
    float2 tipUV : TEXCOORD5;
    
    //�C���X�^���V���O_�J���[
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
    
    // �e�s��4x4�}�g���ɓ����
    matrix mtxWorld;
    mtxWorld._11_12_13_14 = In.mtxWorld0;
    mtxWorld._21_22_23_24 = In.mtxWorld1;
    mtxWorld._31_32_33_34 = In.mtxWorld2;
    mtxWorld._41_42_43_44 = In.mtxWorld3;
    
    // ���_�ϊ�
    float4 pos = mul(float4(In.pos, 1.0f), mtxWorld);
    pos = mul(pos, mtxView);
    pos = mul(pos, mtxProj);

    // �������
    Out.pos = pos;
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