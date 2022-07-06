struct VS_INPUT
{
   float4 mPos      : POSITION;
   float3 mNormal   : NORMAL;
   float2 mUV	    : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 mPos     : POSITION;
    float2 mUV	    : TEXCOORD0;
    float4 mDiffuse : TEXCOORD1;
    float3 mNormal  : TEXCOORD2;
    float3 X        : TEXCOORD3;
    float3 mViewDir	: TEXCOORD4;
    float3 lightDir : TEXCOORD5;
};

float4 vLightPos;
float4 vColor;
float4 vEyePos;

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProj : Projection;

VS_OUTPUT MetalShader_Pass_0_Vertex_Shader_vs_main( VS_INPUT In )
{
   VS_OUTPUT Out;

   Out.mPos = mul(In.mPos, matWorld);
   float4 worldPosition = Out.mPos;
   Out.mPos = mul(Out.mPos, matView);
   Out.mPos = mul(Out.mPos, matProj);

   float amb = vLightPos.w;
   float3 lightDir = worldPosition.xyz - vLightPos.xyz;
   lightDir = normalize(lightDir);
   Out.lightDir = lightDir;
   
   float3 viewDir = normalize(worldPosition.xyz - vEyePos.xyz);
   Out.mViewDir = viewDir;
   
   float3 worldNormal = mul( In.mNormal, (float3x3)matWorld );
   Out.mNormal = normalize(worldNormal);
   Out.X = In.mPos.xyz;
   
   Out.mDiffuse = vColor * max(amb, dot(worldNormal, -lightDir));
   Out.mUV = In.mUV;
   
   return Out;
}

struct PS_INPUT
{
    float4 mPos     : POSITION;
    float2 mUV      : TEXCOORD0;
    float4 mDiffuse : TEXCOORD1;
    float3 mNormal  : TEXCOORD2;
    float3 X        : TEXCOORD3;
    float3 mViewDir	: TEXCOORD4;
    float3 lightDir : TEXCOORD5;
};

texture ReflectMap;
sampler2D ReflectSampler = sampler_state
{
   Texture = (ReflectMap);
};

float4 MetalShader_Pass_0_Pixel_Shader_ps_main( in PS_INPUT In) : COLOR
{
   float3 L = normalize(-In.lightDir);
   float3 N = normalize(In.mNormal);
   float3 V = normalize(vEyePos.xyz - In.X);
   float3 H = normalize(L + V);
   
   float NV = dot(N,V);
   float NH = dot(N,H);
   float VH = dot(V,H);
   float NL = dot(N,L);
   float LH = dot(L,H);
   
   float m = 0.2f;
   float NH2 = NH*NH;
   float D = exp(-(1-NH2)/(NH2*m*m))/(4*m*m*NH2*NH2);

   float G = min(1,min(2*NH*NV/VH, 2*NH*NL/VH));
   
   float n = 3.0f;
   float g = sqrt(n*n+LH*LH-1);
   float gpc = g+LH;
   float gnc = g-LH;
   float cgpc = LH*gpc-1;
   float cgnc = LH*gnc+1;
   float F = 0.5f*gnc*gnc*(1+cgpc*cgpc/(cgnc*cgnc))/(gpc*gpc);
   float F0 = ((n-1)*(n-1))/((n+1)*(n+1));
   
   float3 viewDir = normalize(In.mViewDir);

   float3 viewReflect = reflect(viewDir, In.mNormal);
   //float3 environment2 = tex2D(ReflectSampler, viewReflect).rgb;
   float3 environment = tex2D(ReflectSampler, In.mUV).rgb;
   //environment = environment + environment2;
   //environment /= 2;
   
   float4 light_color = {0.296, 0.304, 1.000f, 1.0f};
   float4 c0 = {0.386f * light_color.x
            , 0.333f * light_color.y
            , 0.185f * light_color.z, 1.0f};
   float4 color = c0 + (light_color-c0)*max(0,F-F0)/(1-F0);
   float4 res = In.mDiffuse + color * max(0,F*D*G/NV);
   //res *= 0.6f;
   res.rgb += environment.rgb * 0.4f;

    return float4(res);
}

technique MetalShader
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 MetalShader_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_a MetalShader_Pass_0_Pixel_Shader_ps_main();
   }
}

