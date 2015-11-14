
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

float opacity : Opacity; //global opacity value
float4 owverrideColor : OvwerrideColor;

float3	 LightColor;
float3   LightVector;
float3	 LightPosition;
float3	 ViewPosition;
float    Ambient;

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
	
	OUT.TexCoord = IN.TexCoord0;

	return OUT;
}

//-----------------------------------------------------------------------------
VS_OUTPUT2 Diffuse_Color_vs( VS_INPUT2 IN )
{
    VS_OUTPUT2 OUT;

	OUT.hposition = mul(float4(IN.Position, 1), worldViewProj);

	return OUT;
}

//-----------------------------------------------------------------------------
// Simple Pixel Shader
//-----------------------------------------------------------------------------
float4 Diffuse_ps( VS_OUTPUT IN ) : COLOR0
{
    return float4(tex2D(Sampler, IN.TexCoord) * IN.color, opacity);
}

//-----------------------------------------------------------------------------
float4 Diffuse_ps_vc( VS_OUTPUT IN ) : COLOR0
{
	return float4(IN.color, 1.f);
}
//-----------------------------------------------------------------------------
float4 Diffuse_Color_ps( VS_OUTPUT2 IN ) : COLOR0
{
	return owverrideColor;
}

//-----------------------------------------------------------------------------
VS_OUTPUT QuadVS(VS_INPUT IN)
{
	VS_OUTPUT OUT;
	OUT.hposition = mul(float4(IN.Position, 1), worldViewProj);
	OUT.TexCoord = IN.TexCoord0;
	OUT.color = IN.Color;
	return OUT;
}

//-----------------------------------------------------------------------------
float4 DrawShadowPS(VS_OUTPUT IN) : COLOR
{
    return float4(tex2D(Sampler, IN.TexCoord).rgb, 1.0);
}

//-------------------------------------------------------------------
// TECHNIQUES
Technique Technique_Diffuse
{
    pass P0
    {
		//Lighting = FALSE;
		AlphaTestEnable = true;
		AlphaFunc = greater;
		AlphaRef = 0x11000011;
		//CullMode = 0;
		//AlphaBlendEnable = false; //where i started the alpha addition
		
		Sampler[0] = (Sampler); // Needed by pixel shader
		
		//SrcBlend = SrcAlpha;
        //DestBlend = InvSrcAlpha;
		
		VertexShader = compile vs_2_0 Diffuse_vs();
		PixelShader  = compile ps_2_0 Diffuse_ps();
    }
}

Technique Technique_Selected
{
     pass P0
     {
 		Lighting = FALSE;
 		FillMode = Wireframe;
 		CullMode = None;
 		
 		VertexShader = compile vs_2_0 Diffuse_vs();
 		PixelShader  = compile ps_2_0 Diffuse_ps_vc();
     }
}

//-------------------------------------------------------------------
Technique Technique_DiffuseVC
{
    pass P0
    {
		Lighting = FALSE;
		//CullMode = 0;

		VertexShader = compile vs_2_0 Diffuse_vs();
		PixelShader  = compile ps_2_0 Diffuse_ps_vc();
    }
}

//-------------------------------------------------------------------
Technique Technique_DiffuseColorVC
{
    pass P0
    {
		Lighting = FALSE;
		//CullMode = 0;

		VertexShader = compile vs_2_0 Diffuse_Color_vs();
		PixelShader  = compile ps_2_0 Diffuse_Color_ps();
    }
}

//-------------------------------------------------------------------
technique DrawHardwareShadowMap
{
    Pass P0
    {
        VertexShader = compile vs_2_0 QuadVS();
        PixelShader = compile ps_2_0 DrawShadowPS();
    }
}

//-------------------------------------------------------------------
Technique Technique_DiffuseUI
{
    pass P0
    {
		Lighting = FALSE;
		//CullMode = 0;

		VertexShader = compile vs_2_0 Diffuse_vs();
		PixelShader  = compile ps_2_0 Diffuse_ps_vc();
    }
}