// Global variables

#define SMAP_SIZE	1024
#define SMAP_HEIGHT (1024 * 3)

#define SHADOW_EPSILON 0.00005f
#define NUM_SPLITS 3

const float4 g_SplitTestColors[4] = {{1.f, 0.f, 0.f, 1.f},
									{0.f, 1.f, 0.f, 1.f},
									{0.f, 0.f, 1.f, 1.f},
									{1.f, 1.f, 0.f, 1.f}};
									
const float TexShift[4] = {{0.f},
						   {1.f / 3.f},
						   {2.f / 3.f},
						   {1.f}};
// blur samples						   
const float2 samples[12] = {
   -0.326212, -0.405805,
   -0.840144, -0.073580,
   -0.695914,  0.457137,
   -0.203345,  0.620716,
    0.962340, -0.194983,
    0.473434, -0.480026,
    0.519456,  0.767022,
    0.185461, -0.893124,
    0.507431,  0.064425,
    0.896420,  0.412458,
   -0.321940, -0.932615,
   -0.791559, -0.597705,
};

//> VARS
float4x4 worldViewProj		: MWorldViewProjection;
float4x4 worldView			: MWorldView;
float4x4 matWorld			: MWorld;
float4x4 ViewToLightProj	: MViewToLightProj;  // Transform from view space to light projection space
float4x4 AViewToLightProj[4];

float3 LightColor;
float3 LightVector;
float3 LightPosition = float4(0.f, 0.f, 0.f, 1.f);
float3 ViewPosition;
float3 g_vLightPos;
float3 g_vLightDir;
float4 g_vSplitDistances;

float	Ambient;
float	opacity : Opacity; //global opacity value
float4	owverrideColor : OvwerrideColor;
float	fCosTheta;  // Cosine of theta of the spot light
bool	bReciveShadow;

texture DiffuseMap;
texture NormalMap;
texture EnvironmentMap;
texture ShadowMap;
//> END_VARS

sampler DiffuseMapSampler = sampler_state
{
	Texture = <DiffuseMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler NormalMapSampler = sampler_state
{
	Texture = <NormalMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler EnvironmentMapSampler = sampler_state
{
	Texture = <EnvironmentMap>;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

sampler2D ShadowMapSampler = sampler_state
{
    Texture = <ShadowMap>;
    MinFilter = Point;  
    MagFilter = Point;
    MipFilter = Point;
    AddressU  = Border;
    AddressV  = Border;
    BorderColor = 0xffffffff;
};

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

struct VS_OUTPUT_DIFFUSE
{
	float4 Position				: POSITION;
	float3 Normal				: TEXCOORD0;
	float4 vPosition			: TEXCOORD1;
	float2 TexCoord0			: TEXCOORD2;
	float4 wPosition			: TEXCOORD3;
	float4 texCoord[3]			: TEXCOORD4;
	float3 Color0				: COLOR0;
};

// float3 LightVector			: TEXCOORD3;
// float3 vLightTS				: TEXCOORD4;
// float3 vViewTS				: TEXCOORD5;
// float4 vPosLight				: TEXCOORD6;

struct VS_OUTPUT_SHADOW
{
	float4 Position				: POSITION;
	float2 TexCoord				: TEXCOORD0;
	float4 Depth				: TEXCOORD1;
};

//-----------------------------------------------------------------------
VS_OUTPUT_DIFFUSE Diffuse_VS(VS_INPUT IN)
{
	VS_OUTPUT_DIFFUSE OUT;
	
	// Transform to screen coord
	OUT.Position = mul(float4(IN.Position, 1), worldViewProj);
	
	// Transform position to view space
	OUT.vPosition = mul(float4(IN.Position, 1), worldView);
	
	// world space position
	OUT.wPosition = mul(float4(IN.Position, 1), matWorld);
	
	// pass texture coordinates for fetching the diffuse map
	OUT.TexCoord0 = IN.TexCoord0.xy;
	
	OUT.Normal = normalize(mul(IN.Normal, (float3x3)matWorld));
	
	float3x3 MatrixTBN;
	MatrixTBN[0] = mul(IN.S, matWorld);
	MatrixTBN[1] = mul(IN.T, matWorld);
	MatrixTBN[2] = mul(IN.SxT, matWorld);
	
	// Compute and output the world view vector:
 	float3 vViewWS = normalize(ViewPosition - OUT.wPosition);
 	float3 vLightWS = normalize(LightPosition - OUT.wPosition);
 	
	//OUT.vViewTS = mul(MatrixTBN, vViewWS);
	//OUT.vLightTS = mul(MatrixTBN, vLightWS);
	
	//OUT.LightVector = normalize(OUT.vLightTS);

    // Transform the position to light projection space, or the
    // projection space as if the camera is looking out from
    // the spotlight.

    OUT.texCoord[0] = mul(float4(IN.Position, 1), AViewToLightProj[0]);
	OUT.texCoord[1] = mul(float4(IN.Position, 1), AViewToLightProj[1]);
	OUT.texCoord[2] = mul(float4(IN.Position, 1), AViewToLightProj[2]);
	
	OUT.Color0 = IN.Color;
	
	return OUT;
}

//-----------------------------------------------------------------------
float3 offset_lookup(sampler2D map, float2 Position, float2 Offset)
{
	return tex2D(ShadowMapSampler, Position + Offset);
}

//-----------------------------------------------------------------------
float4 Diffuse_PS(VS_OUTPUT_DIFFUSE IN) : COLOR
{
	float4 DiffuseMap = tex2D(DiffuseMapSampler, IN.TexCoord0.xy);
	int iCurrentSplitIndex = 0;
	
	// vLight is the unit vector from the light to this pixel
    float3 vLight = normalize(IN.wPosition.xyz - LightPosition);
    
    float4 Diffuse = float4(1.f, 1.f, 1.f, 1.f);
     
	float distance = IN.vPosition.z;
	int iFind = 0;
	
	for (int i = 0; i < NUM_SPLITS; i++)
	{
		if (iFind == 0 && distance < g_vSplitDistances[i])
		{
		 	iCurrentSplitIndex = i;
		 	iFind = 1;
		}
	}

	float LightAmount = 1.f;
	
	float2 ShadowTexC = 0.5f * (IN.texCoord[iCurrentSplitIndex].xy / IN.texCoord[iCurrentSplitIndex].w) + float2(0.5f, 0.5f);
	ShadowTexC.y = (1.0f - ShadowTexC.y) / 3.f;
	ShadowTexC.y += TexShift[iCurrentSplitIndex];
	
// test	
	
/*
	if ((ShadowTexC.x > 0.f && ShadowTexC.x < 1.f) &&
 		(ShadowTexC.y > TexShift[iCurrentSplitIndex] && ShadowTexC.y < TexShift[iCurrentSplitIndex + 1]))
 	{
 		// 2x2 pcf
		// transform to texel space
		float2 texelpos = float2(ShadowTexC.x * SMAP_SIZE, ShadowTexC.y * SMAP_HEIGHT);
				
		// Determine the lerp amounts           
		float2 lerps = frac(texelpos);
				
		//read in bilerp stamp, doing the shadow checks
		float sourcevals[4];
		sourcevals[0] = (tex2D(ShadowMapSampler, ShadowTexC) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z) ? 0.1f: 1.0f;
				
		sourcevals[1] = (tex2D(ShadowMapSampler, ShadowTexC + float2(1.f/SMAP_SIZE, 0)) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z)? 0.1f: 1.0f;  
		sourcevals[2] = (tex2D(ShadowMapSampler, ShadowTexC + float2(0.f, 1.0/SMAP_HEIGHT)) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z)? 0.1f: 1.0f;  
		sourcevals[3] = (tex2D(ShadowMapSampler, ShadowTexC + float2(1.f/SMAP_SIZE, 1.0/SMAP_HEIGHT)) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z)? 0.1f: 1.0f;  
        
		LightAmount = lerp( lerp( sourcevals[0], sourcevals[1], lerps.x ),
		 						  lerp( sourcevals[2], sourcevals[3], lerps.x ),
								  lerps.y );
	}
*/	
	Diffuse = saturate(dot(-vLight, normalize(IN.Normal))) * LightAmount;
	Diffuse.w = 1.f;
			
    return DiffuseMap; //DiffuseMap * Diffuse * g_SplitTestColors[iCurrentSplitIndex];
}

float4 Diffuse_PS_Red(VS_OUTPUT_DIFFUSE IN) : COLOR
{
	 return float4(1.f, 1.f, 0.f, 1.f);
}

//-----------------------------------------------------------------------
/*
float4 Diffuse_Bump_PS(VS_OUTPUT_DIFFUSE IN) : COLOR
{
	//fetch base color
    float4 color = tex2D(DiffuseMapSampler, IN.TexCoord0.xy);

    //fetch bump normal and unpack it to [-1..1] range
    float3 bumpNormal = 2 * tex2D(NormalMapSampler, IN.TexCoord0.xy) - 1;
    
    // Compute diffuse color component:
	float3 vLightTSAdj = float3(IN.vLightTS.x, IN.vLightTS.y, IN.vLightTS.z);

    //compute diffuse lighting coefficient
    float self_shadow = saturate(dot(bumpNormal, normalize(vLightTSAdj)));
    
    //compute self-shadowing term
    float shadow = saturate(dot(normalize(LightVector), IN.Normal));
    
    //compute final color
    return  float4(LightColor, 1.f) * saturate(self_shadow * shadow * Ambient); // x color
}

//-----------------------------------------------------------------------
float4 Parallax_Mapping_PS(VS_OUTPUT_DIFFUSE IN) : COLOR
{
	// Sample the height map at the current texture coordinate:
	float fCurrentHeight = tex2D(NormalMapSampler, IN.TexCoord0).a * 0.04 - 0.02;
		
	float3 vViewTS = normalize(IN.vViewTS);
	
	float2 texSample = IN.TexCoord0 + vViewTS.xy * fCurrentHeight;

	float4 color = tex2D(DiffuseMapSampler, texSample);
	
	 //fetch bump normal and unpack it to [-1..1] range
    float3 bumpNormal = 2 * tex2D(NormalMapSampler, texSample) - 1;
    
    // light in tangent space
	float3 vLightTSAdj = float3(IN.vLightTS.x, -IN.vLightTS.y, IN.vLightTS.z);

    //compute diffuse lighting coefficient
    float self_shadow = saturate(dot(bumpNormal, normalize(vLightTSAdj)));
    
    //compute self-shadowing term
    float shadow = saturate(dot(normalize(LightVector), IN.Normal));

	return float4(LightColor, 1.f) * color * saturate(self_shadow * shadow * Ambient);
}
*/

//-----------------------------------------------------------------------
VS_OUTPUT_SHADOW Shadow_VS(VS_INPUT IN)
{
	VS_OUTPUT_SHADOW OUT;
	
	OUT.Position = mul(float4(IN.Position, 1), worldViewProj);
	OUT.TexCoord = IN.TexCoord0.xy;
	OUT.Depth = OUT.Position;

	return OUT;
}

//-----------------------------------------------------------------------
float4 Shadow_PS(VS_OUTPUT_SHADOW IN) : COLOR
{
	float4 DiffuseMap = tex2D(DiffuseMapSampler, IN.TexCoord.xy);
	float depth = IN.Depth.z;
	return float4(depth.xxx, DiffuseMap.w);
}

//-----------------------------------------------------------------------
// Tehniques
//-----------------------------------------------------------------------
Technique DiffuseOnly
{
	Pass P0
	{
		AlphaFunc = Greater;
		AlphaTestEnable = true;
		AlphaRef = 0x00000011;
		//CullMode=CCW;
		
		VertexShader = compile vs_2_0 Diffuse_VS();
		PixelShader  = compile ps_3_0 Diffuse_PS();
		
	}
}

Technique DiffuseOnlyRed
{
	Pass P0
	{
		AlphaFunc = Greater;
		AlphaTestEnable = true;
		AlphaRef = 0x00000011;
		//CullMode=CCW;
		
		VertexShader = compile vs_2_0 Diffuse_VS();
		PixelShader  = compile ps_3_0 Diffuse_PS_Red();
		
	}
}
/*
Technique DiffuseBump
{
	Pass P0
	{
		VertexShader = compile vs_2_0 Diffuse_VS();
		PixelShader  = compile ps_2_0 Diffuse_Bump_PS();
	}
}

Technique Parallax
{
	Pass P0
	{
		VertexShader = compile vs_2_0 Diffuse_VS();
		PixelShader = compile ps_2_0 Parallax_Mapping_PS();
	}
}
*/

technique RenderShadow
{
	Pass P0
	{
		AlphaFunc = Greater;
		AlphaTestEnable = true;
		AlphaRef = 0x00000011;
		//CullMode=CW;
		
		VertexShader = compile vs_2_0 Shadow_VS();
		PixelShader = compile ps_2_0 Shadow_PS();
	}
}
