//--------------------------------------------------------------------------------------
//  シェーダー管理クラス   ( shaderManager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "shader3D.h"
#include "shader3DNoTexture.h"
#include "shader3DNoLight.h"
#include "shader3DRimLightTexture.h"
#include "shader3DDepthShadow.h"
#include "shader3DShadowMap.h"
#include "shader3DThun.h"
#include "shader3DOutline.h"
#include "shader3DPlayer.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class Shader;

//--------------------------------------------------------------------------------------
//  シェーダー管理クラスの定義
//--------------------------------------------------------------------------------------
class ShaderManager
{
public: 
	enum class TYPE 
	{
		SHADER_3D = 0 ,
		SHADER_3D_NO_TEXTURE ,
		SHADER_3D_NO_LIGHT ,
		SHADER_3D_RIM_LIGHT_TEXTURE ,
		SHADER_3D_DEPTH_SHADOW ,
		SHADER_3D_SHADOW_MAP ,
		SHADER_3D_THUN ,
		SHADER_3D_OUTLINE ,
		SHADER_3D_PLAYER ,
		SHADER_MAX ,
	};

	ShaderManager( );										//  コンストラクタ
	~ShaderManager( );										//  デストラクタ

	static void		Init( void );							//  初期化						
	static void		Uninit( void );							//  終了

	static Shader*	GetShader( TYPE type );					//  シェーダー情報の取得
	static void		DrawEnd( void );						//  シェーダー描画終了

private:
	static Shader*	m_shader[ ( int )TYPE::SHADER_MAX ];	//  シェーダーポインタ
};

#endif
