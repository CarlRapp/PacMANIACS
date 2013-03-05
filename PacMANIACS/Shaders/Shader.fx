cbuffer PerFrame
{
	matrix View;
	matrix Projection;
	float3 CameraPos;
}

cbuffer PerObject
{
	matrix World;
	matrix WorldInverseTranspose;
	matrix WVP;
};

Texture2D Color;

SamplerState g_Sampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VSIn
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 TexCoord : TEXTURECOORD;
};

struct PSSceneIn
{
	float4 Pos			: SV_POSITION;
	float3 NormalW		: NORMAL;
	float2 TexCoord		: TEXTURECOORD;
};

//-----------------------------------------------------------------------------------------
// State Structures
//-----------------------------------------------------------------------------------------
RasterizerState NoCulling
{
	CullMode = NONE;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
PSSceneIn VSScene(VSIn input)
{
	PSSceneIn output = (PSSceneIn)0;
	
	output.Pos				= mul(float4(input.Pos,1)	, WVP);
	//output.Pos				= mul(float4(input.Pos,1)	, World);
	//output.Pos				= mul(output.Pos			, View);
	//output.Pos				= mul(output.Pos			, Projection);
	
	output.NormalW			= mul(input.Normal			,(float3x3)WorldInverseTranspose);
	
	output.TexCoord			= input.TexCoord;
	
	return output;
}

//-----------------------------------------------------------------------------------------
// PixelShader: PSSceneMain
//-----------------------------------------------------------------------------------------
float4 PSScene(PSSceneIn input) : SV_Target
{	
	input.NormalW = normalize(input.NormalW);
	float4 texColor			= Color.Sample(g_Sampler, input.TexCoord);
	return texColor;
}

DepthStencilState DepthStencil
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
};

BlendState NoBlending
{
	AlphaToCoverageEnable = FALSE;
	BlendEnable[0] = FALSE;
	BlendEnable[1] = FALSE;
};

//-----------------------------------------------------------------------------------------
// Technique: RenderTextured  
//-----------------------------------------------------------------------------------------
technique11 BasicTech
{
    pass p0
    {
		// Set VS, GS, and PS
        SetVertexShader( CompileShader( vs_4_0, VSScene() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_4_0, PSScene() ) );
		SetDepthStencilState( DepthStencil, 0 );
		SetBlendState(NoBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xffffffff);
	    SetRasterizerState( NoCulling );
    }  
}
