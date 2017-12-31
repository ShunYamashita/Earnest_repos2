//--------------------------------------------------------------------------------------
//  ブレイブゲージ   ( brave.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _BRAVE_H_
#define _BRAVE_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "main.h"
#include "scene.h"
#include "number.h"

//--------------------------------------------------------------------------------------
//  ブレイブクラスの定義
//--------------------------------------------------------------------------------------
class Brave : public Scene
{
public:
	Brave( );
	Brave( D3DXVECTOR3 position , D3DXVECTOR3 size );				//  コンストラクタ
	~Brave( );													//  デストラクタ

	HRESULT					Init( void );						//  初期化
	void					Uninit( void );						//  終了
	void					Update( void );						//  更新
	void					Draw( void );						//  描画
	void					DrawDepth( void ){ }				//  デプス値の書き込み

	static Brave*			Create( D3DXVECTOR3 position ,		//  生成				
									D3DXVECTOR3 size , 
									int nBaseBrave , 
									int nNumDigit );

	void					SetBrave( int nBrave );				//  ブレイブの設定
	void					SetEndBrave( int nBrave );			//  ブレイブの設定

private:
	std::list< Number* >	m_pNumber;							//  数字クラスのポインタ
	int						m_nNumDigit;						//  桁数
	float					m_fEndBrave;						//  目標のブレイブ
	int						m_nBrave;							//  ブレイブ
	int						m_nBaseBrave;						//  基準ブレイブ 
};

#endif
