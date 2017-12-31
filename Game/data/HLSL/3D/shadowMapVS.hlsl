float4x4	worldMatrix;					//  ワールド行列
float4x4	lightViewProjectionMatrix;		//  ライトから見たビュープロジェクション行列

struct IN_VERTEX
{
	float3 position : POSITION0;
};

struct OUT_VERTEX
{
	float4 position : POSITION0;
	float4 depth : TEXCOORD0;
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	output.position = mul( float4( input.position , 1.0f ) , worldMatrix );
	output.position = mul( output.position , lightViewProjectionMatrix );
	output.depth = output.position;

	return output;
}