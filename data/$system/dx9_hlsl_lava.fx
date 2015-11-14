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
//> VARS
float4x4 worldViewProj		: MWorldViewProjection;
float4x4 worldView			: MWorldView;
float4x4 matWorld			: MWorld;
float4x4 ViewToLightProj	: MViewToLightProj;  // Transform from view space to light projection space
float4x4 AViewToLightProj[4];
float3 LightColor;
float3 LightVector;
float3 LightPosition;
float3 ViewPosition;
float3 g_vLightPos;
float3 g_vLightDir;
float4 g_vSplitDistances;
float Ambient;
float opacity : Opacity; //global opacity value
float4 owverrideColor : OvwerrideColor;
float fCosTheta;  // Cosine of theta of the spot light
bool bReciveShadow;
float time_0_X;
texture DiffuseMap;
texture NormalMap;
texture EnvironmentMap;
texture ShadowMap;
texture Noise_Tex;
texture Flame_Tex;
//> END_VARS


float noiseScale = float( 0.21 );
float morphRate = float( 0.05 );
float bubbleRate = float( 0.12 );
float bubbleScale = float( 0.95 );
float scale = float( 0.82 );
float bias = float( -0.12 );

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


struct VS_OUTPUT_SHADOW
{
	float4 Position				: POSITION;
	float2 TexCoord				: TEXCOORD0;
	float4 Depth				: TEXCOORD1;
};

sampler Flame = sampler_state
{
   Texture = (Flame_Tex);
   ADDRESSU = CLAMP;
   ADDRESSV = CLAMP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

sampler Noise = sampler_state
{
   Texture = (Noise_Tex);
   ADDRESSU = WRAP;
   ADDRESSV = WRAP;
   ADDRESSW = WRAP;
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

sampler DiffuseMapSampler = sampler_state
{
	Texture = <DiffuseMap>;
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

//----------------------------------------------------------------------------------------------
VS_OUTPUT_DIFFUSE Lava_VS(VS_INPUT IN)
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

//----------------------------------------------------------------------------------------------
// We use noise as our base texture. Then we use more noisy to
// bubble in that noise. Both the base noise and bubble noise is
// animated, though the bubbling at a higher rate.

float4 Lava_PS(VS_OUTPUT_DIFFUSE IN) : COLOR 
{
	float4 DiffuseMap = tex2D(DiffuseMapSampler, IN.TexCoord0.xy);
// 	int iCurrentSplitIndex = 0;
// 	
// 	// vLight is the unit vector from the light to this pixel
//     float3 vLight = normalize(IN.wPosition.xyz - LightPosition);
//     
//     float4 Diffuse = float4(1.f, 1.f, 1.f, 1.f);
//      
// 	float distance = IN.vPosition.z;
// 	int iFind = 0;
// 	
// 	for (int i = 0; i < NUM_SPLITS; i++)
// 	{
// 		if (iFind == 0 && distance < g_vSplitDistances[i])
// 		{
// 		 	iCurrentSplitIndex = i;
// 		 	iFind = 1;
// 		}
// 	}

// 	float2 ShadowTexC = 0.5f * (IN.texCoord[iCurrentSplitIndex].xy / IN.texCoord[iCurrentSplitIndex].w) + float2(0.5f, 0.5f);
// 	ShadowTexC.y = (1.0f - ShadowTexC.y) / 3.f;
// 	ShadowTexC.y += TexShift[iCurrentSplitIndex];
// 
// 	if ((ShadowTexC.x > 0.f && ShadowTexC.x < 1.f) &&
//  		(ShadowTexC.y > TexShift[iCurrentSplitIndex] && ShadowTexC.y < TexShift[iCurrentSplitIndex + 1]))
//  	{
//  		// 2x2 pcf
// 		// transform to texel space
// 		float2 texelpos = float2(ShadowTexC.x * SMAP_SIZE, ShadowTexC.y * SMAP_HEIGHT);
// 				
// 		// Determine the lerp amounts           
// 		float2 lerps = frac(texelpos);
// 				
// 		//read in bilerp stamp, doing the shadow checks
// 		float sourcevals[4];
// 		sourcevals[0] = (tex2D(ShadowMapSampler, ShadowTexC) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z) ? 0.1f: 1.0f;
// 				
// 		sourcevals[1] = (tex2D(ShadowMapSampler, ShadowTexC + float2(1.f/SMAP_SIZE, 0)) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z)? 0.1f: 1.0f;  
// 		sourcevals[2] = (tex2D(ShadowMapSampler, ShadowTexC + float2(0.f, 1.0/SMAP_HEIGHT)) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z)? 0.1f: 1.0f;  
// 		sourcevals[3] = (tex2D(ShadowMapSampler, ShadowTexC + float2(1.f/SMAP_SIZE, 1.0/SMAP_HEIGHT)) + SHADOW_EPSILON < IN.texCoord[iCurrentSplitIndex].z)? 0.1f: 1.0f;  
//         
// 		float LightAmount = lerp( lerp( sourcevals[0], sourcevals[1], lerps.x ),
// 		 						  lerp( sourcevals[2], sourcevals[3], lerps.x ),
// 								  lerps.y );
// 		Diffuse = saturate(dot(-vLight, normalize(IN.Normal))) * LightAmount;
// 		Diffuse.w = 1.f;
// 	}
			
//    // Bubble coords
	float3 coord;
	coord.xy = IN.TexCoord0 * bubbleScale;
	coord.z = bubbleRate * time_0_X;

	// Sample noise for x and y
	float noiseX = tex3D(Noise, coord).r - 0.5;
	float noiseY = tex3D(Noise, coord + 0.5).r - 0.5;

	// Offset our base noise with the bubble noise
	coord.x = IN.TexCoord0.x + noiseX * noiseScale;
	coord.y = IN.TexCoord0.y + noiseY * noiseScale;
	coord.z = time_0_X * morphRate;

	// Sample the base noise
	float base = tex3D(Noise, coord).r;
	 
	// Assign a firey color from the base noise
	return tex1D(Flame, scale * base + bias);
}

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

//--------------------------------------------------------------//
// Technique Section for Effect Workspace.Fire Effects.Lava
//--------------------------------------------------------------//
technique Lava
{
   pass Single_Pass
   {
      CULLMODE = NONE;

      VertexShader = compile vs_2_0 Lava_VS();
      PixelShader = compile ps_3_0 Lava_PS();
   }
}
//-----------------------------------------------------------------------
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
