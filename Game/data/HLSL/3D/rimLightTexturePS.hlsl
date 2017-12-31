float3	worldPosEye;				//  カメラワールド座標
float3	lightDirectWorld;			//  ライトの方向ベクトル( 単位ベクトルでワールド )
float	specularPower;				//  スペキュラーの度合い
float4	materialDiffuseColor;		//  マテリアル拡散光
sampler sampler0;					//  テクスチャ

struct IN_VERTEX
{
	float2 texcoord : TEXCOORD0;
	float3 worldNormal : TEXCOORD1;
	float3 worldPosition : TEXCOORD2;
};

float4 main( IN_VERTEX input ) : COLOR0
{
	input.worldNormal= normalize( input.worldNormal );
	
	//  カメラへのベクトル
	float3 worldToEye = normalize( worldPosEye - input.worldPosition );

	//  反射ベクトル
	float3 reflectVector = reflect( lightDirectWorld , input.worldNormal );

	//  スペキュラー( 一時保存用 )
	float workSpecular = pow( max( dot( reflectVector , worldToEye ) , 0.0f ) , specularPower );
	//float workSpecular = pow( dot( reflectVector , worldToEye ) * 0.5f + 0.5f , specularPower );

	//  拡散光
	float workDiffuse = max( dot( input.worldNormal , -lightDirectWorld ) , 0.0f );
	//float workDiffuse = dot( input.worldNormal , -lightDirectWorld  ) * 0.5f + 0.5f;

	//  スペキュラー( マテリアル×ライトカラー )
	float3 specular = workSpecular * float3( 1.0f , 1.0f , 1.0f );

	//  拡散光( マテリアル×ライトカラー )
	float3 diffuse = workDiffuse * float3( 0.25f , 0.25f , 0.25f ) * float3( materialDiffuseColor.x , materialDiffuseColor.y , materialDiffuseColor.z );

	//  環境光( マテリアル×ライトカラー )
	float3 ambient = float3( 0.1f , 0.1f , 0.1f );

	float3 emissive = float3( 0.75f , 0.75f , 0.75f );;

	//  リムライトの計算
	float lightness = 1.0f - abs( dot( worldToEye , input.worldNormal ) ) * 1.2f;
	lightness = lightness * 0.6f;
	float4 returnColor = float4( lightness , lightness , lightness , 1.0f );
	float4 color = float4( ( diffuse + ambient + specular + emissive ) , 1.0f );
	float4 textureColor = tex2D( sampler0 , input.texcoord );
	returnColor = textureColor * color + returnColor;

	//  テクスチャを使う場合はfloat4( tex2D( ) * ( diffuse + ambient + specular ) , 1.0f )に変更
	return returnColor;
}