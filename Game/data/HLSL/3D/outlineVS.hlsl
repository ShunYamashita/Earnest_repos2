float4x4	worldMatrix;					//  ���[���h�s��
float4x4	viewMatrix;						//  �r���[�s��
float4x4	projectionMatrix;				//  �v���W�F�N�V�����s��

struct IN_VERTEX
{
	float3 positionLocal : POSITION0;
	float3 normal : NORMAL0;
};

struct OUT_VERTEX
{
	float4 positionWorld : POSITION0;
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	float3 outlinePosition = input.positionLocal + input.normal * 0.2f;

	output.positionWorld = mul( float4( outlinePosition , 1.0f ) , worldMatrix );
	output.positionWorld = mul( output.positionWorld , viewMatrix );
	output.positionWorld = mul( output.positionWorld , projectionMatrix );

	return output;
}