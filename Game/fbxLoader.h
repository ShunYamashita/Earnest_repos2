//--------------------------------------------------------------------------------------
//  FBXLoader   ( fbxLoader.h )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------
#ifndef _FBX_LOADER_H_
#define _FBX_LOADER_H_

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
//  FBXLoader�N���X�̒�`
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
 
	FBXLoader( );																						//  �R���X�g���N�^
	~FBXLoader( );																						//  �f�X�g���N�^

	HRESULT					Init( void );																//  ������
	void					Uninit( void );																//  �I��
	void					Update( void );																//  �X�V
	void					Draw( void );																//  �`��
	void					DrawDepth( void );															//  �f�v�X�l�̏�������

	void					SetPosition( D3DXVECTOR3 position ){ m_position = position; }				//  ���W�̑��
	void					SetPositionAt( D3DXVECTOR3 positionAt ){ m_positionAt = positionAt; }		//  ���W�̑��
	void					SetScale( float scale ){ m_scale = scale; }									//  �傫���{���̑��
	void					SetFixedPose( bool fixedPose ){ m_fixedPose = fixedPose; }					//  �Œ�p�����ǂ�����ݒ�
	void					SetDrawInfo( D3DXVECTOR3 position , D3DXVECTOR3 positionAt )				//  �`��ɕK�v�ȏ��̐ݒ�
							{ 
								m_position = position;
								m_positionAt = positionAt; 
							}

	void					SetMotion( MOTION motion );													//  ���[�V�����̐ݒ�

	bool					GetMotionFinish( void ){ return m_motionFinish; }							//  ���[�V�������I���������̎擾
	int						GetMotionFrame( void );														//  ���[�V�����t���[���擾
	float					GetMotionFrameRate( void );													//  ���[�V�����S�̂���t���[�������擾

	static FBXLoader*		Create( TYPE type ,															//  ����
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

	D3DXVECTOR3										m_position;						//  ���W
	D3DXVECTOR3										m_positionAt;					//  �����_

	TYPE											m_type;							//  ���
	int												m_currentFrame;					//  ���݂̃t���[��  
	int												m_allTime;						//  �S�̂̃t���[��
	int												m_motionIndex;					//  ���[�V�����ԍ�
	MOTION											m_motion;						//  ���[�V����
	float											m_scale;						//  �傫��
	bool											m_makeVertrx;					//  ���_�o�b�t�@����������̃t���O
	bool											m_motionFinish;					//  ���[�V�������I�������̂��̃t���O
	bool											m_fixedPose;					//  �Œ�p�����ǂ����̃t���O
	std::vector< std::string >						m_textures;						//  �}�e���A��     
	std::unordered_map< std::string , std::string >	m_diffuseMap;					//  �f�B�t���[�Y�}�e���A��
	std::unordered_map< std::string , std::string >	m_specularMap;					//  �X�y�L�����[�}�e���A��
	std::unordered_map< std::string , std::string >	m_normalMap;					//  �@���}�e���A��
	std::vector< Mesh >								m_meshes; 						//  ���b�V�����
	LPDIRECT3DVERTEXBUFFER9							m_pVtxBuff;						//  ���_�o�b�t�@�ւ̃|�C���^
	std::vector< LPDIRECT3DVERTEXBUFFER9 >			m_pVtxBuffers;					//  ���_�o�b�t�@�̔z��
	Animator*										m_animator;						//  �A�j���[�^�[
};

#endif