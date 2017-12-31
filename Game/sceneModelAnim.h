//--------------------------------------------------------------------------------------
//  モデルレンダリング処理   ( sceneModelAnim.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_MODEL_ANIM_H_
#define _SCENE_MODEL_ANIM_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include "utility.h"
#include "stateAnimator.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------
#define MAX_MODEL_PARTS		( 24 )						//  モデルパーツ最大数
#define MAX_ANIMATION		( 32 )						//  アニメーション最大数

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class SceneModelParts;
class Animation;

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SceneModelAnim : public Scene
{
public:
	typedef enum
	{
		TYPE_PLAYER = 0 ,
		TYPE_ENEMY ,
		TYPE_MAX
	} TYPE;

	SceneModelAnim( int nPriority = 3 );										//  デフォルトコンストラクタ
	SceneModelAnim( D3DXVECTOR3 position ,										//  コンストラクタ
					D3DXVECTOR3 rot ,
					D3DXVECTOR3 scale );				
	~SceneModelAnim( );															//  デストラクタ

	HRESULT					Init( void );										//  初期化
	void					Uninit( void );										//  終了
	void					Update( void );										//  更新
	void					Draw( void );										//  描画
	void					DrawDepth( void );									//  デプス値の書き込み

	static SceneModelAnim*	Create( TYPE type ,									//  生成
									D3DXVECTOR3 position ,							
									D3DXVECTOR3 rot ,
									D3DXVECTOR3 scale );

	D3DXVECTOR3				GetModelPartsPos( int nIndex );						// モデルパーツ座標の取得

	Utility::HIT_SPHERE		GetHitSphere( void );								// 

	void					SetAnimation( StateAnimator::MOTION motion );		//  アニメーションの設定

	void					SetScale( D3DXVECTOR3 scale );						//  大きさの設定
	D3DXVECTOR3				GetScale( void);									//  大きさの取得

	void					SetPositionAt( D3DXVECTOR3 posAt );					//  注視点の設定

	bool					GetAnimationFinish( void );							//  アニメーションが終わっているかの確認

protected:
	D3DXVECTOR3				m_posAt;											//  注視点の座標

	D3DXVECTOR3				m_scale;											//  大きさの倍率
	D3DXVECTOR3				m_rot;												//  回転

	TYPE					m_type;												//  種類
	bool					m_bSlow;											//  スロー状態かどうか
	float					m_fVelocity;										//  速度
	float					m_fJumpPower;										//  ジャンプ力
	StateAnimator::MOTION	m_motion;											//  モーション
	int						m_nKey;												//  キー番号
	float					m_fFrame;											//  フレームカウント
	bool					m_bAnimationFinish;

	Utility::HIT_SPHERE		m_hitSphere;										//  当たり判定

private:
	SceneModelParts*		m_pModelParts[ MAX_MODEL_PARTS ];					//  モデルパーツ
	Animation*				m_pAnimation[ MAX_ANIMATION ];						//  アニメーション

	int						m_nNextAnimation;
	int						m_nNumParts;
	int						m_nShiftFrame;
	bool					m_bAnimation;
};

#endif

