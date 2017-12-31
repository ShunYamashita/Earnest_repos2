sampler sampler0;

struct IN_VERTEX
{
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	float4 color = tex2D( sampler0 , input.texcoord ) * input.color;

	return color;
}