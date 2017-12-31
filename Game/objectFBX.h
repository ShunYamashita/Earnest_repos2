//--------------------------------------------------------------------------------------
//  ObjectFBX   ( objectFBX.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _OBJECT_FBX_H_
#define _OBJECT_FBX_H_

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
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
//  �N���X�̑O���錾
//--------------------------------------------------------------------------------------
class FBXNode;
class FBXMesh;

//--------------------------------------------------------------------------------------
//  �����_���[�N���X�̒�`
//--------------------------------------------------------------------------------------
class ObjectFBX : public Scene
{
public: 
	////////////////////////////////////////////////////////////////////////////////////////
	//  �@ �o�C�i���[�������ꍇ�́A�|�C���^�͊�{���Ԗڂ̃f�[�^( �z�� )�����������ق�������
	//  �A ��]�̏��Ԃ��s��( XYZ�̏��Ԃŉ�]�Ȃ�)
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
 
	ObjectFBX( );																			//  �R���X�g���N�^
	~ObjectFBX( );																			//  �f�X�g���N�^

	HRESULT											Init( void );							//  ������
	void											Uninit( void );							//  �I��
	void											Update( void );							//  �X�V
	void											Draw( void );							//  �`��
	void											DrawDepth( void );						//  �f�v�X�l�̏�������

	void											SetPosition( D3DXVECTOR3 position );	//  ���W�̑��
	void											SetSize( D3DXVECTOR3 size );			//  �傫���̑��
	void											SetRot( float fRot );					//  ��]�p�̑��
	void											SetScale( float fScale );				//  �傫���{���̑��

	void											MovePos( D3DXVECTOR3 movePos );			//  ���W�̈ړ�
	void											ChangeRot( float fChangeRot );			//  ��]�p�̕ω�
	void											ChangeScale( float fChangeScale );		//  �傫���{���̕ω�

	static ObjectFBX*								Create( D3DXVECTOR3 position ,
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

	D3DXVECTOR3										m_position;						//  ���W
	D3DXVECTOR3										m_positionAt;					//  �����_
	D3DXVECTOR3										m_vectorDirect;					//  �i�s�����x�N�g��

	FbxTime											m_startTime;
	FbxTime											m_endTime;
	int												m_currentFrame;
	int												m_allTime;
	float											m_scale;
	bool											m_makeVertrx;
	std::vector< std::string >						m_textures;						//  �}�e���A��     
	std::unordered_map< std::string , std::string >	m_diffuseMap;					//  �f�B�t���[�Y�}�e���A��
	std::unordered_map< std::string , std::string >	m_specularMap;					//  �X�y�L�����[�}�e���A��
	std::unordered_map< std::string , std::string >	m_normalMap;					//  �@���}�e���A��
	std::vector< Mesh >								m_meshes; 	
	LPDIRECT3DVERTEXBUFFER9							m_pVtxBuff;						//  ���_�o�b�t�@�ւ̃|�C���^
	std::vector< LPDIRECT3DVERTEXBUFFER9 >			m_pVtxBuffers;					//
	Animator*										m_animator; 
};

#endif