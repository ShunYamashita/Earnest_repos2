//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[( �v���C���[�p )   ( playerPS.hlsl )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
//  �I�u�W�F�N�g�̃e�N�X�`��
sampler samplerTexture = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  �g�D�[���}�b�s���O�p�̃e�N�X�`��
sampler samplerThun = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  �@���}�b�s���O�p�̃e�N�X�`��
sampler samplerNormal = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  �X�y�L�����[�}�b�s���O�p�̃e�N�X�`��
sampler samplerSpecular = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  ���C�g�̊g�U��
float4 lightDiffuseColor;				

struct IN_VERTEX
{
	float4 positionWorld : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 toEyeT : TEXCOORD1;
	float3 lightDirectionT : TEXCOORD2;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	////  ���ʂ̐F
	//float4 returnColor;

	////  �O�̂��ߒP�ʃx�N�g����
	//input.normal = normalize( input.normal );

	////  �n�[�t�����o�[�g�g�U�v�Z�ɂ�胉�C�f�B���O�v�Z
	//float lambertLightness = max( dot( normalT , -lightDirectionLocal ) , 0.0f );
	//lambertLightness = lambertLightness * 0.5f + 0.5f;
	//lambertLightness = lambertLightness * lambertLightness;

	////==================================
	////  �g�D�[���V�F�[�_�[����
	////==================================
	////  �F�����e�N�Z����U�����Ƃ��āA�g�D�[���}�b�v�e�N�X�`�����甽�˗��̎擾
	//float4 color = tex2D( samplerThun , float2( lambertLightness , 0.0f ) );
	//float4 diffuseColor = tex2D( samplerTexture , input.texcoord );
	//float4 specularColor = tex2D( samplerSpecular , input.texcoord );

	////  �e�N�X�`���̐F���Ɗ|�����킹��
	//returnColor = color * ( diffuseColor );

	//  �P�ʃx�N�g����
	input.toEyeT = normalize( input.toEyeT );
	input.lightDirectionT = normalize( input.lightDirectionT );

	//  0.0f�`1.0f�̒l��-1.0f�`1.0f��
	float3 normalT = tex2D( samplerNormal , input.texcoord ).rgb * 2.0f - 1.0f;
	normalT = normalize( normalT );

	//  ���˃x�N�g��
	float3 reflectVector = reflect( input.lightDirectionT , normalT );

	//  �X�y�L�����[( �ꎞ�ۑ��p )
	float workSpecular = pow( max( dot( reflectVector , input.toEyeT ) , 0.0f ) , 10.0f );

	//  �g�U��
	float workDiffuse = max( dot( normalT , -input.lightDirectionT ) , 0.0f );

	//  �e�N�X�`������X�y�L�����[�J���[�̎擾
	float4 specularColor = tex2D( samplerSpecular , input.texcoord );

	//  �X�y�L�����[( �}�e���A���~���C�g�J���[ )
	float3 specular = workSpecular * float3( 1.0f , 1.0f , 1.0f );

	//  �g�U��( �}�e���A���~���C�g�J���[ )
	float3 diffuse = workDiffuse * float3( 0.7f , 0.7f , 0.7f );

	//  ����( �}�e���A���~���C�g�J���[ )
	float3 ambient = float3( 0.5f , 0.5f , 0.5f );

	float4 diffuseColor = tex2D( samplerTexture , input.texcoord );

	float4 color = float4( ( diffuse + ambient + specular ) , specularColor.r * 2.0f );

	float4 returnColor = color * diffuseColor;

	return returnColor;
}