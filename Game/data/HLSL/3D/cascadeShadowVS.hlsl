float4x4	worldMatrix;					//  ���[���h�s��
float4x4	viewMatrix;						//  �r���[�s��
float4x4	projectionMatrix;				//  �v���W�F�N�V�����s��
float3		lightDirectLocal;				//  ���C�g�̕����x�N�g��( �P�ʃx�N�g���Ń��[�J�� )
float4x4	lightViewProjectionMatrix;		//  ���C�g���猩���r���[�v���W�F�N�V�����s��
float4x4	shadow01Matrix;					//  �V���h�E01�s��
float4x4	shadow02Matrix;					//  �V���h�E02�s��
float4x4	shadow03Matrix;					//  �V���h�E03�s��
float4x4	shadow04Matrix;					//  �V���h�E04�s��
float3		splitPosition;					//  �������W

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