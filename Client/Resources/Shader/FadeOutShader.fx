struct VS_INPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
};

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProj : Projection;

VS_OUTPUT FadeOut_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul( Input.mPosition, matWorld );
   Output.mPosition = mul( Output.mPosition, matView );
   Output.mPosition = mul( Output.mPosition, matProj );

   Output.mTexCoord = Input.mTexCoord;
   
   return Output;
}

texture DiffuseMap;
sampler2D FadeOutSampler = sampler_state
{
	Texture = (DiffuseMap);
};

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
};

float4 vColor;
float fFadeVal;
 
float4 FadeOut_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR
{
    float2 uv = Input.mTexCoord;
    uv -= 0.5f;
    uv /= fFadeVal;
    uv += 0.5f;
    if(uv.x >= 1.f || uv.x <= 0.f || uv.y >= 1.f || uv.y <= 0.f) return vColor;
    else return vColor * tex2D(FadeOutSampler, uv);
}

technique FadeOutShader
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 FadeOut_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 FadeOut_Pass_0_Pixel_Shader_ps_main();
   }
}