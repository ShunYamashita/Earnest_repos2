struct IN_VERTEX
{
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	float4 color = float4( 0.0f , 0.0f , 0.0f , 1.0f );

	return color;
}