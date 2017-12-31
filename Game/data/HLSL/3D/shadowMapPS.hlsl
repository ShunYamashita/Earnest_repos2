float far;

struct IN_VERTEX
{
	float4 depth : TEXCOORD0;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	//  デプス値の正規化したものを代入
	float4 returnColor = input.depth.z / input.depth.w;

	return returnColor;
}