//--------------------------------------------------------------------------------------
//  ピクセルシェーダー( プレイヤー用 )   ( playerPS.hlsl )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
//  オブジェクトのテクスチャ
sampler samplerTexture = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  トゥーンマッピング用のテクスチャ
sampler samplerThun = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  法線マッピング用のテクスチャ
sampler samplerNormal = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  スペキュラーマッピング用のテクスチャ
sampler samplerSpecular = sampler_state 
{
	MipFilter = LINEAR;
	MagFilter = LINEAR;
	MinFilter = ANISOTROPY;
	MaxAnisotropy = 8;
};

//  ライトの拡散光
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
	////  結果の色
	//float4 returnColor;

	////  念のため単位ベクトル化
	//input.normal = normalize( input.normal );

	////  ハーフランバート拡散計算によりライディング計算
	//float lambertLightness = max( dot( normalT , -lightDirectionLocal ) , 0.0f );
	//lambertLightness = lambertLightness * 0.5f + 0.5f;
	//lambertLightness = lambertLightness * lambertLightness;

	////==================================
	////  トゥーンシェーダー処理
	////==================================
	////  色情報をテクセルのU成分として、トゥーンマップテクスチャから反射率の取得
	//float4 color = tex2D( samplerThun , float2( lambertLightness , 0.0f ) );
	//float4 diffuseColor = tex2D( samplerTexture , input.texcoord );
	//float4 specularColor = tex2D( samplerSpecular , input.texcoord );

	////  テクスチャの色情報と掛け合わせる
	//returnColor = color * ( diffuseColor );

	//  単位ベクトル化
	input.toEyeT = normalize( input.toEyeT );
	input.lightDirectionT = normalize( input.lightDirectionT );

	//  0.0f～1.0fの値を-1.0f～1.0fに
	float3 normalT = tex2D( samplerNormal , input.texcoord ).rgb * 2.0f - 1.0f;
	normalT = normalize( normalT );

	//  反射ベクトル
	float3 reflectVector = reflect( input.lightDirectionT , normalT );

	//  スペキュラー( 一時保存用 )
	float workSpecular = pow( max( dot( reflectVector , input.toEyeT ) , 0.0f ) , 10.0f );

	//  拡散光
	float workDiffuse = max( dot( normalT , -input.lightDirectionT ) , 0.0f );

	//  テクスチャからスペキュラーカラーの取得
	float4 specularColor = tex2D( samplerSpecular , input.texcoord );

	//  スペキュラー( マテリアル×ライトカラー )
	float3 specular = workSpecular * float3( 1.0f , 1.0f , 1.0f );

	//  拡散光( マテリアル×ライトカラー )
	float3 diffuse = workDiffuse * float3( 0.7f , 0.7f , 0.7f );

	//  環境光( マテリアル×ライトカラー )
	float3 ambient = float3( 0.5f , 0.5f , 0.5f );

	float4 diffuseColor = tex2D( samplerTexture , input.texcoord );

	float4 color = float4( ( diffuse + ambient + specular ) , specularColor.r * 2.0f );

	float4 returnColor = color * diffuseColor;

	return returnColor;
}