//--------------------------------------------------------------------------------------
//  レイヤーAI基底クラス   ( layerAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "layerAI.h"
#include "nodeAI.h"
#include "actionNodeAI.h"
#include "enemy.h"
#include "stateAnimator.h"
#include "imgui_impl_dx9.h"
#include <algorithm>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  全てのノードのデータの破棄
//--------------------------------------------------------------------------------------
void LayerAI::Release( void )
{
	std::for_each( m_childNode.begin( ) , m_childNode.end( ) ,
				   [ ]( NodeAI* nodeAI )
				   {
						nodeAI->Release( );
						nodeAI = nullptr;
				   } );

	m_childNode.clear( );
}

//--------------------------------------------------------------------------------------
//  全てのノードの実行可能状態の判断
//--------------------------------------------------------------------------------------
void LayerAI::AllJudgeActive( void )
{
	std::for_each( m_childNode.begin( ) , m_childNode.end( ) ,
				   [ ]( NodeAI* nodeAI )
				   {
						nodeAI->JudgeActive( );
				   } );
}

//--------------------------------------------------------------------------------------
//  デバッグ描画
//--------------------------------------------------------------------------------------
void LayerAI::DrawDebug( void )
{
	std::string work = m_name + "Layer";

	if( ImGui::TreeNode( work.c_str( ) ) ) 
	{
		//  名前の取得
		ImGui::InputText( "Name" , m_addNodeName , sizeof( m_addNodeName ) );

		//  Nodeの追加
		if( ImGui::Button( "AddNode" ) ) 
		{
			std::string work = m_addNodeName;

			//  同じ名前があった場合被らないように設定
			work = RenameChildNodeSameName( work );

			AddChildNode( CharacterAI::NODE_TYPE::ACTION , work , m_characterAI , this );
			strcpy( m_addNodeName , "" );
		}

		//  自身の削除
		if( ImGui::Button( "Delete" ) ) 
		{
			Release( );

			//  親レイヤーに削除を通知
			if( m_parentNode != nullptr )
			{
				m_parentNode->DeleteChildLayer( );
			}
		}

		//  所持ノード全てのデバック描画
		for( auto iterator : m_childNode )
		{
			iterator->DrawDebug( );
		}

		ImGui::TreePop( );
	}
}

//--------------------------------------------------------------------------------------
//  ノードの追加
//--------------------------------------------------------------------------------------
void LayerAI::AddChildNode( CharacterAI::NODE_TYPE type , std::string name , CharacterAI* characterAI , LayerAI* parentLayer )
{
	switch( type )
	{
		case CharacterAI::NODE_TYPE::ACTION:
		{
			m_childNode.push_back( new ActionNodeAI( name , characterAI , parentLayer , Enemy::STATE::MOVE ) );

			break;
		}
		case CharacterAI::NODE_TYPE::RULE:
		{
			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  子レイヤーの削除
//--------------------------------------------------------------------------------------
void LayerAI::DeleteChildNode( std::string name )
{
	int countChild = 0;

	//  所持ノード全ての中で名前が等しいものを削除
	for( auto iterator : m_childNode )
	{
		if( iterator->GetName( ) == name )
		{
			m_childNode.remove( iterator );

			return;
		}
	}
}

//--------------------------------------------------------------------------------------
//  同じ名前があった場合被らないように名前を直す
//--------------------------------------------------------------------------------------
const std::string LayerAI::RenameChildNodeSameName( const std::string& name )
{
	//  同じ名前カウント用
	int countSameName = 0; 

	//  名前のコピー
	std::string workName = name;

	while( 1 )
	{
		//  同じ名前があったかのフラグ
		bool samaName = false;

		//  同じ名前の数分カウント
		for( auto iterator : m_childNode )
		{
			if( iterator->GetName( ) == workName )
			{
				countSameName++;

				samaName = true;
			}
		}

		if( samaName == false )
		{
			return workName;
		}

		//  文字の追加( _値+1 )
		workName = name + "_" + std::to_string( countSameName );
	}

	return name;
}