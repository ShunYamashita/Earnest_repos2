//--------------------------------------------------------------------------------------
//  モデルレンダリング処理   ( sceneModel.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_MODEL_PARTS_H_
#define _SCENE_MODEL_PARTS_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SceneModelParts
{
public:
	SceneModelParts(  );												//  デフォルトコンストラクタ		
	~SceneModelParts( );												//  デストラクタ

	HRESULT				Init( void );									//  初期化
	void				Uninit( void );									//  終了
	void				Update( D3DXMATRIX mtxParent );					//  更新
	void				Draw( D3DXVECTOR3 position ,					//  描画
							  D3DXVECTOR3 posAt ,
							  D3DXCOLOR color );
	void				DrawDepth( D3DXVECTOR3 position ,				//  デプス値の書き込み
								   D3DXVECTOR3 posAt , 
								   D3DXCOLOR color );						

	static SceneModelParts* Create( int nIndex ,
									 int nParent ,
									 char* pFileName ,
									 D3DXVECTOR3 position ,
									 D3DXVECTOR3 rot ,
									 D3DXVECTOR3 scale );

	void				SetModelName( char* pFileName );
	void				SetIndex( int nIndex );
	int					GetIndex( void );
	void				SetParent( int nParent );
	int					GetParent( void );

	void				SetRot( D3DXVECTOR3 rot );						//  回転角の設定
	D3DXVECTOR3			GetRot( void );									//  回転角の取得

	D3DXMATRIX			GetMtxWorld( void );							//  ワールド行列の取得

	void				SetScale( D3DXVECTOR3 scale );					//  大きさの設定
	D3DXVECTOR3			GetScale( void );								//  大きさの取得

	void				SetSeekPos( D3DXVECTOR3 seekPos );				//  回転角の設定

	void				SetPositionAt( D3DXVECTOR3 posAt );				//  注視点の設定

private:
	LPD3DXMESH			m_pMeshModel;									//  メッシュ情報
	LPD3DXBUFFER		m_pBuffMatModel;								//  マテリアル情報
	DWORD				m_nNumMatModel;									//  マテリアル数
	D3DXMATRIX			m_mtxWorld;										//  ワールド行列

	D3DXVECTOR3			m_position;
	D3DXVECTOR3			m_posAt;										//  注視点の座標

	D3DXVECTOR3			m_scale;										//  大きさの倍率
	D3DXVECTOR3			m_rot;											//  回転

	D3DXVECTOR3			m_seekPos;

	int					m_nIndex;										//  識別番号
	int					m_nParent;										//  親番号

	char				m_aModelName[ 256 ];							//  テクスチャ名保存用
	char				m_aTextureName[ MAX_MATERIAL ][ 256 ];			//  テクスチャ名保存用
};

#endif

