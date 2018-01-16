//--------------------------------------------------------------------------------------
//  AI管理クラス   ( AIManager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _AI_MANAGER_H_
#define _AI_MANAGER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <unordered_map>

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  前方宣言
//--------------------------------------------------------------------------------------
class CharacterAI;
class MetaAI;
class NavigationAI;
class Scene;

//--------------------------------------------------------------------------------------
//  AI管理クラスの定義
//--------------------------------------------------------------------------------------
class AIManager
{
public: 
	AIManager( ){ }																					//  コンストラクタ

	static void										Init( void );									//  初期化
	static void										Uninit( void );									//  終了
	static void										Update( void );									//  更新
	static void										DrawDebug( bool draw );							//  デバッグ描画

	static int										AddCharacterAI( std::string name ,				//  キャラクターAIの追加
																	Scene* scene );					
	static void										DeleteCharacterAI( int index );					//  キャラクターAIの削除
	static void										AllDeleteCharacterAI( void );					//  全てのキャラクターAIの削除
	static CharacterAI*								GetCharacterAI( int index );					//  キャラクターAIの取得
	static std::unordered_map< int , CharacterAI* > GetCharacterMap( );								//  キャラクターAIマップの取得

private:
	static const std::string						RenameSameName( const std::string& name );		//  同じ名前があった場合被らないように名前を直す

	static std::unordered_map< int , CharacterAI* >	m_characterAI;									//  キャラクターAI
	static MetaAI*									m_metaAI;										//  メタAI
	static NavigationAI*							m_navigationAI;									//  ナビゲーションAI
};

#endif
