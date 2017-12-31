float4x4	worldMatrix;					//  ワールド行列
float4x4	viewMatrix;						//  ビュー行列
float4x4	projectionMatrix;				//  プロジェクション行列

struct IN_VERTEX
{
	float3 positionLocal : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VERTEX
{
	float4 positionWorld : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	output.positionWorld = mul( float4( input.positionLocal , 1.0f ) , worldMatrix );
	output.positionWorld = mul( output.positionWorld , viewMatrix );
	output.positionWorld = mul( output.positionWorld , projectionMatrix );
	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}