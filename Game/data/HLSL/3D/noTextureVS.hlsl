float4x4	worldMatrix;					//  ���[���h�s��
float4x4	viewMatrix;						//  �r���[�s��
float4x4	projectionMatrix;				//  �v���W�F�N�V�����s��
float3		lightDirectLocal;				//  ���C�g�̕����x�N�g��( �P�ʃx�N�g���Ń��[�J�� )
float4		lightDiffuseColor;				//  ���C�g�̃f�B�t���[�Y�J���[

struct IN_VERTEX
{
	float3 localPosition : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct OUT_VERTEX
{
	float4 positionWorld : POSITION0;
	float2 texcoord : TEXCOORD0;
	float4 color : COLOR0;
};

OUT_VERTEX main( IN_VERTEX input )
{
	OUT_VERTEX output = ( OUT_VERTEX )0;

	output.positionWorld = mul( float4( input.localPosition , 1.0f ) , worldMatrix );
	output.positionWorld = mul( output.positionWorld , viewMatrix );
	output.positionWorld = mul( output.positionWorld , projectionMatrix );
	output.texcoord = input.texcoord;

	float lightness = max( dot( -lightDirectLocal , input.normal ) , 0.0f );
	output.color.x = min( lightness * lightDiffuseColor.x + 0.2f , 1.0f );
	output.color.y = min( lightness * lightDiffuseColor.y + 0.2f , 1.0f );
	output.color.z = min( lightness * lightDiffuseColor.z + 0.2f , 1.0f );
	output.color.w = 1.0f;

	return output;
}