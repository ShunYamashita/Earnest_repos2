float4x4	worldMatrix;					//  ���[���h�s��
float4x4	viewMatrix;						//  �r���[�s��
float4x4	projectionMatrix;				//  �v���W�F�N�V�����s��
float3		lightDirectLocal;				//  ���C�g�̕����x�N�g��( �P�ʃx�N�g���Ń��[�J�� )
float4x4	lightViewProjectionMatrix;		//  ���C�g���猩���r���[�v���W�F�N�V�����s��

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