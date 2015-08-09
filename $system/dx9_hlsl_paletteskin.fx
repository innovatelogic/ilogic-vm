
#define SMAP_SIZE	1024
#define SMAP_HEIGHT (1024 * 3)
#define SHADOW_EPSILON 0.00005f
#define NUM_SPLITS 3

const float3 g_SplitTestColors[4] = {{1.f, 0.f, 0.f},
									{0.f, 1.f, 0.f},
									{0.f, 0.f, 1.f},
									{1.f, 1.f, 0.f}};
const float TexShift[4] = {{0.f},
						   {1.f / 3.f},
						   {2.f / 3.f},
						   {1.f}};
//> VARS
float4x4 worldViewProj;
float4x3 Bones[29];

float4x4 matWorld	   : MWorld;
float4x4 matWInverse   : MWorldInverse;
float4x4 matView	   : MView;
float4x4 worldView     : MWorldView;
float4x4 matWorldView  : MWorldView;
float4x4 matViewInverse: MViewInverse;
float4x4 ViewToLightProj : MViewToLightProj;  // Transform from view space to light projection space
float4x4 AViewToLightProj[4];

float3 LightColor;
float3 LightVector;
float3 LightPosition;
float3 ViewPosition;
float3 g_vLightPos;
float3 g_vLightDir;
float4 g_vSplitDistances;

float    Ambient;
bool	 bReciveShadow;

float opacity : Opacity; //global opacity value
float4 owverrideColor : OvwerrideColor;

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
    AddressU = Clamp;
    AddressV = Clamp;
};

struct VS_INPUT {
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

struct VS_OUTPUT {
    float4 HPosition : POSITION;
    float4 vPosition : TEXCOORD0;
    float2 TexCoord0 : TEXCOORD1;
    float2 TexCoord1 : TEXCOORD2;
    float4 vPosLight : TEXCOORD3;
	float4 wPosition : TEXCOORD4;
	float4 texCoord[3] : TEXCOORD5;
    float3 Color0    : COLOR0;
};

struct VS_OUTPUT_SHADOW
{
	float4 Position				: POSITION;
	float4 Depth				: TEXCOORD0;
};

//-----------------------------------------------------------------------
VS_OUTPUT main(VS_INPUT IN)
{
    VS_OUTPUT OUT;

    float i; // Index into matrix palette

    float4 inPos;
    inPos.xyz = IN.Position;
    inPos.w = 1.0;

    // Pass through texcoords
    OUT.TexCoord0.xy = IN.TexCoord0.xy;
    OUT.TexCoord1.xy = IN.TexCoord0.xy;

	float3 tempPos, tempNormal;

    /////////////////////////////////////////////////////////////////////
    // FIRST BONE
    // We don't worry about the ELSE condition because we defined the 
    // initial conditions.

    // grab first bone matrix
    i = IN.Indices.x;

    // First transformed position and normal
    tempPos = mul(inPos, Bones[i]) * IN.Weights.x;
    tempNormal = mul(IN.Normal, (float3x3)Bones[i]) * IN.Weights.x;

    /////////////////////////////////////////////////////////////////////
    // SECOND BONE
    // Next bone.

    if(IN.Weights.y > 0.0f)
    {
        i = IN.Indices.y;

        // Add second transformed position and normal
        tempPos += mul(inPos, Bones[i]) * IN.Weights.y;
        tempNormal += mul(IN.Normal, (float3x3)Bones[i]) * IN.Weights.y;

        /////////////////////////////////////////////////////////////////////
        // THIRD BONE
        // Note we only skin the normal by the first two bones, these are by 
        // far the most significant.

        if(IN.Weights.z > 0.0f)
        {
            i = IN.Indices.z;

            // Add third transformed position only
            tempPos += mul(inPos, Bones[i]) * IN.Weights.z;

            /////////////////////////////////////////////////////////////////////
            // FOURTH BONE

            if(IN.Weights.w > 0.0f)
            {
                i = IN.Indices.w;
                
                // Add fourth transformed position only
                tempPos += mul(inPos, Bones[i]) * IN.Weights.w;
            }
        }
    }

    // Normalize the final skinned normal
    float3 finalNormal = normalize(tempNormal);
    OUT.Color0.xyz = max(dot(finalNormal, LightVector), 0).xxx;

    float4 finalPos;
    finalPos.xyz = tempPos;
    finalPos.w = 1.0;

    // Transform the final skinned position
    OUT.HPosition = mul(finalPos, worldViewProj);
    
    // Transform position to view space
	OUT.vPosition = mul(finalPos, worldView);
	
	OUT.wPosition = mul(finalPos, matWorld);
	
	//
    // Transform the position to light projection space, or the
    // projection space as if the camera is looking out from
    // the spotlight.
    //
    OUT.vPosLight = mul(OUT.wPosition, ViewToLightProj);
    
    OUT.texCoord[0] = mul(float4(IN.Position, 1), AViewToLightProj[0]);
	OUT.texCoord[1] = mul(float4(IN.Position, 1), AViewToLightProj[1]);
	OUT.texCoord[2] = mul(float4(IN.Position, 1), AViewToLightProj[2]);
	
    return OUT;
}

//-----------------------------------------------------------------------
float4 mainPS(VS_OUTPUT IN) : COLOR
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

	float2 ShadowTexC = 0.5f * (IN.texCoord[iCurrentSplitIndex].xy / IN.texCoord[iCurrentSplitIndex].w) + float2(0.5f, 0.5f);
	ShadowTexC.y = (1.0f - ShadowTexC.y) / 3.f;
	ShadowTexC.y += TexShift[iCurrentSplitIndex];
			
	//Diffuse = float4(g_SplitTestColors[i], 1.f);
	
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
        
		float LightAmount = lerp( lerp( sourcevals[0], sourcevals[1], lerps.x ),
		 						  lerp( sourcevals[2], sourcevals[3], lerps.x ),
								  lerps.y );
		Diffuse = LightAmount;
		Diffuse.w = 1.f;
	}
			
    return DiffuseMap * Diffuse;// * g_SplitTestColors[iCurrentSplitIndex];
}

//-----------------------------------------------------------------------
VS_OUTPUT_SHADOW Shadow_VS(VS_INPUT IN)
{
	VS_OUTPUT_SHADOW OUT;
	
	float i; // Index into matrix palette

    float4 inPos;
    inPos.xyz = IN.Position;
    inPos.w = 1.0;
    
    float3 tempPos, tempNormal;

    /////////////////////////////////////////////////////////////////////
    // FIRST BONE
    // We don't worry about the ELSE condition because we defined the 
    // initial conditions.

    // grab first bone matrix
    i = IN.Indices.x;

    // First transformed position and normal
    tempPos = mul(inPos, Bones[i]) * IN.Weights.x;
    tempNormal = mul(IN.Normal, (float3x3)Bones[i]) * IN.Weights.x;
    
    /////////////////////////////////////////////////////////////////////
    // SECOND BONE
    // Next bone.

    if(IN.Weights.y > 0.0f)
    {
        i = IN.Indices.y;

        // Add second transformed position and normal
        tempPos += mul(inPos, Bones[i]) * IN.Weights.y;
        tempNormal += mul(IN.Normal, (float3x3)Bones[i]) * IN.Weights.y;

        /////////////////////////////////////////////////////////////////////
        // THIRD BONE
        // Note we only skin the normal by the first two bones, these are by 
        // far the most significant.

        if(IN.Weights.z > 0.0f)
        {
            i = IN.Indices.z;

            // Add third transformed position only
            tempPos += mul(inPos, Bones[i]) * IN.Weights.z;

            /////////////////////////////////////////////////////////////////////
            // FOURTH BONE

            if(IN.Weights.w > 0.0f)
            {
                i = IN.Indices.w;
                
                // Add fourth transformed position only
                tempPos += mul(inPos, Bones[i]) * IN.Weights.w;
            }
        }
    }

    float4 finalPos;
    finalPos.xyz = tempPos;
    finalPos.w = 1.0;
    	
	OUT.Position = mul(finalPos, worldViewProj);
	OUT.Depth = OUT.Position;

	return OUT;
}

//-----------------------------------------------------------------------
float4 Shadow_PS(VS_OUTPUT_SHADOW IN) : COLOR
{
	float depth = IN.Depth.z;
	return float4(depth.xxx, 1.f);
}

//-----------------------------------------------------------------------
technique PaletteSkinTechnique
{
    pass P0
    {
        // Shaders
        VertexShader = compile vs_2_a main();
        PixelShader  = compile ps_2_a mainPS();  
    }
}

technique RenderShadow
{
	Pass P0
	{
		AlphaFunc = Greater;
		AlphaTestEnable = true;
		AlphaRef = 0x00000011;
		
		VertexShader = compile vs_2_a Shadow_VS();
		PixelShader = compile ps_2_a Shadow_PS();
	}
}