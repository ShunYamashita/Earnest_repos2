//--------------------------------------------------------------------------------------
//  ノードAIクラス   ( nodeAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _NODE_AI_H_
#define _NODE_AI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <string>
#include <list>
#include "characterAI.h"

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class LayerAI;

//--------------------------------------------------------------------------------------
//  ノードAIクラスの定義
//--------------------------------------------------------------------------------------
class NodeAI
{
public:
	enum class STATE			//  状態
	{
		INACTIVE = 0 ,			//  非アクティブ
		SUCCESS ,				//  成功
		FAILURE ,				//  失敗
		RUNNING ,				//  実行中
		COMPLETED ,				//  完了
	};

	NodeAI( ) : m_active( true ){ }											//  コンストラクタ
	NodeAI( CharacterAI::NODE_TYPE type ,									//  引数付きコンストラクタ
			std::string name ,												
			CharacterAI* characterAI ,
			LayerAI* parentLayer ) :
			m_type( type ) ,
			m_name( name ) ,
			m_characterAI( characterAI ) ,
			m_parentLayer( parentLayer ) ,			
			m_active( true ) ,
			m_state( STATE::INACTIVE ){ }																		

	virtual void			Release( void );								//  解放
	virtual void			Run( void ) = 0;								//  実行
	virtual void			JudgeActive( void );							//  実行可能かの判断をする
	virtual void			EnableActive( void ){ m_active = true; }		//  実行可能状態に
	virtual void			DisableActive( void ){ m_active = false; }		//  実行不可能状態に
	virtual void			DrawDebug( void ) = 0;							//  デバッグ描画
	virtual void			AddChildLayer( CharacterAI::LAYER_TYPE type ,	//  レイヤーの追加
										   std::string name ,				
										   CharacterAI* characterAI ,
										   NodeAI* parentNode );
	virtual void			DeleteChildLayer( void );						//  子レイヤーの削除
	virtual bool			GetActive( void ){ return m_active; }			//  実行状態の取得
	virtual STATE			GetState( void ){ return m_state; }				//  状態の取得
	virtual std::string		GetName( void ){ return m_name; }				//  名前の取得

protected:
	std::string				m_name;											//  ノード名
	LayerAI*				m_parentLayer;									//  親レイヤー
	LayerAI*				m_childLayer;									//  子レイヤー
	CharacterAI*			m_characterAI;									//  キャラクターAI
	STATE					m_state;										//  状態
	CharacterAI::NODE_TYPE	m_type;											//  種類
	bool					m_active;										//  アクティブかのフラグ
	char					m_addLayerName[ 32 ];							//  追加レイヤー名
};

#endif
