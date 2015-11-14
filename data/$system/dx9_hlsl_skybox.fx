// oi oi
//> VARS
float4x4	mViewProj;
float4x4	mInvViewProj;
float4x4	mInvView;
float4		view_position;
float		sun_alfa, sun_theta, sun_shininess, sun_strength;
texture EnvironmentMap;
//> END_VARS

float zfar = 4000;

struct VS_INPUT
{
    float3  Pos      : POSITION;
    float4  displacement : COLOR0;
};

struct VS_OUTPUT
{
    float4  Pos     : POSITION;
	float3	v	: TEXCOORD1;
	float3  sun	: TEXCOORD2;
};



samplerCUBE skySRGB = sampler_state
{  
    Texture = <EnvironmentMap>; 
    MipFilter = LINEAR; 
    MinFilter = LINEAR; 
    MagFilter = LINEAR; 
	AddressU  = CLAMP;		
    AddressV  = CLAMP;
    AddressW  = CLAMP;
    SRGBTexture = true;
};

samplerCUBE sky = sampler_state
{  
    Texture = <EnvironmentMap>; 
    MipFilter = LINEAR; 
    MinFilter = LINEAR; 
    MagFilter = LINEAR; 
	AddressU  = CLAMP;		
    AddressV  = CLAMP;
    AddressW  = CLAMP;
};

VS_OUTPUT VShader(VS_INPUT i)
{
    VS_OUTPUT   o;
    
    
	o.Pos = mul(float4(zfar*i.Pos,1), mViewProj);
	
	/*float4	direction	= float4(i.Pos.xy,1,1);
	
	direction =  mul(direction , mInvViewProj);	*/
	
	//o.v = direction.xyz/direction.w - view_position.xyz/view_position.w;
	o.v = i.Pos;
    
	o.sun.x = cos(sun_theta)*sin(sun_alfa);
	o.sun.y = sin(sun_theta);
	o.sun.z = cos(sun_theta)*cos(sun_alfa);

	return o;
}

float4 PShader(VS_OUTPUT i) : COLOR
{
    float3 sunlight = sun_strength*pow(saturate(dot(normalize(i.v), i.sun)),sun_shininess)*float3(1.2, 0.4, 0.1);
	
	float4 ut;
	ut.a = 1;
	ut.rgb = pow(texCUBE(skySRGB,i.v) + sunlight, 1/2.2);
	//if(i.v.y < 0) ut.rgb = float3(0.090,0.244,0.23);
	//ut.rgb = 0.5+0.5*i.v;
	return ut;
}

float4 PShader1_1(VS_OUTPUT i) : COLOR
{	
	float4 ut;
	ut.a = 1;
	ut.rgb = texCUBE(sky,i.v);
	return ut;
}

technique T0
{
    pass P0
    {        
        pixelshader = compile ps_2_0 PShader();
		vertexshader = compile vs_1_1 VShader(); 
    }
}

technique T1
{
    pass P0
    {        
        pixelshader = compile ps_1_1 PShader1_1();
		vertexshader = compile vs_1_1 VShader(); 
    }
}
