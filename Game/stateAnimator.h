//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )  ( stateAnimator.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _STATE_ANIMATOR_H_
#define _STATE_ANIMATOR_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include "utility.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )クラスの定義
//--------------------------------------------------------------------------------------
class StateAnimator
{
public:
	typedef enum
	{
		MOTION_NEUTRAL = 0 ,										//  ニュートラル
		MOTION_MOVE ,												//  移動
		MOTION_JUMP ,												//  ジャンプ
		MOTION_BACKJUMP ,											//  後ろジャンプ
		MOTION_LANDING_SHORT ,										//  着地( 硬直短い )
		MOTION_LANDING_LONG ,										//  着地( 硬直長い )
		MOTION_ATTACK_BRAVE000 ,									//  Brave攻撃000
		MOTION_ATTACK_HP000_BEFORE ,								//  HP攻撃000( 前 )
		MOTION_ATTACK_HP000_AFTER ,									//  HP攻撃000( 後 )	
		MOTION_ATTACK_HP001 ,										//  HP攻撃
		MOTION_JUMP_ATTACK000 ,										//  ジャンプ攻撃000
		MOTION_LOCKON_DASH ,										//  ロックオンダッシュ
		MOTION_ESCAPE ,												//  回避
		MOTION_VANISH ,												//  消える
		MOTION_WIN ,												//  勝利
		MOTION_LOSE ,												//  敗北
		MOTION_RUSH_ATTACK ,										//  突進攻撃
		MOTION_MAGIC_READY ,										//  魔法構え
		MOTION_MAGIC_ACTIVE ,										//  魔法発動
		MOTION_MAGIC_ACTIVE2 ,										//  魔法発動
		MOTION_MAGIC_ACTIVE3 ,										//  魔法発動
		MOTION_GUARD ,												//  防御
		MOTION_BEND ,												//  のけぞり
		MOTION_BLOW ,												//  吹っ飛び
		MOTION_PASSIVE ,											//  受け身
		MOTION_RESULT_WIN ,											//  リザルト勝利
		MOTION_RESULT_LOSE ,										//  リザルト敗北
	} MOTION;														//  モーション

	StateAnimator( );												//  デフォルトコンストラクタ		
	~StateAnimator( );												//  デストラクタ

	bool						SetMotion( MOTION motion ,
										   bool bForce = false );	//  モーションの設定
	MOTION						GetMotion( void );					//  モーションの取得

	static StateAnimator*		Create( MOTION motion );			//  モーションの生成

private:
	MOTION						m_motion;							//  モーション
};

#endif

