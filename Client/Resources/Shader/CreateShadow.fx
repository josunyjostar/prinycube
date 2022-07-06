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
float4x4 matLightView : View;
float4x4 matLightProj : Projection;

VS_OUTPUT CreateShadowShader_CreateShadow_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, matWorld);
	Output.mPosition = mul(Output.mPosition, matLightView);
	Output.mPosition = mul(Output.mPosition, matLightProj);

	Output.mTexCoord = Input.mTexCoord;
	Output.mClipPosition = Output.mPosition;

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
	float4 mClipPosition: TEXCOORD1;
};

float fAlpha;

float4 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	float depth;

	float2 uv = Input.mClipPosition.xy;
	uv.y = -uv.y;
	uv = uv * 0.5 + 0.5;

	if (!(uv.x < 0 || uv.y < 0 || uv.x > 1 || uv.y >1))
	{
		depth = 0.f;
	}
	
	return float4(depth.xxx, fAlpha*albedo.a);
}

technique CreateShadowShader
{
	pass Pass_0
	{
		VertexShader = compile vs_2_0 CreateShadowShader_CreateShadow_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 CreateShadowShader_CreateShadow_Pixel_Shader_ps_main();
	}
}

