float4x4	worldMatrix;					//  ワールド行列
float4x4	viewMatrix;						//  ビュー行列
float4x4	projectionMatrix;				//  プロジェクション行列
float3		lightDirectLocal;				//  ライトの方向ベクトル( 単位ベクトルでローカル )
float4x4	lightViewProjectionMatrix;		//  ライトから見たビュープロジェクション行列

struct IN_VERTEX
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VERTEX
{
 	float4 position : POSITION;
 	float4 color : COLOR0;
 	float2 texcoord : TEXCOORD0;
 	float4 positionLight : TEXCOORD1;
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	float4 worldPosition;
	worldPosition = mul( float4( input.position , 1.0f ) , worldMatrix );
	output.position = mul( worldPosition , viewMatrix );
	output.position = mul( output.position , projectionMatrix );
	output.texcoord = input.texcoord;

	float lightness = max( dot( -lightDirectLocal , input.normal ) , 0.0f );
	output.color.x = min( lightness * 0.8f + 0.3f , 1.0f );
	output.color.y = min( lightness * 0.8f + 0.3f , 1.0f );
	output.color.z = min( lightness * 0.8f + 0.3f , 1.0f );
	output.color.w = 1.0f;

	output.positionLight = mul( worldPosition , lightViewProjectionMatrix );

	return output;
}