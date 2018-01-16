//--------------------------------------------------------------------------------------
//  レイヤーAIクラス   ( layerAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _LAYER_AI_H_
#define _LAYER_AI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <list>
#include <string>
#include "characterAI.h"

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class NodeAI;

//--------------------------------------------------------------------------------------
//  レイヤーAIクラスの定義
//--------------------------------------------------------------------------------------
class LayerAI
{
public:
	LayerAI( ) : m_parentNode( nullptr ) { }									//  コンストラクタ
	LayerAI( CharacterAI::LAYER_TYPE type ,										//  引数付きコンストラクタ
			 std::string name ,
			 CharacterAI* characterAI ,
			 NodeAI* parentNode ) :
			 m_type( type ) ,
			 m_name( name ) ,
			 m_characterAI( characterAI ) ,
			 m_parentNode( parentNode ) { }			

	virtual void			Release( void );									//  データの破棄
	virtual void			AllJudgeActive( void );								//  全てのノードのアクティブ判断
	virtual void			DrawDebug( void );									//  デバッグ描画
	virtual	std::string		GetName( void ){ return m_name; }					//  名前の取得
	virtual void			AddChildNode( CharacterAI::NODE_TYPE type ,			//  子ノードの追加
										  std::string name ,
										  CharacterAI* characterAI ,
										  LayerAI* parentLayer );
	virtual void			DeleteChildNode( std::string name );				//  子ノードの削除

	virtual void			Run( void ) = 0;									//  実行

protected:
	const std::string		RenameChildNodeSameName( const std::string& name );	//  同じ名前があった場合被らないように名前を直す

	CharacterAI::LAYER_TYPE	m_type;												//  種類
	std::string				m_name;												//  レイヤー名
	NodeAI*					m_parentNode;										//  親ノード
	std::list< NodeAI* >	m_childNode;										//  子ノード
	CharacterAI*			m_characterAI;										//  キャラクターAI
	char					m_addNodeName[ 32 ];								//  追加ノード名
};

#endif
