//--------------------------------------------------------------------------------------
//  3Dシェーダー ( アウトライン )プログラム   ( shader3DThun.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER3D_OUTLINE_H_
#define _SHADER3D_OUTLINE_H_

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
//  3Dシェーダー ( アウトライン )クラスの定義
//--------------------------------------------------------------------------------------
class Shader3DOutline : public Shader
{
public: 
	Shader3DOutline( ){ }																		//  コンストラクタ
	~Shader3DOutline( ){ }																		//  デストラクタ

	void				Init( void ) override;													//  初期化						
	void				Uninit( void ) override;												//  終了
	void				SetVertexDeclaration( void ) override;									//  頂点フォーマットの定義
	void				SetVertexShader( void ) override;										//  頂点シェーダーの設定
	void				SetPixelShader( void ) override;										//  ピクセルシェーダーの設定

	void				SetWorldMatrix( const D3DXMATRIX &worldMatrix );						//  ワールド行列の設定
	void				SetViewMatrix( const D3DXMATRIX &viewMatrix );							//  ビュー行列の設定
	void				SetProjectionMatrix( const D3DXMATRIX &projectionMatrix );				//  プロジェクション行列の設定

	void				SetShaderInfo( const D3DXMATRIX &worldMatrix ,							//  シェーダーに必要な情報の設定
									   const D3DXMATRIX &viewMatrix ,
									   const D3DXMATRIX &projectionMatrix  );

	void				DrawBegin( void );														//  シェーダー描画開始

private:

};

#endif
