//--------------------------------------------------------------------------------------
//  キャラクターAIクラス   ( characterAI.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _CHARACTER_AI_H_
#define _CHARACTER_AI_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <string>

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class Scene;
class RootNodeAI;

//--------------------------------------------------------------------------------------
//  キャラクターAIクラスの定義
//--------------------------------------------------------------------------------------
class CharacterAI
{
public: 
	enum class NODE_TYPE		//  ノード種類
	{
		ROOT = 0 ,				//  ルート	
		ACTION ,				//  アクション
		RULE ,					//  ルール( 条件 )
	};

	enum class LAYER_TYPE		//  ノード種類
	{
		STATEMACHIN = 0 ,		//  ステートマシン
		BEHAVIORTREE ,			//  ビヘイビアツリー
		PRIORITY  ,				//  優先度レイヤー
		SEQUENCE ,				//  流れレイヤー
		RANDOM ,				//  ランダムレイヤー
	};

	CharacterAI( );														//  コンストラクタ
	CharacterAI( std::string name ) :									//  引数付きコンストラクタ
				 m_name( name ) { }														

	void				Init( void );									//  初期化
	void				Uninit( void );									//  終了
	void				Update( void );									//  更新
	void				DrawDebug( void );								//  デバッグ描画

	Scene*				GetScene( void );								//  対象オブジェクトの取得
	bool				GetActive( void );								//  アクティブ状態かのフラグ取得
	std::string			GetName( void ){ return m_name; }				//  名前の取得
	RootNodeAI*			GetRootNodeAI( void ){ return m_rootNodeAI; }	//  ルートノードの取得

	static CharacterAI*	Create( std::string name ,						//  生成
								Scene* scene );							

private:
	std::string			m_name;											//  名前
	RootNodeAI*			m_rootNodeAI;									//  ルートノード
	Scene*				m_scene;										//  対象オブジェクト
	bool				m_active;										//  アクティブ状態かのフラグ
};

#endif
