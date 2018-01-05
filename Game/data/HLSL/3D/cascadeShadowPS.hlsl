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
	// シャドウマップから値の取得
	float2 shadowUV = 0.5f * input.positionLight.xy / input.positionLight.w + float2( 0.5f , 0.5f );
	shadowUV.y = 1.0f - shadowUV.y;
	shadowUV.x = shadowUV.x + offset.x;
	shadowUV.y = shadowUV.y + offset.y;
	float shadowValue =  tex2D( samplerShadow , shadowUV ).r;

	float4 returnColor = tex2D( samplerTexture , input.texcoord ) * input.color;

	// 光源から頂点までの距離を計算
	float depth = input.positionLight.z / input.positionLight.w;

	// シャドウマップの深度の値と比較
	if( depth - shadowValue > bias )
	{
		// 影になっている
		returnColor.x = returnColor.x * 0.5f;
		returnColor.y = returnColor.y * 0.5f;
		returnColor.z = returnColor.z * 0.5f;
	}

	return returnColor;
}