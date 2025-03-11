#define MAX_LIGHT 10

//Struct
struct Material
{
    float4 baseColor;
    float3 normal;
    float4 specularIntensity;
    float3 worldPos;
    float3 viewDir;
};

struct LightPixelInput
{
    float4 pos : SV_POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float3 worldPos : POSITION0;
    float3 viewPos : POSITION1;
};

struct Light
{
    float4 color;
    
    float3 direction;
    float attentionIntensity;
    
    float3 position;
    float range;
    
    float inner;
    float outer;
    int type;
    int isActive;
};

//Texture
Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D normalMap : register(t2);

SamplerState samp : register(s0);

//Constant Buffer
cbuffer MaterialBuffer : register(b0)
{
    float4 mDiffuse;
    float4 mSpecular;
    float4 mAmbient;
    float4 mEmissive;
	
    float shininess;
    int isNormalMap;
}

cbuffer LightBuffer : register(b1)
{
    Light lights[MAX_LIGHT];
    
    float4 ambientColor;
    float4 ambientCeil;   
    
    int lightCount;
}

//Function
float3 GetNormal(float3 T, float3 B, float3 N, float2 uv)
{   
    if (!isNormalMap)
    {
        return normalize(N);
    }
	
    T = normalize(T);
    B = normalize(B);
    N = normalize(N);

    float3 normal = normalMap.Sample(samp, uv).rgb;
    normal = normal * 2.0f - 1.0f;
    float3x3 TBN = float3x3(T, B, N);
	
    return normalize(mul(normal, TBN));
}

Material GetMaterial(LightPixelInput input)
{
    Material material;
    material.baseColor = diffuseMap.Sample(samp, input.uv);
    material.normal = GetNormal(input.tangent, input.binormal, input.normal, input.uv);
    material.specularIntensity = specularMap.Sample(samp, input.uv);
    material.worldPos = input.worldPos;
    material.viewDir = normalize(input.worldPos - input.viewPos);
	
    return material;
}

float4 CalcAmbient(Material material)
{
    float up = material.normal.y * 0.5f + 0.5f;
	
    float4 ambient = ambientColor + up * ambientCeil;
	
    return material.baseColor * ambient * mAmbient;
}

float4 CalcDirectional(Material material, Light light)
{
    float3 toLight = normalize(light.direction);
	
    float diffuseInstensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
    if (diffuseInstensity > 0)
    {
		//Blinn Phong Shading
        float3 halfWay = normalize(material.viewDir + toLight);
        float specular = saturate(dot(material.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
    }

    return finalColor * material.baseColor;
}

float4 CalcPoint(Material material, Light light)
{
    float3 toLight = material.worldPos - light.position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
	
    float diffuseInstensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
    if (diffuseInstensity > 0)
    {
        float3 halfWay = normalize(material.viewDir + toLight);
        float specular = saturate(dot(material.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
    }
	
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = pow(distanceToLightNormal, light.attentionIntensity);    

    return finalColor * material.baseColor * attention;
}

float4 CalcSpot(Material material, Light light)
{
    float3 toLight = material.worldPos - light.position;
    float distanceToLight = length(toLight);
    toLight /= distanceToLight;
	
    float diffuseInstensity = saturate(dot(material.normal, -toLight));
    float4 finalColor = light.color * diffuseInstensity * mDiffuse;
	
    if (diffuseInstensity > 0)
    {
        float3 halfWay = normalize(material.viewDir + toLight);
        float specular = saturate(dot(material.normal, -halfWay));

        finalColor += light.color * pow(specular, shininess) *
		material.specularIntensity * mSpecular;
    }
	
    float3 dir = normalize(light.direction);
    float cosAngle = dot(dir, toLight);
	
    float outer = cos(radians(light.outer));
    float inner = 1.0f / cos(radians(light.inner));
	
    float conAttention = saturate((cosAngle - outer) * inner);
    conAttention = pow(conAttention, light.attentionIntensity);
	
    float distanceToLightNormal = 1.0f - saturate(distanceToLight / light.range);
    float attention = pow(distanceToLightNormal, light.attentionIntensity);

    return finalColor * material.baseColor * attention * conAttention;
}