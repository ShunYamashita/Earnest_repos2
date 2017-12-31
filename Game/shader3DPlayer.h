//--------------------------------------------------------------------------------------
//  3Dシェーダー ( プレイヤー用 )プログラム   ( shader3DPlayer.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER3D_PLAYER_H_
#define _SHADER3D_PLAYER_H_

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
//  3Dシェーダー ( トゥーンシェーダー )クラスの定義
//--------------------------------------------------------------------------------------
class Shader3DPlayer : public Shader
{
public: 
	Shader3DPlayer( ){ }																		//  コンストラクタ
	~Shader3DPlayer( ){ }																		//  デストラクタ

	void				Init( void ) override;													//  初期化						
	void				Uninit( void ) override;												//  終了
	void				SetVertexDeclaration( void ) override;									//  頂点フォーマットの定義
	void				SetVertexShader( void ) override;										//  頂点シェーダーの設定
	void				SetPixelShader( void ) override;										//  ピクセルシェーダーの設定

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );						//  ワールド行列の設定
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );							//  ビュー行列の設定
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );				//  プロジェクション行列の設定
	void				SetLightDirectionLocal( const D3DXVECTOR3 &lightDirectionLocal );		//  ライトの方向ベクトルの設定
	void				SetLightDiffuseColor( const D3DXCOLOR &lightDiffuseColor );				//  ライトの拡散光の設定
	void				SetCameraPositionEyeLocal( const D3DXVECTOR3 &cameraPositionEyeLocal );	//  カメラのローカル座標の設定

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,							//  シェーダーに必要な情報の設定
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix ,
									   const D3DXVECTOR3 &lightDirectionLocal ,
									   const D3DXCOLOR &lightDiffuseColor ,
									   const D3DXVECTOR3 &cameraPositionEyeLocal );

	UINT				GetSamplerTextureIndex( void );											//  テクスチャサンプラーの取得
	UINT				GetSamplerThunIndex( void );											//  トゥーンマップ用のサンプラーの取得
	UINT				GetSamplerNormalIndex( void );											//  法線用のサンプラーの取得
	UINT				GetSamplerSpecularIndex( void );										//  スペキュラー用のサンプラーの取得

	void				DrawBegin( void );														//  シェーダー描画開始

private:

};

#endif
