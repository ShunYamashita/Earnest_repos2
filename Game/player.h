//--------------------------------------------------------------------------------------
//  プレイヤー  ( player.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "sceneModelAnim.h"
#include "effekseerManager.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class MeshTracing;
class StencilShadow;
class StateAnimator;
class PlayerUI;
class Life;
class Shadow;
class Brave;
class Magic;
class PlayerState;

//--------------------------------------------------------------------------------------
//  プレイヤークラスの定義
//--------------------------------------------------------------------------------------
class Player : public SceneModelAnim
{
public:
	enum class STATE														//  状態
	{
		NEUTRAL = 0 ,														//  ニュートラル
		MOVE ,																//  移動
		JUMP ,																//  ジャンプ
		LANDING ,															//  着地
		ATTACK_HP_ATTACK000 ,												//  Brave攻撃000
		ATTACK_BRAVE_ATTACK000_BEFORE ,										//  HP攻撃000( 前 )
		ATTACK_BRAVE_ATTACK000_AFTER ,										//  HP攻撃000( 後 )	
		LOCKON_DASH ,														//  ロックオンダッシュ
		FREE_DASH ,															//  フリーダッシュ
		ESCAPE ,															//  回避
		RUSH_ATTACK ,														//  突進攻撃
		MAGIC_READY ,														//  魔法構え
		MAGIC_ACTIVE ,														//  魔法発動
		GUARD ,																//  防御
		BEND ,																//  のけぞり
		BLOW ,																//  吹っ飛び
		PASSIVE ,															//  受け身
		RESULT_WIN ,														//  リザルト勝利
		RESULT_LOSE ,														//  リザルト敗北
		MAX ,																//  最大数
	};

	enum class MAGIC														//  魔法
	{
		FIRE = 0 ,															//  炎
		LIGHTNING ,															//  雷
		TORNADE ,															//  竜巻
	};

	Player( );																//  コンストラクタ
	~Player( );																//  デストラクタ

	HRESULT				Init( void );										//  初期化
	void				Uninit( void );										//  終了
	void				Update( void );										//  更新
	void				Draw( void );										//  描画

	static Player*		Create( D3DXVECTOR3 position ,						//  生成
								D3DXVECTOR3 rot ,
								D3DXVECTOR3 scale ,
								float fSpeed ,
								MAGIC magic , 
								int nPlayerNo = 0 );

	/*-----------------------
		設定/変更系の関数
	-----------------------*/
	void				Damage( D3DXVECTOR3 blowVecDirect ,					//  ダメージ処理
								float fBlowPower = 0.0f ,					
								int nDamage = 250 ,
								bool bBlow = true );						
	int					BraveDamage( int nDamage = 100 );					//  ブレイブダメージ処理
	void				StopAllEffekseer( void );							//  全てのエフェクシアの再生ストップ
	void				ChangeState( PlayerState* playerState );			//  状態の変化
	void				ChangeLockon( void );								//  ロックオン状態の変更
	void				SetCurrentJumpPower( float jumpPower );				//  現在のジャンプ力に値を設定
	void				SetVecDirect( D3DXVECTOR3 vecDirect );				//  方向ベクトルの設定
	void				SetPositionAt( D3DXVECTOR3 positionAt );			//  注視点の設定
	void				SetSpeed( float speed );							//  速度の設定
	void				SetMovePower( float movePower );					//  移動力の設定
	void				SetBlowPower( float blowPower );					//  吹っ飛び力の設定
	void				SetInvisibleTime( int invisibleTime );				//  無敵時間の設定
	void				SetHomingDirect( bool homingDirect );				//  追尾方向の設定
	void				SetEffekseerHandle( EffekseerManager::TYPE type ,	//  指定した種類のエフェクシア識別子の設定
											::Effekseer::Handle handle );			
	void				SetJudgeHit( bool judgeHit );						//  敵と当たり判定するかのフラグ設定
	void				SetGuardFlag( bool guardFlag );						//  防御フラグの設定
	void				SetMagic( Magic* magic );							//  魔法クラスのポインタの設定
	void				AddLife( int addLife );								//  体力の増加
	void				DeleteMagic( void );								//  魔法の削除

	/*-----------------------
		取得系の関数
	-----------------------*/
	D3DXVECTOR3			GetVecDirect( void );								//  方向ベクトルの取得
	D3DXVECTOR3			GetPositionAt( void );								//  注視点の取得
	D3DXVECTOR3			GetBlowVecDirect( void );							//  吹っ飛び方向ベクトルの取得
	int					GetPlayerNo( void );								//  プレイヤー番号の取得
	int					GetLife( void );									//  体力の取得
	int					GetInvisibleTime( void );							//  無敵時間の取得
	int					GetAnimationKey( void );							//  アニメーションキーの取得
	float				GetAnimationFrame( void );							//  アニメーションフレームの取得
	float				GetSpeed( void );									//  速度の取得
	float				GetBaseSpeed( void );								//  基本速度の取得
	float				GetJumpPower( void );								//  ジャンプ力の取得
	float				GetCurrentJumpPower( void );						//  現在のジャンプ力の取得
	float				GetTargetDistance( void );							//  目標までの距離の取得
	float				GetMovePower( void );								//  移動力の取得
	float				GetBlowPower( void );								//  吹っ飛び力の取得
	bool				GetJudgeHit( void );								//  当たり判定をするかどうかの取得
	bool				GetGuard( void );									//  防御しているかどうかの取得
	bool				GetLockon( void );									//  ロックオン情報の取得
	bool				GetHomingDirect( void );							//  追尾する向きの取得( false : 右 , true : 左 )
	Magic*				GetMagic( void );									//  魔法クラスのポインタの取得
	MAGIC				GetMagicType( void );								//  魔法の種類取得
	PlayerState*		GetPlayerState( STATE state );						//  状態の取得
	::Effekseer::Handle GetEffekseerHandle( EffekseerManager::TYPE type );	//  エフェクシア識別子の取得

	/*-----------------------
		アクション系の関数
	-----------------------*/
	void				MoveHeight( float addMove );						//  ジャンプでの移動
	void				DownCurrentJumpPower( float downPower );			//  ジャンプ力を下げる

	/*-----------------------
		当たり判定系の関数
	-----------------------*/
	bool				JudgeHitGround( void );								//  地面との当たり判定
	bool				JudgeHitEnemy( void );								//  敵との当たり判定
	bool				JudgeHitWall( void );								//  壁との当たり判定

private:
	D3DXVECTOR3			m_firstPos;											//  最初の座標
	D3DXVECTOR3			m_beforePos;										//  前回の座標
	float				m_fDistance;										//  距離
	D3DXVECTOR3			m_vecDirect;										//  進行方向
	D3DXVECTOR3			m_blowVecDirect;									//  吹っ飛び進行方向
	float				m_fSpeed;											//  速度
	float				m_fBaseSpeed;										//  基準速度
	float				m_fCurrentJumpPower;								//  現在のジャンプ力
	float				m_fRotateY;											//  回転Y軸
	float				m_fMovePower;										//  移動力
	float				m_fHomingTime;										//  追尾時間
	float				m_fBlowPower;										//  吹っ飛び力
	float				m_fMoveY;											//  下に移動する力
	int					m_nWaitTime;										//  待ち時間
	int					m_nCntJump;											//  ジャンプ回数カウント用
	int					m_nLife;											//  体力
	int					m_nBrave;											//  ブレイブ
	int					m_nInvisibleTime;									//  無敵時間
	int					m_nPlayerNo;										//  プレイヤー番号
	int					m_nPressTime;										//  プレス時間
	bool				m_bLockOn;											//  ロックオンフラグ
	bool				m_bVoiceStart;										//  開始ボイスを喋ったかどうか
	bool				m_bJudgeHit;										//  当たり判定をするかどうか
	bool				m_bHomingLeft;										//  追尾時左が入力されていたかどうか
	bool				m_bGuard;											//  防御中かどうか
	bool				m_bSlashEffekseer;									//  攻撃エフェクト
	MAGIC				m_magic;											//  魔法
	STATE				m_state;											//  状態

	Shadow*				m_pShadow;											//  影のクラスのポインタ
	Life*				m_pLife;											//  体力クラスのポインタ
	Brave*				m_pBrave;											//  ブレイブクラスのポインタ
	PlayerUI*			m_pPlayerUI;										//  プレイヤーUIクラスのポインタ
	Magic*				m_pMagic;											//  魔法クラスのポインタ
	StateAnimator*		m_pStateAnimator;									//  ステートマシン( アニメーター )クラスのポインタ
	MeshTracing*		m_pMeshTracing;										//  軌跡メッシュクラスのポインタ
	StencilShadow*		m_stencilShadow;									//  ステンシルシャドウクラスのポインタ

	PlayerState*		m_playerState;										//  プレイヤー状態クラスのポインタ

	PlayerState*		m_allState[ ( int )STATE::MAX ];					//  状態全ての保存用

	::Effekseer::Handle m_handle[ EffekseerManager::TYPE_MAX ];				//  エフェクシアハンドル

	const int			BASE_LIFE = 3000;									//  体力初期値
	const int			BASE_BRAVE = 1000;									//  ブレイブ初期値
};

#endif

