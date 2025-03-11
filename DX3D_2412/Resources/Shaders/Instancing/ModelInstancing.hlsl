#include "../VertexHeader.hlsli"
#include "../PixelHeader.hlsli"

struct VertexInput
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
	
    matrix transform : INSTANCE_TRANSFORM;
};

struct PixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : POSITION0;
    float3 viewPos : POSITION1;
};

LightPixelInput VS(VertexInput input)
{
    LightPixelInput output;
    output.pos = mul(input.pos, input.transform);
    
    output.worldPos = output.pos;
    
    output.pos = mul(output.pos, view);
    output.pos = mul(output.pos, projection);
    
    output.uv = input.uv;
    output.normal = mul(input.normal, (float3x3) input.transform);
    output.tangent = mul(input.tangent, (float3x3) input.transform);
    output.binormal = cross(output.normal, output.tangent);
    output.viewPos = invView._41_42_43;
    
    return output;
}

float4 PS(LightPixelInput input) : SV_TARGET //->SV->OM
{
    Material material = GetMaterial(input);
    
    float4 ambient = CalcAmbient(material);
    
    float4 result = 0;
    
    [unroll(MAX_LIGHT)]
    for (int i = 0; i < lightCount; i++)
    {
        if (!lights[i].isActive)
            continue;

        if (lights[i].type == 0)
            result += CalcDirectional(material, lights[i]);
        else if (lights[i].type == 1)
            result += CalcPoint(material, lights[i]);
        else if (lights[i].type == 2)
            result += CalcSpot(material, lights[i]);
    }
    
    return result + ambient + mEmissive;
}