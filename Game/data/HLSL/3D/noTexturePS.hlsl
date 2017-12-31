sampler sampler0;
float	value;

struct IN_VERTEX
{
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	float4	color = input.color;

	return color;
}