#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

LightPixelInput VS(VertexUVNormalTangent input)// : SV_POSITION//SV->RS
{
    LightPixelInput output;
    output.pos = mul(input.pos, world);
    
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) world);
    output.tangent = mul(input.tangent, (float3x3) world);
    output.binormal = cross(output.normal, output.tangent);
    output.viewPos = invView._41_42_43;
    
    return output;
}

cbuffer TileBuffer : register(b10)
{
    float2 coord;
    float2 size;
}

float4 PS(LightPixelInput input) : SV_TARGET //->SV->OM
{
    Material material = GetMaterial(input);
    
    float4 result = CalcDirectional(material, lights[0]);    
    float4 ambient = CalcAmbient(material);
    
    result += ambient;
    
    if(input.uv.x > coord.x && input.uv.x < coord.x + size.x)
    {
        if(input.uv.y > coord.y && input.uv.y < coord.y + size.y)
            return result + mEmissive;
    }    
    
    return result;
}