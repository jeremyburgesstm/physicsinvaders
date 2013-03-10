//--------------------------------------------------------------------------------------
// TexturedUnlit.fx
//
//--------------------------------------------------------------------------------------

Texture2D tex2D;
SamplerState linearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

BlendState SrcAlphaBlendingAdd
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
    BlendOp = ADD;
    SrcBlendAlpha = ZERO;
    DestBlendAlpha = ZERO;
    BlendOpAlpha = ADD;
    RenderTargetWriteMask[0] = 0x0F;
};

matrix World;
matrix View;
matrix Projection;

struct PS_INPUT
{
	float4 Pos : SV_POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

struct VS_INPUT
{
	float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 Tex : TEXCOORD;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS( VS_INPUT input )
{
	PS_INPUT output;
	
	output.Pos = mul( input.Pos, World );
    output.Pos = mul( output.Pos, View );    
    output.Pos = mul( output.Pos, Projection );
	output.Color = input.Color;
	output.Tex = input.Tex;
	
    return output;  
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 textured( PS_INPUT input ) : SV_Target
{
    return tex2D.Sample( linearSampler, input.Tex ) * input.Color;
}

float4 noTexture( PS_INPUT input ) : SV_Target
{
    return input.Color; 
}


//--------------------------------------------------------------------------------------
// Techniques
//--------------------------------------------------------------------------------------
technique10 texturedBasic
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, textured() ) );

		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}

technique10 untexturedBasic
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, noTexture() ) );

		SetBlendState( SrcAlphaBlendingAdd, float4( 0.0f, 0.0f, 0.0f, 0.0f ), 0xFFFFFFFF );
    }
}