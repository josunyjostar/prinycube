struct VS_INPUT
{
   float4 mPosition : POSITION;
   float3 mNormal: NORMAL;
   float2 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition : POSITION;
   float2 mTexCoord : TEXCOORD0;
   float4 mDiffuse : TEXCOORD1;
};

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProj : Projection;

float4 vLightDir;
float4 vColor;

VS_OUTPUT DiffuseColorShader_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul( Input.mPosition, matWorld );

   float3 vWorldNormal = normalize(mul(Input.mNormal, (float3x3)matWorld));
   Output.mDiffuse = saturate(dot(-vLightDir, vWorldNormal)) + float4(0.1f, 0.1f, 0.1f, 1.f);
   Output.mDiffuse *= vColor;

   Output.mPosition = mul( Output.mPosition, matView );
   Output.mPosition = mul( Output.mPosition, matProj );

   Output.mTexCoord = Input.mTexCoord;
   
   return Output;
}

texture DiffuseMap;
sampler2D DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap);
};

struct PS_INPUT
{
	float2 mTexCoord : TEXCOORD0;
    float4 mDiffuse : TEXCOORD1;
};
 
float4 DiffuseColorShader_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	albedo *= Input.mDiffuse;
	return albedo;
}

technique DiffuseColorShader
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 DiffuseColorShader_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 DiffuseColorShader_Pass_0_Pixel_Shader_ps_main();
   }
}