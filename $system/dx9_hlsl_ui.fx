
//-----------------------------------------------------------------------------
// Effect File Variables
//-----------------------------------------------------------------------------

//> VARS
float4x4 worldViewProj : WorldViewProjection; // This matrix will be loaded by the application
float4x4 matWorld	   : MWorld;
float4x4 matWInverse   : MWorldInverse;
float4x4 matView	   : MView;
float4x4 matViewInverse: MViewInverse;
float4x4 worldView	   : MWorldView;
float4x4 matTexTransform: MTexcoordTransform;

float opacity : Opacity; //global opacity value
float4 owverrideColor : OvwerrideColor;

float3	 LightColor;
float3   LightVector;
float3	 LightPosition;
float3	 ViewPosition;
float    Ambient;
float4	 TextureOffset;

texture DiffuseMap; // This texture will be loaded by the application
texture NormalMap;
texture EnvironmentMap;
//> END_VARS

sampler Sampler = sampler_state
{
    Texture   = (DiffuseMap);
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};

//-----------------------------------------------------------------------------
// Vertex Definitions
//-----------------------------------------------------------------------------

// Our sample application will send vertices 
// down the pipeline laid-out like this...

struct VS_INPUT
{
    float3 Position   : POSITION;
    float3 Normal     : NORMAL;
    float3 Color      : COLOR0;
    float2 TexCoord0  : TEXCOORD0;
    float2 TexCoord1  : TEXCOORD1;
    float3 S          : TEXCOORD2;
    float3 T          : TEXCOORD3;
    float3 SxT        : TEXCOORD4;
    float4 Weights    : TEXCOORD5;
    float4 Indices    : TEXCOORD6;
};

struct VS_INPUT2
{
	float3 Position : POSITION; //in object space
};

// Once the vertex shader is finished, it will 
// pass the vertices on to the pixel shader like this...

struct VS_OUTPUT
{
    float4 hposition : POSITION;
	float2 TexCoord  : TEXCOORD0;
    float3 color	 : COLOR0;
};

struct VS_OUTPUT2
{
    float4 hposition : POSITION;
};
// And finally, the pixel shader will send a single 
// color value to the frame buffer like this...

struct PS_OUTPUT
{
	float4 color : COLOR;
};

//-----------------------------------------------------------------------------
// Simple Vertex Shader
//-----------------------------------------------------------------------------

VS_OUTPUT Diffuse_vs( VS_INPUT IN )
{
    VS_OUTPUT OUT;

	OUT.hposition = mul(float4(IN.Position, 1), worldViewProj);

	OUT.color = IN.Color; // Pass white as a default color
	
	float2 ModifText =  mul(float4(IN.TexCoord0.xy, 1.f, 1.f), matTexTransform);
	
	OUT.TexCoord = ModifText; //float2(IN.TexCoord0.x + matTexTransform[2][0], IN.TexCoord0.y + matTexTransform[2][1]);//ModifText;

	return OUT;
}

//-----------------------------------------------------------------------------
// Simple Pixel Shader
//-----------------------------------------------------------------------------
float4 Diffuse_ps( VS_OUTPUT IN ) : COLOR0
{
    return float4(tex2D(Sampler, IN.TexCoord) * IN.color, opacity);
}

//-------------------------------------------------------------------
// TECHNIQUES
//-------------------------------------------------------------------
Technique Technique_DiffuseUI
{
    pass P0
    {
		Lighting = FALSE;
		AlphaFunc = Greater;
		AlphaTestEnable = true;
		AlphaRef = 0x00000011;

		VertexShader = compile vs_1_1 Diffuse_vs();
		PixelShader  = compile ps_1_1 Diffuse_ps();
    }
}