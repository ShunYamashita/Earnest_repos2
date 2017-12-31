sampler		samplerTexture;
sampler		samplerShadow;
float4		offset;
float		bias;

struct IN_VERTEX
{
 	float4 color : COLOR0;
 	float2 texcoord : TEXCOORD0;
 	float4 positionLight : TEXCOORD1;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	// �V���h�E�}�b�v����l�̎擾
	float2 shadowUV = 0.5f * input.positionLight.xy / input.positionLight.w + float2( 0.5f , 0.5f );
	shadowUV.y = 1.0f - shadowUV.y;
	shadowUV.x = shadowUV.x + offset.x;
	shadowUV.y = shadowUV.y + offset.y;
	float shadowValue =  tex2D( samplerShadow , shadowUV ).r;

	float4 returnColor = tex2D( samplerTexture , input.texcoord ) * input.color;

	// �������璸�_�܂ł̋������v�Z
	float depth = input.positionLight.z / input.positionLight.w;

	// �V���h�E�}�b�v�̐[�x�̒l�Ɣ�r
	if( depth - shadowValue > bias )
	{
		// �e�ɂȂ��Ă���
		returnColor.x = returnColor.x * 0.5f;
		returnColor.y = returnColor.y * 0.5f;
		returnColor.z = returnColor.z * 0.5f;
	}

	return returnColor;
}