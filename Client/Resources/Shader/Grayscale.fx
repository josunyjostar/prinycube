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

VS_OUTPUT GrayScale_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul( Input.mPosition, matWorld );
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
};

float4 GrayScale_Pass_0_Pixel_Shader_ps_main( PS_INPUT Input ) : COLOR
{
    float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
   albedo.rgb = dot(albedo.rgb, float3(0.3, 0.59, 0.11));
	return albedo;
}

technique GrayScale
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 GrayScale_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 GrayScale_Pass_0_Pixel_Shader_ps_main();
   }
}