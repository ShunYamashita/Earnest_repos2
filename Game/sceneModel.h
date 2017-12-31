//--------------------------------------------------------------------------------------
//  モデルレンダリング処理   ( sceneModel.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_MODEL_H_
#define _SCENE_MODEL_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

class SceneBG;

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SceneModel : public Scene
{
public:
	typedef enum
	{
		TYPE_TREE = 0,
		TYPE_CASTLE ,
		TYPE_FIELD000 ,
		TYPE_HIT_FIELD000 ,
		TYPE_FIELD001 ,
		TYPE_HIT_FIELD001 ,
		TYPE_WALL000 ,
	} TYPE;

	SceneModel(  );														//  デフォルトコンストラクタ
	SceneModel( D3DXVECTOR3 position ,									//  コンストラクタ
				D3DXVECTOR3 rot ,
				D3DXVECTOR3 scale );				
	~SceneModel( );														//  デストラクタ

	HRESULT				Init( void );									//  初期化
	void				Uninit( void );									//  終了
	void				Update( void );									//  更新
	void				Draw( void );									//  描画
	void				DrawDepth( void );								//  デプス値の書き込み

	static SceneModel* Create( TYPE type ,								//  生成
							   D3DXVECTOR3 position ,						
							   D3DXVECTOR3 rot ,
							   D3DXVECTOR3 scale );

	static D3DXMATRIX*	GetFixedLookAtMatrix( D3DXMATRIX* pMtx ,		//  方向によっての回転行列の取得
											  D3DXVECTOR3* pPos ,
											  D3DXVECTOR3* pPosAt ,
											  D3DXVECTOR3* pVecUp );

protected:
	static bool			IntoView( D3DXVECTOR3 vec1 ,					//  視界に入ってるかの確認
								  D3DXVECTOR3 vec2 ,
								  float fViewAngle );

	void				SetScale( D3DXVECTOR3 scale );					//  大きさの設定
	D3DXVECTOR3			GetScale( void);								//  大きさの取得

	void				SetPositionAt( D3DXVECTOR3 posAt );					//  注視点の設定

	D3DXVECTOR3			m_posAt;										//  注視点の座標

	D3DXVECTOR3			m_scale;										//  大きさの倍率
	D3DXVECTOR3			m_rot;											//  回転
	D3DXQUATERNION		m_quaternion;									//  クォーターニオン

	D3DXVECTOR3			m_vecAxis;										//  軸の指定

	TYPE				m_type;											//  種類

	LPD3DXMESH			m_pMeshModel;									//  メッシュ情報

private:
	LPD3DXBUFFER		m_pBuffMatModel;								//  マテリアル情報
	DWORD				m_nNumMatModel;									//  マテリアル数

	char				m_aTextureName[ MAX_MATERIAL ][ 256 ];			//  テクスチャ名保存用
};

#endif

