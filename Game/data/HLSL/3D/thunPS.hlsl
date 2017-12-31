sampler samplerTexture;					//  オブジェクトのテクスチャ
sampler samplerThun;					//  トゥーンマッピング用のテクスチャ
float3	lightDirectionLocal;			//  ライトの方向ベクトル( 単位ベクトルでローカル )

struct IN_VERTEX
{
	float4 positionWorld : POSITION0;
	float2 texcoord : TEXCOORD0;
	float3 normal : TEXCOORD1;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	//  結果の色
	float4 returnColor;

	//  念のため単位ベクトル化
	input.normal = normalize( input.normal );

	//  ハーフランバート拡散計算によりライディング計算
	float lambertLightness = max( dot( input.normal , -lightDirectionLocal ) , 0.0f );
	lambertLightness = lambertLightness * 0.5f + 0.5f;
	lambertLightness = lambertLightness * lambertLightness;

	//==================================
	//  トゥーンシェーダー処理
	//==================================
	//  色情報をテクセルのU成分として、トゥーンマップテクスチャから反射率の取得
	float4 color = tex2D( samplerThun , float2( lambertLightness , 0.0f ) );

	//  テクスチャの色情報と掛け合わせる
	returnColor = color * tex2D( samplerTexture , input.texcoord );

	return returnColor;
}