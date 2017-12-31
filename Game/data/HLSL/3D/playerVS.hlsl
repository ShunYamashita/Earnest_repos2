//--------------------------------------------------------------------------------------
//  頂点シェーダー( プレイヤー用 )   ( playerVS.hlsl )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
float4x4	worldMatrix;					//  ワールド行列
float4x4	viewMatrix;						//  ビュー行列
float4x4	projectionMatrix;				//  プロジェクション行列
float3		lightDirectionLocal;			//  ライトの方向ベクトル( 単位ベクトルでローカル )
float3		cameraPositionEyeLocal;			//  カメラのローカル座標

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
	float3 toEyeT : TEXCOORD1;
	float3 lightDirectionT : TEXCOORD2;
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	output.positionWorld = mul( float4( input.positionLocal , 1.0f ) , worldMatrix );
	output.positionWorld = mul( output.positionWorld , viewMatrix );
	output.positionWorld = mul( output.positionWorld , projectionMatrix );
	output.texcoord = input.texcoord;

	float3 tangent = cross( input.normal , float3( 0.0f , 1.0f , 0.0f ) );
	float3 binormal = cross( input.normal , tangent );

	//  テクスチャ空間からローカル空間に変換する行列の生成
	float3x3 matrixTBN;
	matrixTBN[ 0 ] = tangent;
	matrixTBN[ 1 ] = binormal;
	matrixTBN[ 2 ] = input.normal;

	//  ローカル空間からテクスチャ空間に変換する行列の生成
	float3x3 matrixInverseTBN = transpose( matrixTBN );

	output.lightDirectionT = mul( lightDirectionLocal , matrixInverseTBN );

	float3 toEyeL = cameraPositionEyeLocal - input.positionLocal;
	toEyeL = normalize( toEyeL );
	output.toEyeT = mul( toEyeL , matrixInverseTBN );

	return output;
}