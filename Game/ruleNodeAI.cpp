//--------------------------------------------------------------------------------------
//  ルールノードAIクラス   ( ruleNodeAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "ruleNodeAI.h"
#include "layerAI.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  実行処理
//--------------------------------------------------------------------------------------
void RuleNodeAI::Run( void )
{

} 

//--------------------------------------------------------------------------------------
//  デバック描画
//--------------------------------------------------------------------------------------
void RuleNodeAI::DrawDebug( void )
{
	std::string work = m_name + "RuleNode";

	if( ImGui::TreeNode( work.c_str( ) ) ) 
	{
		//  名前の取得
		ImGui::InputText( "Name" , m_addLayerName , sizeof( m_addLayerName ) );

		if( m_childLayer == nullptr )
		{
			//  Layerの追加
			if( ImGui::Button( "AddLayer" ) ) 
			{
				std::string work = m_addLayerName;
				AddChildLayer( CharacterAI::LAYER_TYPE::PRIORITY , work , m_characterAI , this );
				strcpy( m_addLayerName , "" );
			}
		}

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

		if( m_childLayer != nullptr )
		{
			m_childLayer->DrawDebug( );
		}

		ImGui::TreePop( );
	}
}