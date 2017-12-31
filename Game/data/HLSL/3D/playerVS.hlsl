//--------------------------------------------------------------------------------------
//  ���_�V�F�[�_�[( �v���C���[�p )   ( playerVS.hlsl )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
float4x4	worldMatrix;					//  ���[���h�s��
float4x4	viewMatrix;						//  �r���[�s��
float4x4	projectionMatrix;				//  �v���W�F�N�V�����s��
float3		lightDirectionLocal;			//  ���C�g�̕����x�N�g��( �P�ʃx�N�g���Ń��[�J�� )
float3		cameraPositionEyeLocal;			//  �J�����̃��[�J�����W

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

	//  �e�N�X�`����Ԃ��烍�[�J����Ԃɕϊ�����s��̐���
	float3x3 matrixTBN;
	matrixTBN[ 0 ] = tangent;
	matrixTBN[ 1 ] = binormal;
	matrixTBN[ 2 ] = input.normal;

	//  ���[�J����Ԃ���e�N�X�`����Ԃɕϊ�����s��̐���
	float3x3 matrixInverseTBN = transpose( matrixTBN );

	output.lightDirectionT = mul( lightDirectionLocal , matrixInverseTBN );

	float3 toEyeL = cameraPositionEyeLocal - input.positionLocal;
	toEyeL = normalize( toEyeL );
	output.toEyeT = mul( toEyeL , matrixInverseTBN );

	return output;
}