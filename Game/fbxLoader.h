//--------------------------------------------------------------------------------------
//  FBXLoader   ( fbxLoader.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _FBX_LOADER_H_
#define _FBX_LOADER_H_

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
//  FBXLoaderクラスの定義
//--------------------------------------------------------------------------------------
class FBXLoader
{
public: 
	enum class MOTION
	{
		INIT = 0,
		IDLE ,
		RUN ,
		JUMP ,
		LANDING ,
		ATTACK01 ,
		ATTACK02 ,
		ATTACK03 ,
		HP_ATTACK ,
		DASH_START ,
		DASH_END ,
		GROUND_GUARD_START ,
		GROUND_GUARD_END ,
		AIR_GUARD_START ,
		AIR_GUARD_END ,
		MAX ,
	};

	enum class TYPE
	{
		WINKS = 0,
		RAUDER ,
	};

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
		bool loop;
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
 
	FBXLoader( );																						//  コンストラクタ
	~FBXLoader( );																						//  デストラクタ

	HRESULT					Init( void );																//  初期化
	void					Uninit( void );																//  終了
	void					Update( void );																//  更新
	void					Draw( void );																//  描画
	void					DrawDepth( void );															//  デプス値の書き込み

	void					SetPosition( D3DXVECTOR3 position ){ m_position = position; }				//  座標の代入
	void					SetPositionAt( D3DXVECTOR3 positionAt ){ m_positionAt = positionAt; }		//  座標の代入
	void					SetScale( float scale ){ m_scale = scale; }									//  大きさ倍率の代入
	void					SetFixedPose( bool fixedPose ){ m_fixedPose = fixedPose; }					//  固定姿勢かどうかを設定
	void					SetDrawInfo( D3DXVECTOR3 position , D3DXVECTOR3 positionAt )				//  描画に必要な情報の設定
							{ 
								m_position = position;
								m_positionAt = positionAt; 
							}

	void					SetMotion( MOTION motion );													//  モーションの設定

	bool					GetMotionFinish( void ){ return m_motionFinish; }							//  モーションが終了したかの取得
	int						GetMotionFrame( void );														//  モーションフレーム取得
	float					GetMotionFrameRate( void );													//  モーション全体からフレーム割合取得

	static FBXLoader*		Create( TYPE type ,															//  生成
									D3DXVECTOR3 position ,
									D3DXVECTOR3 positionAt ,
									float scale = 1.0f );

private:
	void											AnalyzePosition( FbxMesh* pMesh );    
	void											AnalyzeNormal( FbxMesh* pMesh );     
	void											AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers = false );     
	void											AnalyzeMaterial( FbxMesh* pMesh );     
	void											AnalyzeCluster( FbxMesh* pMesh , int index );
	Animator*										AnalyzeAnimation( FbxImporter* importer , FbxScene* scene );
	void											AnalyzeNoMeshAnimation( FbxImporter* importer , FbxScene* scene , Animator* animator , bool loop );
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

	TYPE											m_type;							//  種類
	int												m_currentFrame;					//  現在のフレーム  
	int												m_allTime;						//  全体のフレーム
	int												m_motionIndex;					//  モーション番号
	MOTION											m_motion;						//  モーション
	float											m_scale;						//  大きさ
	bool											m_makeVertrx;					//  頂点バッファを作ったかのフラグ
	bool											m_motionFinish;					//  モーションが終了したのかのフラグ
	bool											m_fixedPose;					//  固定姿勢かどうかのフラグ
	std::vector< std::string >						m_textures;						//  マテリアル     
	std::unordered_map< std::string , std::string >	m_diffuseMap;					//  ディフューズマテリアル
	std::unordered_map< std::string , std::string >	m_specularMap;					//  スペキュラーマテリアル
	std::unordered_map< std::string , std::string >	m_normalMap;					//  法線マテリアル
	std::vector< Mesh >								m_meshes; 						//  メッシュ情報
	LPDIRECT3DVERTEXBUFFER9							m_pVtxBuff;						//  頂点バッファへのポインタ
	std::vector< LPDIRECT3DVERTEXBUFFER9 >			m_pVtxBuffers;					//  頂点バッファの配列
	Animator*										m_animator;						//  アニメーター
};

#endif