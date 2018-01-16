//--------------------------------------------------------------------------------------
//  アクションノードAIクラス   ( actionNodeAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "actionNodeAI.h"
#include "layerAI.h"
#include "enemyRauder.h"
#include "enemyRauderState.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  実行処理
//--------------------------------------------------------------------------------------
void ActionNodeAI::Run( void )
{
	if( m_active )
	{
		//  エネミー情報の取得
		EnemyRauder* enemy = ( EnemyRauder* )m_characterAI->GetScene( );

		if( m_state == NodeAI::STATE::RUNNING )
		{
			if( enemy->GetMotionFinish( ) )
			{
				m_state = NodeAI::STATE::COMPLETED;
			}
		}
		else 
		{
			if( m_characterAI != nullptr )
			{
				if( enemy != nullptr )
				{
					//  実行中に
					m_state = NodeAI::STATE::RUNNING;

					enemy->GetCurrentEnemyState( )->ChangeMove( EnemyRauderState::MOVE_TYPE::TARGET_CHASE );				
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  デバッグの描画
//--------------------------------------------------------------------------------------
void ActionNodeAI::DrawDebug( void )
{
	std::string work = m_name + "ActionNode";

	if( ImGui::TreeNode( work.c_str( ) ) ) 
	{
		//  自身の削除
		if( ImGui::Button( "Delete" ) ) 
		{
			Release( );

			//  親レイヤーに削除を通知
			if( m_parentLayer != nullptr )
			{
				m_parentLayer->DeleteChildNode( m_name );
			}
		}

		ImGui::TreePop( );
	}
}