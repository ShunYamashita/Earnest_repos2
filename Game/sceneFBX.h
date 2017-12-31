//--------------------------------------------------------------------------------------
//  レンダリング処理   ( sceneFBX.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _SCENE_FBX_H_
#define _SCENE_FBX_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include <fbxsdk.h>
#include <vector>
#include <list>
#include "scene.h"
#include "math.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class FBXNode;
class FBXMesh;

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include <fbxsdk.h>
#include <vector>
#include <list>
#include "scene.h"
#include "math.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  マクロ定義
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class SceneFBX : public Scene
{
public: 
	////////////////////////////////////////////////////////////////////////////////////////
	//  ① バイナリー化した場合は、ポインタは基本何番目のデータ( 配列 )を持たせたほうがいい
	//  ② 回転の順番が不明( XYZの順番で回転など)
	////////////////////////////////////////////////////////////////////////////////////////

	struct BornRefarence 
	{     
		BornRefarence(unsigned char index, float weight) : index(index), weight(weight) {}     
		unsigned char index;     
		float weight; 
	}; 
 
	struct Point 
	{     
		Point(const D3DXVECTOR3& positions) : positions(positions) {}     
		D3DXVECTOR3 positions;     
		std::vector<BornRefarence> bornRefarences; 
	}; 
 
	struct Mesh 
	{     
		std::vector<Point> points;     
		std::vector<D3DXVECTOR3> normals;      
		std::vector<D3DXVECTOR2> texcoords; 
 
		std::vector<unsigned short> positionIndices;     
		std::vector<unsigned short> normalIndices;     
		std::vector<unsigned short> texcoordIndices;          
		int materialIndex; 
 
		D3DXMATRIX initInvMatrix;
		std::vector<std::vector<D3DXMATRIX>> matrixes;
	};

	struct MATERIAL
	{
		D3DXCOLOR	Diffuse;
		D3DXCOLOR	Ambient;
		D3DXCOLOR	Specular;
		D3DXCOLOR	Emission;
		float		Shininess;
	};
 
	struct myNode 
	{     
		std::list<myNode*> children;     
		std::string name;     
		std::vector<std::string> attributeNames;     
		D3DXVECTOR3 translation;      
		D3DXVECTOR3 rotation;      
		D3DXVECTOR3 scaling;  
 
		std::vector<std::string> textures; // のマテリアル     
		std::vector<Mesh> meshes; 

		LPDIRECT3DVERTEXBUFFER9	m_pVtxBuff;								//  頂点バッファへのポインタ

		static FbxTime	m_startTime;
		static FbxTime	m_endTime;
		static int		m_currentFrame;
		static int		m_allTime;
		static bool		m_makeVertrx;
 
		static myNode* recursiveNode( FbxManager* pManager, FbxNode* pNode, bool bTexcoordVReverse,
									  FbxTime startTime, FbxTime endTime );     
		void MakeVertex( int size );

		void recursiveDraw(void);
 
	private:     
		void analyzePosition(FbxMesh* pMesh);    
		void analyzeNormal(FbxMesh* pMesh);     
		void analyzeTexcoord(FbxMesh* pMesh, bool bRevers = false);     
		void analyzeMaterial(FbxNode* pNode);     
		void analyzeMaterial(FbxMesh* pMesh);     
		void analyzeCluster(FbxMesh* pMesh);     
		FbxAMatrix GetGeometry(FbxNode* pNode);

		static std::string GetAttributeTypeName(FbxNodeAttribute::EType type);
	};
 
	SceneFBX( );																//  コンストラクタ
	~SceneFBX( );																//  デストラクタ

	HRESULT				Init( void );											//  初期化
	void				Uninit( void );											//  終了
	void				Update( void );											//  更新
	void				Draw( void );											//  描画

	void				SetPosition( D3DXVECTOR3 pos );								//  座標の代入
	void				SetSize( D3DXVECTOR3 size );								//  大きさの代入
	void				SetRot( float fRot );									//  回転角の代入
	void				SetScale( float fScale );								//  大きさ倍率の代入

	void				MovePos( D3DXVECTOR3 movePos );							//  座標の移動
	void				ChangeRot( float fChangeRot );							//  回転角の変化
	void				ChangeScale( float fChangeScale );						//  大きさ倍率の変化

private:
	myNode*				m_pTopNode;
};

#endif