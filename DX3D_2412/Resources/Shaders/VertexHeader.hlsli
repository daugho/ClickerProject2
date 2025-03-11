//VertexLayouts

struct Vertex
{
    float4 pos : POSITION;
};

struct VertexUV
{
    float4 pos : POSITION;
    float2 uv : UV;
};

struct VertexColor
{
    float4 pos : POSITION;
    float4 color : COLOR;
};

struct VertexUVNormal
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct VertexUVNormalTangent
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VertexUVNormalTangentBlend
{
    float4 pos : POSITION;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float4 indices : BLENDINDICES;
    float4 weights : BLENDWEIGHTS;
};

//Const Buffer
cbuffer WorldBuffer : register(b0)
{
    matrix world;
}

cbuffer ViewBuffer : register(b1)
{
    matrix view;
    matrix invView;
}

cbuffer ProjectionBuffer : register(b2)
{
    matrix projection;
}

struct Frame
{
    int clip;
    int curFrame;
    float time;
    float scale;
};

struct Motion
{
    float duration;
    float tweenTime;
    float runningTime;
    float playTime;
	
    Frame cur, next;
};

cbuffer FrameBuffer : register(b3)
{
    Motion motion;
}

Texture2DArray transformMap : register(t0);

matrix LoadTransform(int index, int frame, int clip)
{
    float4 c0 = transformMap.Load(int4(index * 4 + 0, frame, clip, 0));
    float4 c1 = transformMap.Load(int4(index * 4 + 1, frame, clip, 0));
    float4 c2 = transformMap.Load(int4(index * 4 + 2, frame, clip, 0));
    float4 c3 = transformMap.Load(int4(index * 4 + 3, frame, clip, 0));
	
    return matrix(c0, c1, c2, c3);
}

matrix SkinWorld(float4 indices, float4 weights)
{
    matrix transform = 0;
    matrix curAnim, nextAnim;
    
    for (int i = 0; i < 4; i++)
    {
        if(weights[i] == 0.0f)
            continue;
        
        int clip = motion.cur.clip;
        int curFrame = motion.cur.curFrame;
        
        matrix cur = LoadTransform(indices[i], curFrame, clip);
        matrix next = LoadTransform(indices[i], curFrame + 1, clip);
        
        curAnim = lerp(cur, next, motion.cur.time);
        
        if (motion.next.clip > -1)
        {
            matrix nextCur = LoadTransform(indices[i], motion.next.curFrame, motion.next.clip);
            matrix nextNext = LoadTransform(indices[i], motion.next.curFrame + 1, motion.next.clip);
            
            nextAnim = lerp(nextCur, nextNext, motion.next.time);
            curAnim = lerp(curAnim, nextAnim, motion.tweenTime);
        }
        
        transform += weights[i] * curAnim;
    }
    
    return transform;
}