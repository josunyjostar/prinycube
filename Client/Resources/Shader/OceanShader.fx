float4x4 gWorldMatrix : World;
float4x4 gViewMatrix : View;
float4x4 gProjectionMatrix : Projection;

float4 gWorldLightPosition;
float4 gWorldCameraPosition : ViewPosition;

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal: NORMAL;
};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir: TEXCOORD2;
	float3 mReflection: TEXCOORD3;
};

VS_OUTPUT OceanShader_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorldMatrix);

	float3 lightDir = Output.mPosition.xyz - gWorldLightPosition.xyz;
		lightDir = normalize(lightDir);

	float3 viewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);
		Output.mViewDir = viewDir;

	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	float3 worldNormal = mul(Input.mNormal, (float3x3)gWorldMatrix);
		worldNormal = normalize(worldNormal);

	Output.mDiffuse = dot(-lightDir, worldNormal);
	Output.mReflection = reflect(lightDir, worldNormal);

	return Output;
}

struct PS_INPUT
{
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir: TEXCOORD2;
	float3 mReflection: TEXCOORD3;
};

float fAlpha;
float fLightAmount;
 
float4 OceanShader_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input) : COLOR
{
	float3 diffuse = saturate(Input.mDiffuse);

	float3 reflection = normalize(Input.mReflection);
	float3 viewDir = normalize(Input.mViewDir);
	float3 specular = 0;
	if (diffuse.x > 0)
	{
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 200.0f);
	}

	float3 ambient = float3(0.1f, 0.1f, 0.1f);
	
	diffuse.x *= 0.0;	diffuse.y *= 0.3; diffuse.z *=0.7;
	specular *= 0.5;

	diffuse *= fLightAmount;

	return float4(ambient + diffuse + specular, fAlpha);
}

technique OceanShader
{
	pass Pass_0
	{
      ALPHABLENDENABLE = TRUE;
      BLENDOP = ADD;
      DESTBLEND = INVSRCALPHA;
      SRCBLEND = SRCALPHA;
		VertexShader = compile vs_2_0 OceanShader_Pass_0_Vertex_Shader_vs_main();
		PixelShader = compile ps_2_0 OceanShader_Pass_0_Pixel_Shader_ps_main();
	}
}

