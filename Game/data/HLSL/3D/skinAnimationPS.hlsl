sampler samplerTexture;					//  �I�u�W�F�N�g�̃e�N�X�`��
sampler samplerThun;					//  �g�D�[���}�b�s���O�p�̃e�N�X�`��
float3	lightDirectionLocal;			//  ���C�g�̕����x�N�g��( �P�ʃx�N�g���Ń��[�J�� )

struct IN_VERTEX
{
	float4 positionWorld : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	//  ���ʂ̐F
	float4 returnColor;

	//  �O�̂��ߒP�ʃx�N�g����
	input.normal = normalize( input.normal );

	//  �n�[�t�����o�[�g�g�U�v�Z�ɂ�胉�C�f�B���O�v�Z
	float lambertLightness = max( dot( input.normal , -lightDirectionLocal ) , 0.0f );
	lambertLightness = lambertLightness * 0.5f + 0.5f;
	lambertLightness = lambertLightness * lambertLightness;

	//==================================
	//  �g�D�[���V�F�[�_�[����
	//==================================
	//  �F�����e�N�Z����U�����Ƃ��āA�g�D�[���}�b�v�e�N�X�`�����甽�˗��̎擾
	float4 color = tex2D( samplerThun , float2( lambertLightness , 0.0f ) );

	//  �e�N�X�`���̐F���Ɗ|�����킹��
	returnColor = color * tex2D( samplerTexture , input.texcoord );

	return returnColor;
}