float4x4	worldMatrix;					//  ワールド行列
float4x4	viewMatrix;						//  ビュー行列
float4x4	projectionMatrix;				//  プロジェクション行列
float4x4	worldInverseTransposeMatrix;	//  ワールド変換行列の逆行列の転置行列

struct IN_VERTEX
{
	float3 localPosition : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VERTEX
{
	float4 localPosition : POSITION0;		//  worldViewProjectionMatrixが必要
	float2 texcoord : TEXCOORD0;			//  texcoord
	float3 worldNormal : TEXCOORD1;			//  worldInverseTransposeMatrixが必要
	float3 worldPosition : TEXCOORD2;		//  mtxWorldが必要
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	output.localPosition = mul( float4( input.localPosition , 1.0f ) , worldMatrix );
	output.localPosition = mul( output.localPosition , viewMatrix );
	output.localPosition = mul( output.localPosition , projectionMatrix );
	output.texcoord = input.texcoord;
	output.worldNormal = mul( float4( input.normal , 0.0f ) , worldInverseTransposeMatrix ).xyz;
	output.worldPosition = mul( float4( input.localPosition , 1.0f ) , worldMatrix ).xyz;

	return output;
}