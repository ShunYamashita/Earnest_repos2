float4x4	worldMatrix;					//  ワールド行列
float4x4	viewMatrix;						//  ビュー行列
float4x4	projectionMatrix;				//  プロジェクション行列
float3		lightDirectLocal;				//  ライトの方向ベクトル( 単位ベクトルでローカル )
float4x4	lightViewProjectionMatrix;		//  ライトから見たビュープロジェクション行列
float4x4	shadow01Matrix;					//  シャドウ01行列
float4x4	shadow02Matrix;					//  シャドウ02行列
float4x4	shadow03Matrix;					//  シャドウ03行列
float4x4	shadow04Matrix;					//  シャドウ04行列
float3		splitPosition;					//  分割座標

struct IN_VERTEX
{
	float3	position : POSITION0;
	float3	normal : NORMAL0;
	float4	color : COLOR0;
	float2	texcoord : TEXCOORD0;
};

struct OUT_VERTEX
{
 	float4	position : POSITION;
 	float4	color : COLOR0;
 	float2	texcoord : TEXCOORD0;
 	float4	positionLight : TEXCOORD1;
	float4	shadowCoord01 : TEXCOORD2;
	float4	shadowCoord02 : TEXCOORD3;
	float4	shadowCoord03 : TEXCOORD4;
	float4	shadowCoord04 : TEXCOORD5;
	float4	splitPosition : TEXCOORD6;
	float	depth : TEXCOORD7;
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

	output.shadowCoord01 = mul( shadow01Matrix , worldPosition );
	output.shadowCoord02 = mul( shadow02Matrix , worldPosition );
	output.shadowCoord03 = mul( shadow03Matrix , worldPosition );
	output.shadowCoord04 = mul( shadow04Matrix , worldPosition );

	output.splitPosition = splitPosition;

	output.depth = output.position.w;

	return output;
}