//--------------------------------------------------------------------------------------
//  ノードAI基底クラス   ( nodeAI.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "nodeAI.h"
#include "layerAI.h"
#include "priorityLayerAI.h"
#include "sequenceLayerAI.h"
#include "randomLayerAI.h"
#include <algorithm>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ノードの破棄
//--------------------------------------------------------------------------------------
void NodeAI::Release( void )
{
	//  子レイヤーの削除
	DeleteChildLayer( );
}

//--------------------------------------------------------------------------------------
//  条件から実行可能かどうかの判断
//--------------------------------------------------------------------------------------
void NodeAI::JudgeActive( void )
{
	if( m_childLayer != nullptr )
	{
		m_childLayer->AllJudgeActive( );
	}
}

//--------------------------------------------------------------------------------------
//  レイヤーの追加
//--------------------------------------------------------------------------------------
void NodeAI::AddChildLayer( CharacterAI::LAYER_TYPE type , std::string name , CharacterAI* characterAI , NodeAI* parentNode )
{
	if( m_childLayer != nullptr )
	{
		return;
	}

	switch( type )
	{
		case CharacterAI::LAYER_TYPE::PRIORITY:
		{
			m_childLayer = new PriorityLayerAI( name , characterAI , parentNode );

			break;
		}
		case CharacterAI::LAYER_TYPE::SEQUENCE:
		{
			m_childLayer = new SequenceLayerAI( name , characterAI , parentNode );

			break;
		}
		case CharacterAI::LAYER_TYPE::RANDOM:
		{
			m_childLayer = new RandomLayerAI( name , characterAI , parentNode );

			break;
		}
	}
}

//--------------------------------------------------------------------------------------
//  レイヤーの削除
//--------------------------------------------------------------------------------------
void NodeAI::DeleteChildLayer( void )
{
	if( m_childLayer == nullptr )
	{
		return;
	}

	m_childLayer->Release( );
	delete m_childLayer;
	m_childLayer = nullptr;
}