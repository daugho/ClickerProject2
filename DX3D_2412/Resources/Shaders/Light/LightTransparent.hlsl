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
    
    float4 color = result + ambient + mEmissive;
    
    return float4(color.rgb, material.baseColor.a * mDiffuse.a);
}