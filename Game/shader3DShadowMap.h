//--------------------------------------------------------------------------------------
//  3Dシェーダー ( シャドウマップ )プログラム   ( shader3DShadowMap.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_3D_SHADOW_MAP_H_
#define _SHADER_3D_SHADOW_MAP_H_

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
//  3Dシェーダー ( シャドウマップ )クラスの定義
//--------------------------------------------------------------------------------------
class Shader3DShadowMap : public Shader
{
public: 
	Shader3DShadowMap( ){ }														//  コンストラクタ
	~Shader3DShadowMap( ){ }													//  デストラクタ

	void	Init( void ) override;												//  初期化						
	void	Uninit( void ) override;											//  終了
	void	SetVertexDeclaration( void ) override;								//  頂点フォーマットの定義
	void	SetVertexShader( void ) override;									//  頂点シェーダーの設定
	void	SetPixelShader( void ) override;									//  ピクセルシェーダーの設定

	void	SetWorldMatrix( const D3DXMATRIX& matrix );							//  ワールド行列の設定
	void	SetLightViewProjectionMatrix( const D3DXMATRIX& matrix );			//  ライトから見たビュープロジェクション行列の設定
	void	SetFar( const float& fFar );										//  ファーの値設定

	void	SetShaderInfo( const D3DXMATRIX &worldMatrix ,						//  シェーダーに必要な情報の設定
						   const D3DXMATRIX &lightViewProjectionMatrix ,
						   const float& fFar );

	void	DrawBegin( void );													//  シェーダー描画開始

private:

};

#endif
