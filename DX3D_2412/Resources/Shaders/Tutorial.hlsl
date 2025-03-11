//Shader(셰이더) : Shade + er -> GPU 프로그래밍
//Sementic(시멘틱) : 반환값이나 매개변수의 사용 용도
//SV(System Value)

cbuffer WorldBuffer : register(b0)
{
    matrix world;   
}

cbuffer ViewBuffer : register(b1)
{
    matrix view;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct VertexInput
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

PixelInput VS( VertexInput input )// : SV_POSITION//SV->RS
{
    PixelInput output;
    output.pos = mul(input.pos, world);
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    output.color = input.color;
    
	return output;
}

float4 PS( PixelInput input) : SV_TARGET//->SV->OM
{
    return input.color;
}