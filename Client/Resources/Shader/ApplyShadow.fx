struct VS_INPUT 
{
   float4 mPosition: POSITION;
   float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT 
{
   float4 mPosition: POSITION;
   float2 mTexCoord : TEXCOORD0;
   float4 mClipPosition: TEXCOORD1;
};

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProj : Projection;

float4x4 matLightView : View;
float4x4 matLightProj : Projection;

VS_OUTPUT ApplyShadowShader_Vertex_Shader_vs_main( VS_INPUT Input )
{
    VS_OUTPUT Output;

    Output.mPosition = mul(Input.mPosition, matWorld);

    Output.mClipPosition = mul(Output.mPosition, matLightView);
    Output.mClipPosition = mul(Output.mClipPosition, matLightProj);

	Output.mPosition = mul(Output.mPosition, matView);
	Output.mPosition = mul(Output.mPosition, matProj);

    Output.mTexCoord = Input.mTexCoord;

    return Output;
}

texture ShadowMap;
sampler2D ShadowSampler = sampler_state
{
    Texture = (ShadowMap);
};

texture DiffuseMap;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap);
};

struct PS_INPUT
{
    float2 mTexCoord : TEXCOORD0;
    float4 mClipPosition: TEXCOORD1;
};

float fDiffuse;

float4 ApplyShadowShader_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);

    albedo.rgb *= fDiffuse;

    float2 uv = Input.mClipPosition.xy;
    uv.y = -uv.y;
    uv = uv * 0.5 + 0.5;

    if (!(uv.x < 0 || uv.y < 0 || uv.x > 1 || uv.y >1))
    {
        float4 shadowDepth = tex2D(ShadowSampler, uv);

        albedo.rgb *= (shadowDepth.r * shadowDepth.a * 0.5f)+0.5f;
    }

    return albedo;
}

technique ApplyShadowShader
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 ApplyShadowShader_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ApplyShadowShader_Pixel_Shader_ps_main();
   }
}