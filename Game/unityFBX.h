//--------------------------------------------------------------------------------------
//  UnityFBX   ( unityFBX.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _UNITY_FBX_H_
#define _UNITY_FBX_H_

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include <Windows.h>
#include <string>
#include <fbxsdk.h>
#include <vector>
#include <list>
#include <unordered_map>
#include "scene.h"
#include "math.h"
#include "main.h"

//--------------------------------------------------------------------------------------
//  クラスの前方宣言
//--------------------------------------------------------------------------------------
class FBXNode;
class FBXMesh;

//--------------------------------------------------------------------------------------
//  レンダラークラスの定義
//--------------------------------------------------------------------------------------
class UnityFBX : public Scene
{
public: 
	////////////////////////////////////////////////////////////////////////////////////////
	//  ① バイナリー化した場合は、ポインタは基本何番目のデータ( 配列 )を持たせたほうがいい
	//  ② 回転の順番が不明( XYZの順番で回転など)
	////////////////////////////////////////////////////////////////////////////////////////

	struct BornRefarence 
	{     
		BornRefarence( unsigned char index , float weight , std::string name ) : index( index ), weight( weight ), name( name ) { }     
		unsigned char index;     
		float weight; 
		std::string name;
	}; 
 
	struct Point 
	{     
		Point( const D3DXVECTOR3& positions ) : positions( positions ) { }     
		D3DXVECTOR3 positions;     
		std::vector< BornRefarence > bornRefarences; 
	}; 
 
	struct Mesh 
	{     
		std::vector< Point > points;     
		std::vector< D3DXVECTOR3 > normals;      
		std::vector< D3DXVECTOR2 > texcoords; 
 
		std::vector< unsigned short > positionIndices;     
		std::vector< unsigned short > normalIndices;     
		std::vector< unsigned short > texcoordIndices;          
		int materialIndex; 

		std::string materialName;
 
		std::vector< D3DXMATRIX > initInvMatrix;
	};

	struct Cluster
	{
		std::string name;
		D3DXMATRIX	initInvMatrix;
	};

	struct Frame
	{
		std::vector< D3DXMATRIX > matrix;
	};

	struct Motion
	{
		std::string	name;
		std::vector< Frame > frame;
	};

	struct Animator 
	{     
		std::vector< Cluster > cluster;
		std::vector< Motion > motion;
	};

	struct MATERIAL
	{
		D3DXCOLOR	Diffuse;
		D3DXCOLOR	Ambient;
		D3DXCOLOR	Specular;
		D3DXCOLOR	Emission;
		float		Shininess;
	};
 
	UnityFBX( );																			//  コンストラクタ
	~UnityFBX( );																			//  デストラクタ

	HRESULT											Init( void );							//  初期化
	void											Uninit( void );							//  終了
	void											Update( void );							//  更新
	void											Draw( void );							//  描画
	void											DrawDepth( void );						//  デプス値の書き込み

	void											SetPosition( D3DXVECTOR3 position );	//  座標の代入
	void											SetSize( D3DXVECTOR3 size );			//  大きさの代入
	void											SetRot( float fRot );					//  回転角の代入
	void											SetScale( float fScale );				//  大きさ倍率の代入

	void											MovePos( D3DXVECTOR3 movePos );			//  座標の移動
	void											ChangeRot( float fChangeRot );			//  回転角の変化
	void											ChangeScale( float fChangeScale );		//  大きさ倍率の変化

	static UnityFBX*								Create( D3DXVECTOR3 position ,
															float scale = 1.0f );

private:
	void											AnalyzePosition( FbxMesh* pMesh );    
	void											AnalyzeNormal( FbxMesh* pMesh );     
	void											AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers = false );     
	void											AnalyzeMaterial( FbxMesh* pMesh );     
	void											AnalyzeCluster( FbxMesh* pMesh , int index );
	Animator*										AnalyzeAnimation( FbxImporter* importer , FbxScene* scene );
	void											AnalyzeNoMeshAnimation( FbxImporter* importer , FbxScene* scene , Animator* animator );
	void											FindSkeleton( FbxNode* node , std::list< FbxNode* > &skeletonList );
	void											MatchBoneReferenceIndex( void );

	FbxAMatrix										GetGeometry( FbxNode* pNode );
	void											MakeVertex( int size );

	static std::string								GetAttributeTypeName( FbxNodeAttribute::EType type );

	void											recursiveDraw( void );

	std::string										m_name;     
	std::vector< std::string >						m_attributeNames;     
	D3DXVECTOR3										m_translation;      
	D3DXVECTOR3										m_rotation;      
	D3DXVECTOR3										m_scaling;  

	D3DXVECTOR3										m_position;						//  座標
	D3DXVECTOR3										m_positionAt;					//  注視点
	D3DXVECTOR3										m_vectorDirect;					//  進行方向ベクトル

	FbxTime											m_startTime;
	FbxTime											m_endTime;
	int												m_currentFrame;
	int												m_allTime;
	float											m_scale;
	bool											m_makeVertrx;
	std::vector< std::string >						m_textures;						//  マテリアル   
	std::unordered_map< std::string , std::string >	m_texturesMap;					//  マテリアル
	std::vector< Mesh >								m_meshes; 	
	LPDIRECT3DVERTEXBUFFER9							m_pVtxBuff;						//  頂点バッファへのポインタ
	std::vector< LPDIRECT3DVERTEXBUFFER9 >			m_pVtxBuffers;					//
	Animator*										m_animator; 
};

#endif