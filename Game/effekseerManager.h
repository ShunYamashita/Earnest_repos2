//--------------------------------------------------------------------------------------
//  エフェクシア管理  ( effekseerManager.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _EFFEKSEER_MANAGER_H_
#define _EFFEKSEER_MANAGER_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include <vector>
#include <map>
#include <unordered_map>

//--------------------------------------------------------------------------------------
//  エフェクシア管理クラスの定義
//--------------------------------------------------------------------------------------
class EffekseerManager
{
public:
	typedef enum
	{
		TYPE_SLASH000 = 0 ,
		TYPE_SLASH001 ,
		TYPE_SPEAR000 ,
		TYPE_SPEAR001 ,
		TYPE_VANISH000 ,
		TYPE_VANISH001 ,
		TYPE_VANISH002 ,
		TYPE_VANISH003 ,
		TYPE_VANISH004 ,
		TYPE_DRILL ,
		TYPE_LANCE ,
		TYPE_BLOOD_LANCE ,
		TYPE_HIT000 ,
		TYPE_LASER000 ,
		TYPE_LASER001 ,
		TYPE_FIRE000 ,
		TYPE_FIRE001 ,
		TYPE_FIRE002 ,
		TYPE_FLAME ,
		TYPE_BANE ,
		TYPE_WATER ,
		TYPE_LIGHTNING ,
		TYPE_METOR ,
		TYPE_PHANTASM_METOR ,
		TYPE_SANDSTORM ,
		TYPE_SNOWSTORM ,
		TYPE_SONICSTORM ,
		TYPE_TORNADE000 ,
		TYPE_TORNADE001 ,
		TYPE_MAGIC_CIRCLE ,
		TYPE_HEAL000 ,
		TYPE_HEAL001 ,
		TYPE_AURA ,
		TYPE_BOSS_DEATH ,
		TYPE_POWER_UP ,
		TYPE_ATMOSPHERE ,
		TYPE_BRAVE ,
		TYPE_SHIELD ,
		TYPE_SMASH ,
		TYPE_MAX ,
	} TYPE;

	EffekseerManager( );																		//  コンストラクタ
	~EffekseerManager( );																		//  デストラクタ

	static void						Init( void );												//  初期化
	static void						Uninit( void );												//  終了
	static void						Update( void );												//  更新
	static void						Draw( void );												//  描画

	static ::Effekseer::Handle		Create( TYPE type ,
											D3DXVECTOR3 position , 
											D3DXVECTOR3 rot , 
											D3DXVECTOR3 scale ,
											float fSpeed = 1.0f );								//  生成
	static void						Stop( ::Effekseer::Handle handle );							//  停止
	static void						StopAll( void );											//  全て停止

	static void						SetPosition( Effekseer::Handle handle , D3DXVECTOR3 position );		//  エフェクト座標の設定
	static void						SetRot( Effekseer::Handle handle , D3DXVECTOR3 rot );		//  エフェクト回転の設定
	static void						SetScale( Effekseer::Handle handle , D3DXVECTOR3 scale );	//  エフェクト大きさの設定
	static void						SetSpeed( Effekseer::Handle handle , float fSpeed );		//  エフェクト再生速度の設定
	static void						SetMatrix( int nCameraNo = 0 );								//  エフェクシア行列の設定  
	static bool						GetExist( Effekseer::Handle handle );						//  存在しているかの取得

	static std::vector<std::string> get_file_path_in_dir( const std::string& dir_name );		//  同一ディレクトリのファイル名取得

private:
	static std::map< std::string , ::Effekseer::Effect* >	m_effectMap;						//  エフェクトマップ
	static std::unordered_map< std::string , ::Effekseer::Effect* >	m_effectMap2;				//  エフェクトマップ

	static ::EffekseerRenderer::Renderer*					m_renderer;							// 描画管理インスタンス
	static ::EffekseerSound::Sound*							m_sound;							// サウンド管理インスタンス
	static ::Effekseer::Manager*							m_manager;							// エフェクト管理用インスタンス
};

#endif