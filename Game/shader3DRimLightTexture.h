//--------------------------------------------------------------------------------------
//  シェーダー3Dプログラム   ( shader3DRimLightTexture.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_RIM_LIGHT_TEXTURE_H_
#define _SHADER_3D_RIM_LIGHT_TEXTURE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "shader.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  シェーダー3Dクラスの定義
//--------------------------------------------------------------------------------------
class Shader3DRimLightTexture : public Shader
{
public: 
	Shader3DRimLightTexture( ){ }																//  コンストラクタ
	~Shader3DRimLightTexture( ){ }																//  デストラクタ

	void				Init( void ) override;													//  初期化						
	void				Uninit( void ) override;												//  終了
	void				SetVertexDeclaration( void ) override;									//  頂点フォーマットの定義
	void				SetVertexShader( void ) override;										//  頂点シェーダーの設定
	void				SetPixelShader( void ) override;										//  ピクセルシェーダーの設定

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );						//  ワールド行列の設定
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );							//  ビュー行列の設定
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );				//  プロジェクション行列の設定
	void				SetWorldInverseTransposeMatrix( const D3DXMATRIX &matrix );				//  ワールド逆行列の転置行列の設定
	void				SetWorldPositionEye( const D3DXVECTOR3 &position );						//  カメラのワールド座標の設定
	void				SetLightDirectWorld( const D3DXVECTOR3 &lightDirectWorld );				//  ライトのワールド座標系の方向ベクトルの設定
	void				SetSpecularPower( const float &specularPower );							//  スペキュラーの度合いの設定
	void				SetMaterialDiffuseColor( const D3DXCOLOR &materialDiffuseColor );		//  マテリアル拡散光の設定

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,							//  シェーダーに必要な情報の設定
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix ,
									   const D3DXMATRIX &worldInverseTransposeMatrix ,
									   const D3DXVECTOR3 &cameraPositionEye ,
									   const D3DXVECTOR3 &lightDirectWorld ,
									   const float &specularPower ,
									   const D3DXCOLOR &materialDiffuseColor  );

	void				DrawBegin( void );														//  シェーダー描画開始

private:

};

#endif
