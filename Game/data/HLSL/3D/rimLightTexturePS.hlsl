float3	worldPosEye;				//  �J�������[���h���W
float3	lightDirectWorld;			//  ���C�g�̕����x�N�g��( �P�ʃx�N�g���Ń��[���h )
float	specularPower;				//  �X�y�L�����[�̓x����
float4	materialDiffuseColor;		//  �}�e���A���g�U��
sampler sampler0;					//  �e�N�X�`��

struct IN_VERTEX
{
	float2 texcoord : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;
	float3 worldPosition : TEXCOORD2;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	input.worldNormal= normalize( input.worldNormal );
	
	//  �J�����ւ̃x�N�g��
	float3 worldToEye = normalize( worldPosEye - input.worldPosition );

	//  ���˃x�N�g��
	float3 reflectVector = reflect( lightDirectWorld , input.worldNormal );

	//  �X�y�L�����[( �ꎞ�ۑ��p )
	float workSpecular = pow( max( dot( reflectVector , worldToEye ) , 0.0f ) , specularPower );
	//float workSpecular = pow( dot( reflectVector , worldToEye ) * 0.5f + 0.5f , specularPower );

	//  �g�U��
	float workDiffuse = max( dot( input.worldNormal , -lightDirectWorld ) , 0.0f );
	//float workDiffuse = dot( input.worldNormal , -lightDirectWorld  ) * 0.5f + 0.5f;

	//  �X�y�L�����[( �}�e���A���~���C�g�J���[ )
	float3 specular = workSpecular * float3( 1.0f , 1.0f , 1.0f );

	//  �g�U��( �}�e���A���~���C�g�J���[ )
	float3 diffuse = workDiffuse * float3( 0.25f , 0.25f , 0.25f ) * float3( materialDiffuseColor.x , materialDiffuseColor.y , materialDiffuseColor.z );

	//  ����( �}�e���A���~���C�g�J���[ )
	float3 ambient = float3( 0.1f , 0.1f , 0.1f );

	float3 emissive = float3( 0.75f , 0.75f , 0.75f );;

	//  �������C�g�̌v�Z
	float lightness = 1.0f - abs( dot( worldToEye , input.worldNormal ) ) * 1.2f;
	lightness = lightness * 0.6f;
	float4 returnColor = float4( lightness , lightness , lightness , 1.0f );
	float4 color = float4( ( diffuse + ambient + specular + emissive ) , 1.0f );
	float4 textureColor = tex2D( sampler0 , input.texcoord );
	returnColor = textureColor * color + returnColor;

	//  �e�N�X�`�����g���ꍇ��float4( tex2D( ) * ( diffuse + ambient + specular ) , 1.0f )�ɕύX
	return returnColor;
}