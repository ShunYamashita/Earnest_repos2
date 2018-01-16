//--------------------------------------------------------------------------------------
//  AI管理クラス   ( AIManager.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "AIManager.h"
#include "characterAI.h"
#include "navigationAI.h"
#include "metaAI.h"
#include "rootNodeAI.h"
#include "imgui_impl_dx9.h"

//--------------------------------------------------------------------------------------
//  インスタンス生成
//--------------------------------------------------------------------------------------
std::unordered_map< int , CharacterAI* >	AIManager::m_characterAI;				//  キャラクターAI
MetaAI*										AIManager::m_metaAI= nullptr;			//  メタAI
NavigationAI*								AIManager::m_navigationAI = nullptr;	//  ナビゲーションAI

//--------------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------------
void AIManager::Init( void )
{
	m_characterAI.clear( );
}

//--------------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------------
void AIManager::Uninit( void )
{
	//  存在している全てのキャラクターAIの終了
	for( unsigned int countCharacterAI = 0; countCharacterAI < m_characterAI.size( ); ++countCharacterAI )
	{
		m_characterAI[ countCharacterAI ]->Uninit( );
	}

	//  空のキャラクターAIを用意して交換することで解放
	std::unordered_map< int , CharacterAI* > swap;
	m_characterAI.swap( swap );
}

//--------------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------------
void AIManager::Update( void )
{
	//  アクティブ状態である全てのキャラクターAIの更新
	for( unsigned int countCharacterAI = 0; countCharacterAI < m_characterAI.size( ); ++countCharacterAI )
	{
		if( m_characterAI[ countCharacterAI ] != nullptr )
		{       
			if( m_characterAI[ countCharacterAI ]->GetActive( ) )
			{
				m_characterAI[ countCharacterAI ]->Update( );
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  デバッグ描画
//--------------------------------------------------------------------------------------
void AIManager::DrawDebug( bool draw )
{
	ImGui::Begin( "AI Manager" , &draw );

	for( int countCharacterAI = 0; countCharacterAI < m_characterAI.size( ); ++countCharacterAI )
	{
		if( m_characterAI[ countCharacterAI ] != nullptr )
		{
			m_characterAI[ countCharacterAI ]->DrawDebug( );
		}
	}

	ImGui::End( );
}

//--------------------------------------------------------------------------------------
//  キャラクターAIの追加
//--------------------------------------------------------------------------------------
int AIManager::AddCharacterAI( std::string name , Scene* scene )
{
	//  格納場所の識別番号
	int index = 0;

	for( unsigned int countCharacterAI = 0; countCharacterAI < m_characterAI.size( ); ++countCharacterAI )
	{
		//  キー値のイテレーターが存在していない場合
		if( m_characterAI[ countCharacterAI ] == nullptr )
		{
			index = countCharacterAI;

			break;
		}
		else
		{
			index++;
		}
	}

	//  同じ名前のリネーム
	std::string workName = RenameSameName( name );

	//  情報の代入
	m_characterAI[ index ] = CharacterAI::Create( workName , scene );

	return index;
}

//--------------------------------------------------------------------------------------
//  キャラクターAIの削除
//--------------------------------------------------------------------------------------
void AIManager::DeleteCharacterAI( int index )
{
	m_characterAI.erase( index );
}

//--------------------------------------------------------------------------------------
//  全てのキャラクターAIの削除
//--------------------------------------------------------------------------------------
void AIManager::AllDeleteCharacterAI( void )
{
	m_characterAI.clear( );
}

//--------------------------------------------------------------------------------------
//  キャラクターAIの取得
//--------------------------------------------------------------------------------------
CharacterAI* AIManager::GetCharacterAI( int index )
{
	return m_characterAI[ index ];
}

//--------------------------------------------------------------------------------------
//  キャラクターAIマップの取得
//--------------------------------------------------------------------------------------
std::unordered_map< int , CharacterAI* > AIManager::GetCharacterMap( )
{
	return m_characterAI;
}

//--------------------------------------------------------------------------------------
//  同じ名前があった場合被らないように名前を直す
//--------------------------------------------------------------------------------------
const std::string AIManager::RenameSameName( const std::string& name )
{
	//  同じ名前カウント用
	int countSameName = 0; 

	//  名前のコピー
	std::string workName = name;

	while( 1 )
	{
		//  同じ名前があったかのフラグ
		bool samaName = false;

		for( unsigned int countCharacterAI = 0; countCharacterAI < m_characterAI.size( ); ++countCharacterAI )
		{
			if( m_characterAI[ countCharacterAI ]->GetName( ) == workName )
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