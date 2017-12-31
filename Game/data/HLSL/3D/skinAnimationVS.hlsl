float4x4	viewMatrix;						//  ビュー行列
float4x4	projectionMatrix;				//  プロジェクション行列

sampler		boneInfoTexture = sampler_state
{
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
}

struct IN_VERTEX
{
	float3 positionLocal : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
	float4 boneIndex : TEXCOORD1;
	float4 boneWeight : TEXCOORD2;
};

struct OUT_VERTEX
{
	float4 positionWorld : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

float4x4 GetBoneMatrix( int index )
{
	float2 texcoord;
	texcoord.x = 1.0f / 256.0f;
	texcoord.y = 1.0f / 256.0f * index;

	float4x4 returnMatrix;

	for( int countRow = 0; countRow < 4; ++countRow )
	{
		float4 row = tex2Dlod( boneInfoTexture , float4( texcoord.x * countRow , texcoord.y , 0.0f , 0.0f ) );
		returnMatrix[ countRow ] = row;
	}

	return returnMatrix;
}

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	float4x4 boneWorld = GetBoneMatrix( input.boneIndex.x ) * input.boneWeight.x + 
						 GetBoneMatrix( input.boneIndex.y ) * input.boneWeight.y +
						 GetBoneMatrix( input.boneIndex.z ) * input.boneWeight.z + 
						 GetBoneMatrix( input.boneIndex.w ) * input.boneWeight.w;

	output.positionWorld = mul( float4( input.positionLocal , 1.0f ) , boneWorld );
	output.positionWorld = mul( output.positionWorld , viewMatrix );
	output.positionWorld = mul( output.positionWorld , projectionMatrix );
	output.texcoord = input.texcoord;
	output.normal = input.normal;

	return output;
}