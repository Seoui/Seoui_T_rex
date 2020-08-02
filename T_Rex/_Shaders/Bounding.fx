float4 LineColor;

cbuffer CB_PerFrame
{
    matrix View;
    matrix Projection;
}

matrix World;

//-----------------------------------------------------------------------------
//Types
//-----------------------------------------------------------------------------
struct VertexInput
{
    float4 Position : POSITION0; // 0 �Ⱥٿ��� �� �ε��� �˾Ƽ� �߰��Ѵ�
};

struct VertexOutput
{
    float4 Position : SV_POSITION0; // SV_�� �پ���� ������ȭ ������ ���� �����ϴ�
};

//-----------------------------------------------------------------------------
//Vertex Shader  ������ȯ�� ���⼭ ���ش�. W, V, P�� ���̴��� �Ѿ�;� �Ѵ�.
//-----------------------------------------------------------------------------
VertexOutput VS(VertexInput input)
{
    VertexOutput output;
    output.Position = mul(input.Position, World);
    output.Position = mul(output.Position, View);
    output.Position = mul(output.Position, Projection);

    return output;
}

//-----------------------------------------------------------------------------
//Pixel Shader
//-----------------------------------------------------------------------------
float4 PS(VertexOutput input) : SV_TARGET0
{
    return LineColor;
}

//-----------------------------------------------------------------------------
//PipeLine
//-----------------------------------------------------------------------------
technique11 T0  //�������� ���⼭ ����
{
    pass P0
    {
        SetVertexShader(CompileShader(vs_5_0, VS())); // VS() ���� �Լ� �̸��� ���ƾ� �Ѵ�
        SetPixelShader(CompileShader(ps_5_0, PS()));  // PS() ���� �Լ� �̸��� ���ƾ� �Ѵ�
    }
}