//--------------------------------------------------------------------------------------
//  3Dシェーダー ( 深度バッファによるシャドウ )プログラム   ( shader3DDepthShadow.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_DEPTH_SHADOW_H_
#define _SHADER_3D_DEPTH_SHADOW_H_

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
//  3Dシェーダー ( 深度バッファによるシャドウ )クラスの定義
//--------------------------------------------------------------------------------------
class Shader3DDepthShadow : public Shader
{
public: 
	Shader3DDepthShadow( ){ }																//  コンストラクタ
	~Shader3DDepthShadow( ){ }																//  デストラクタ

	void	Init( void ) override;															//  初期化						
	void	Uninit( void ) override;														//  終了
	void	SetVertexDeclaration( void ) override;											//  頂点フォーマットの定義
	void	SetVertexShader( void ) override;												//  頂点シェーダーの設定
	void	SetPixelShader( void ) override;												//  ピクセルシェーダーの設定

	void	SetWorldMatrix( const D3DXMATRIX &worldMatrix );								//  ワールド行列の設定
	void	SetViewMatrix( const D3DXMATRIX &viewMatrix );									//  ビュー行列の設定
	void	SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );						//  プロジェクション行列の設定
	void	SetLightDirectionLocal( const D3DXVECTOR3 &lightDirectionLocal );				//  ライトの方向ベクトルの設定
	void	SetLightViewProjectionMatrix( const D3DXMATRIX &lightViewProjectionMatrix );	//  ライトから見たビュープロジェクション行列の設定
	void	SetOffset( const D3DXVECTOR4 &offset );											//  オフセット値
	void	SetBias( const float& bias );													//  バイアス値の設定
	void	SetFar( const float& farValue );												//  ファー値の設定
	UINT	GetSamplerTextureIndex( void );													//  テクスチャサンプラーの取得
	UINT	GetSamplerShadowIndex( void );													//  シャドウサンプラーの取得

	void	SetShaderInfo( const D3DXMATRIX &worldMatrix ,									//  シェーダーに必要な情報の設定
						   const D3DXMATRIX &viewMatrix ,
						   const D3DXMATRIX &projectionMatrix ,
						   const D3DXVECTOR3 &lightDirectionLocal ,
						   const D3DXMATRIX &lightViewProjectionMatrix ,
						   const D3DXVECTOR4 &offset ,
						   const float& bias ,
						   const float& farValue );

	void	DrawBegin( void );																//  シェーダー描画開始

private:

};

#endif
