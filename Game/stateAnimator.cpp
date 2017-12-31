//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )   ( stateAnimator.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "stateAnimator.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )クラスのコンストラクタ
//--------------------------------------------------------------------------------------
StateAnimator::StateAnimator( )
{
	m_motion = StateAnimator::MOTION_NEUTRAL;
}

//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )のデストラクタ
//--------------------------------------------------------------------------------------
StateAnimator::~StateAnimator( )
{

}

//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )のモーションを設定をする関数
//--------------------------------------------------------------------------------------
bool StateAnimator::SetMotion( StateAnimator::MOTION motion , bool bForce )
{
	if( bForce )
	{
		m_motion = motion;

		return true;
	}

	//  移行したいモーションの種類によっての場合分け
	switch( motion )
	{
		case StateAnimator::MOTION_NEUTRAL:
		{
			if( m_motion != StateAnimator::MOTION_NEUTRAL )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_MOVE:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_JUMP:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE || 
				m_motion == StateAnimator::MOTION_JUMP )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_BACKJUMP:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE || 
				m_motion == StateAnimator::MOTION_JUMP )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_LANDING_SHORT:
		{
			if( m_motion == StateAnimator::MOTION_JUMP ||
				m_motion == StateAnimator::MOTION_BACKJUMP || 
				m_motion == StateAnimator::MOTION_ESCAPE )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_LANDING_LONG:
		{
			if( m_motion == StateAnimator::MOTION_JUMP ||
				m_motion == StateAnimator::MOTION_BACKJUMP || 
				m_motion == StateAnimator::MOTION_ESCAPE )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_ATTACK_BRAVE000:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_ATTACK_HP000_BEFORE:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE || 
				m_motion == StateAnimator::MOTION_JUMP ||
				m_motion == StateAnimator::MOTION_LOCKON_DASH )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_ATTACK_HP000_AFTER:
		{
			if( m_motion == StateAnimator::MOTION_ATTACK_HP000_BEFORE ||
				m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE || 
				m_motion == StateAnimator::MOTION_JUMP || 
				m_motion == StateAnimator::MOTION_LOCKON_DASH )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_ATTACK_HP001:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE ||
				m_motion == StateAnimator::MOTION_JUMP )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_JUMP_ATTACK000:
		{
			if( m_motion == StateAnimator::MOTION_JUMP )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_LOCKON_DASH:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE ||
				m_motion == StateAnimator::MOTION_JUMP )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_ESCAPE:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE || 
				m_motion == StateAnimator::MOTION_JUMP ||
				m_motion == StateAnimator::MOTION_LOCKON_DASH || 
				m_motion == StateAnimator::MOTION_MAGIC_READY )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_VANISH:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_WIN:
		{
			if( m_motion != StateAnimator::MOTION_WIN )
			{
				m_motion = motion;

				return true;
			}		

			break;
		}
		case StateAnimator::MOTION_LOSE:
		{
			m_motion = motion;

			return true;

			break;
		}
		case StateAnimator::MOTION_RUSH_ATTACK:
		{
			if( m_motion == StateAnimator::MOTION_LOCKON_DASH )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_MAGIC_READY:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_MAGIC_ACTIVE:
		{
			if( m_motion == StateAnimator::MOTION_MAGIC_READY )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_MAGIC_ACTIVE2:
		{
			if( m_motion == StateAnimator::MOTION_MAGIC_READY )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_MAGIC_ACTIVE3:
		{
			if( m_motion == StateAnimator::MOTION_MAGIC_READY )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_GUARD:
		{
			if( m_motion == StateAnimator::MOTION_NEUTRAL || 
				m_motion == StateAnimator::MOTION_MOVE || 
				m_motion == StateAnimator::MOTION_JUMP ||
				m_motion == StateAnimator::MOTION_LOCKON_DASH )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_BEND:
		{
			//if( m_motion == StateAnimator::MOTION_ATTACK_HP000_AFTER ||
			//	m_motion == StateAnimator::MOTION_ATTACK_BRAVE000 )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_BLOW:
		{
			m_motion = motion;

			return true;

			break;
		}
		case StateAnimator::MOTION_PASSIVE:
		{
			if( m_motion == StateAnimator::MOTION_BLOW )
			{
				m_motion = motion;

				return true;
			}

			break;
		}
		case StateAnimator::MOTION_RESULT_WIN:
		{
			m_motion = motion;

			return true;

			break;
		}
		case StateAnimator::MOTION_RESULT_LOSE:
		{
			m_motion = motion;

			return true;

			break;
		}
	}

	return false;
}

//--------------------------------------------------------------------------------------
//  ステートマシン( アニメーター )のモーションを取得をする関数
//--------------------------------------------------------------------------------------
StateAnimator::MOTION StateAnimator::GetMotion( void )
{
	return m_motion;
}

//--------------------------------------------------------------------------------------
//  インスタンス生成をする関数
//--------------------------------------------------------------------------------------
StateAnimator* StateAnimator::Create( StateAnimator::MOTION motion )
{
	StateAnimator *pStateAnimator;

	//  インスタンス生成
	pStateAnimator = new StateAnimator;

	return pStateAnimator;
}