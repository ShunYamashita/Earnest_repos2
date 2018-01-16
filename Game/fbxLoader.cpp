//--------------------------------------------------------------------------------------
//  FBXLoader  ( fbxLoader.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "fbxLoader.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "shaderManager.h"
#include "light.h"
#include "test.h"
#include "utility.h"
#include "keyboard.h"
#include "boneTexture.h"
#include <algorithm>
#include <functional>
#include <unordered_map>

//--------------------------------------------------------------------------------------
//  �萔��`
//--------------------------------------------------------------------------------------
static const char	THUN_SHADER_TEXTURE_NAME[ 256 ]		= "data/SHADER/thun001.png";
static const char	NONE_SPECULAR_TEXTURE_NAME[ 256 ]	= "data\\TEXTURE\\fade.png";

static const char	TEXTURE_FILEPATH[ 256 ]				= "data\\FBX\\texture";

//--------------------------------------------------------------------------------------
//  �R���X�g���N�^
//--------------------------------------------------------------------------------------
FBXLoader::FBXLoader( )
{
	m_currentFrame = 0;
	m_allTime = 0;
	m_scale = 1.0f;
	m_translation = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
	m_motionIndex = 0;
	m_motionFinish = false;
	m_fixedPose = true;
}

//--------------------------------------------------------------------------------------
//  �f�X�X�g���N�^
//--------------------------------------------------------------------------------------
FBXLoader::~FBXLoader( )
{

}

//--------------------------------------------------------------------------------------
//  ����������
//--------------------------------------------------------------------------------------
HRESULT FBXLoader::Init( void )
{
	//  FBX���f���ǂݍ��ݎ����p�v���O����
	FbxManager* lSdkManager = FbxManager::Create( ); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create( lSdkManager , IOSROOT ); lSdkManager->SetIOSettings( ios ); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create( lSdkManager ,"" ); 
 
	if( m_type == FBXLoader::TYPE::WINKS )
	{
		// Use the first argument as the filename for the importer. 
		if( !lImporter->Initialize( "data/FBX/winks/winks.fbx" , -1, lSdkManager->GetIOSettings( ) ) ) 
		{      
			char buf[ 256 ];     
			sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
			MessageBox( NULL , buf , "error" , MB_OK );     
			return E_FAIL; 
		}  
	}
	else if( m_type == FBXLoader::TYPE::RAUDER )
	{
		// Use the first argument as the filename for the importer. 
		if( !lImporter->Initialize( "data/FBX/rauder/rauder.fbx", -1, lSdkManager->GetIOSettings( ) ) ) 
		{      
			char buf[ 256 ];     
			sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
			MessageBox( NULL , buf , "error" , MB_OK );     
			return E_FAIL; 
		}  
	}
 
	// Create a new scene so that it can be populated by the imported file. 
	FbxScene* lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import( lScene ); 
 
	FbxGeometryConverter converter( lSdkManager ); 
	
	// �O�p�|���S���� 
	converter.Triangulate( lScene , true ); 
	
	// �k�ރ��b�V�����폜 
	converter.RemoveBadPolygonsFromMeshes( lScene ); 
	
	// �}�e���A�����ƂɃ��b�V������ 
	converter.SplitMeshesPerMaterial( lScene , true ); 

	//  �A�j���[�V�������̎擾
	lImporter->GetAnimStackCount( );

	//  ���b�V�����̎擾
	int numberMesh = lScene->GetMemberCount< FbxMesh >( );

	//  �}�e���A�����̎擾
	int numberMaterial = lScene->GetMaterialCount( );

	//  ���b�V���̐����̃��[�v
	for( int countMesh = 0; countMesh < numberMesh; ++countMesh )
	{
		m_meshes.push_back( Mesh( ) );

		//  �w�胁�b�V���̏��̎擾
		FbxMesh* mesh = lScene->GetSrcObject< FbxMesh >( countMesh );

		//  ���W�̎擾
		AnalyzePosition( mesh );

		//  �@�����                 
		AnalyzeNormal( mesh );                 
				
		//  UV���                 
		AnalyzeTexcoord( mesh , true ); 

		//  �}�e���A����́i�Q�Ə��̎擾�j                 
		AnalyzeMaterial( mesh ); 

		//  �{�[�����
		AnalyzeCluster( mesh , countMesh );

		//  �C���f�b�N�X
		FbxLayerElementArrayTemplate< int >* index;

		//  
		if( mesh->GetMaterialIndices( &index ) && index->GetCount( ) > 0 )
		{
			// �}�e���A�����̎擾     
			FbxSurfaceMaterial* material = mesh->GetNode( )->GetMaterial( index->GetAt( 0 ) ); 

			std::string work = material->GetName( );

			//  �}�e���A�����̕ۑ�
			m_meshes.back( ).materialName = material->GetName( ); 
		}
	}

	//  �}�e���A���̐����̃��[�v
	for( int countMaterial = 0; countMaterial < numberMaterial; ++countMaterial )
	{
		// �}�e���A�����̎擾     
		FbxSurfaceMaterial* pMaterial = lScene->GetMaterial( countMaterial ); 

		//  �}�e���A�����̎擾
		std::string materialName = pMaterial->GetName( );

		// �f�B�t���[�Y���̎擾     
		FbxProperty diffuseProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sDiffuse ); 
 
		// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N     
		int layeredTextureCount = diffuseProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// ���C���[�h�e�N�X�`����������Βʏ�e�N�X�`��     
		if ( layeredTextureCount == 0 ) 
		{ 
			// �ʏ�e�N�X�`���̖������`�F�b�N         
			int textureCount = diffuseProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// �e�e�N�X�`���ɂ��ăe�N�X�`�������Q�b�g         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i�Ԗڂ̃e�N�X�`���I�u�W�F�N�g�擾         
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject< FbxFileTexture >( 0 ); 

				// �e�N�X�`���t�@�C�����̎擾    
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet���̎擾         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				int layer = relFileName.rfind( "\\" );
				relFileName.erase( 0 , layer );
 
				// �����o�^         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = TEXTURE_FILEPATH + relFileName;    

				//  �e�N�X�`���N���X�̎擾
				Texture* pTexture2 = SceneManager::GetTexture( );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�         
				m_textures.push_back( strPathName ); 

				//  �e�N�X�`���}�b�v�Ƀ}�e���A�����ƃe�N�X�`�����̑��
				m_diffuseMap.emplace( materialName , strPathName );
			}         
		}     
		else 
		{ 
			// ���C���[�h�e�N�X�`������         
			MessageBox( NULL , "���C���[�h�e�N�X�`��" , "�}�e���A���̎擾" , MB_OK );     
		}    

		// �f�B�t���[�Y���̎擾     
		FbxProperty specularProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sSpecular ); 

		// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N     
		int layeredSpecularCount = specularProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// ���C���[�h�e�N�X�`����������Βʏ�e�N�X�`��     
		if ( layeredSpecularCount == 0 ) 
		{ 
			// �ʏ�e�N�X�`���̖������`�F�b�N         
			int textureCount = specularProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// �e�e�N�X�`���ɂ��ăe�N�X�`�������Q�b�g         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i�Ԗڂ̃e�N�X�`���I�u�W�F�N�g�擾         
				FbxFileTexture* pTexture = specularProperty.GetSrcObject< FbxFileTexture >( 0 ); 

				// �e�N�X�`���t�@�C�����̎擾       
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet���̎擾         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				int layer = relFileName.rfind( "\\" );
				relFileName.erase( 0 , layer );
 
				// �����o�^         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = TEXTURE_FILEPATH + relFileName;    

				//  �e�N�X�`���N���X�̎擾
				Texture* pTexture2 = SceneManager::GetTexture( );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�         
				m_textures.push_back( strPathName ); 

				//  �e�N�X�`���}�b�v�Ƀ}�e���A�����ƃe�N�X�`�����̑��
				m_specularMap.emplace( materialName , strPathName );
			}         
		}     
		else 
		{ 
			// ���C���[�h�e�N�X�`������         
			MessageBox( NULL , "���C���[�h�e�N�X�`��" , "�}�e���A���̎擾" , MB_OK );     
		}   

		// �f�B�t���[�Y���̎擾     
		FbxProperty normalProperty = pMaterial->FindProperty( FbxSurfaceMaterial::sNormalMap ); 

		// �v���p�e�B�������Ă��郌�C���[�h�e�N�X�`���̖������`�F�b�N     
		int layeredNormalCount = normalProperty.GetSrcObjectCount< FbxLayeredTexture >( ); 
 
		// ���C���[�h�e�N�X�`����������Βʏ�e�N�X�`��     
		if ( layeredNormalCount == 0 ) 
		{ 
			// �ʏ�e�N�X�`���̖������`�F�b�N         
			int textureCount = normalProperty.GetSrcObjectCount< FbxFileTexture >( ); 
   
			// �e�e�N�X�`���ɂ��ăe�N�X�`�������Q�b�g         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i�Ԗڂ̃e�N�X�`���I�u�W�F�N�g�擾         
				FbxFileTexture* pTexture = normalProperty.GetSrcObject< FbxFileTexture >( 0 ); 

				// �e�N�X�`���t�@�C�����̎擾              
				std::string relFileName = pTexture->GetRelativeFileName( ); 
 
				// UVSet���̎擾         
				std::string uvSetName = pTexture->UVSet.Get().Buffer( ); 

				int layer = relFileName.rfind( "\\" );
				relFileName.erase( 0 , layer );
 
				// �����o�^         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = TEXTURE_FILEPATH + relFileName;    

				//  �e�N�X�`���N���X�̎擾
				Texture* pTexture2 = SceneManager::GetTexture( );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// �e�N�X�`���̓ǂݍ��݂ƊǗ�         
				m_textures.push_back( strPathName ); 

				//  �e�N�X�`���}�b�v�Ƀ}�e���A�����ƃe�N�X�`�����̑��
				m_normalMap.emplace( materialName , strPathName );
			}         
		}     
		else 
		{ 
			// ���C���[�h�e�N�X�`������         
			MessageBox( NULL , "���C���[�h�e�N�X�`��" , "�}�e���A���̎擾" , MB_OK );     
		}   
	}

	//  �A�j���[�V�������擾
	Animator* animator = AnalyzeAnimation( lImporter , lScene );

	//  ���b�V���̐���
	for( auto itm = m_meshes.begin( ); itm != m_meshes.end( ); ++itm ) 
	{  
		m_pVtxBuff = nullptr;
		m_pVtxBuffers.push_back( m_pVtxBuff );

		//  ������( �܂胏���X�L���Ȃ� )
		MakeVertex( itm->positionIndices.size( ) );
	}

	//  �e�N�X�`���N���X�̎擾
	Texture* texture = SceneManager::GetTexture( );
	texture->SetTextureImage( NONE_SPECULAR_TEXTURE_NAME );

	//  �g�D�[���V�F�[�_�[�p�̃e�N�X�`���擾
	texture->SetTextureImage( THUN_SHADER_TEXTURE_NAME );

	char winksFileName[ ( int )MOTION::MAX - 1 ][ 256 ] = {
																"data/FBX/winks/Animations/idle.fbx" ,
																"data/FBX/winks/Animations/run.fbx" ,
																"data/FBX/winks/Animations/jump.fbx" ,
																"data/FBX/winks/Animations/landing.fbx" ,
																"data/FBX/winks/Animations/attack01.fbx" ,
																"data/FBX/winks/Animations/attack02.fbx" ,
																"data/FBX/winks/Animations/attack03.fbx" ,
																"data/FBX/winks/Animations/hp_attack.fbx" ,
																"data/FBX/winks/Animations/dash_start.fbx" ,
																"data/FBX/winks/Animations/dash_end.fbx" ,
																"data/FBX/winks/Animations/ground_guard_start.fbx" ,
																"data/FBX/winks/Animations/ground_guard_end.fbx" ,
																"data/FBX/winks/Animations/air_guard_start.fbx" ,
																"data/FBX/winks/Animations/air_guard_end.fbx" ,
														  };

	bool winksMotionLoop[ ( int )MOTION::MAX - 1 ] = {
														true ,
														true ,
														false ,
														false ,
														false ,
														false ,
														false ,
														false ,
														false ,
														false , 
														false ,
														false ,
														false ,
														false ,
													 };

	char rauderFileName[ ( int )MOTION::MAX - 1  ][ 256 ] = {
																"data/FBX/rauder/Animations/idle.fbx" ,
																"data/FBX/rauder/Animations/run.fbx" ,
																"data/FBX/rauder/Animations/jump.fbx" ,
																"data/FBX/rauder/Animations/landing.fbx" ,
																"data/FBX/rauder/Animations/attack01.fbx" ,
																"data/FBX/rauder/Animations/attack02.fbx" ,
																"data/FBX/rauder/Animations/attack03.fbx" ,
																"data/FBX/rauder/Animations/ground_hp_attack.fbx" ,
																"data/FBX/rauder/Animations/step_front.fbx" ,
																"data/FBX/rauder/Animations/step_back.fbx" ,
																"data/FBX/rauder/Animations/ground_guard_start.fbx" ,
																"data/FBX/rauder/Animations/ground_guard_end.fbx" ,
																"data/FBX/rauder/Animations/air_guard_start.fbx" ,
																"data/FBX/rauder/Animations/air_guard_end.fbx" ,
															};

	bool rauderMotionLoop[ ( int )MOTION::MAX - 1 ] = {
															true ,
															true ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
															false ,
													  };

	if( m_type == FBXLoader::TYPE::WINKS )
	{
		for( int countAnimation = 0; countAnimation < ( int )MOTION::MAX - 1; countAnimation++ )
		{
			// Use the first argument as the filename for the importer. 
			if( !lImporter->Initialize( winksFileName[ countAnimation ] , -1 , lSdkManager->GetIOSettings( ) ) ) 
			{      
				char buf[ 256 ];     
				sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
				MessageBox( NULL , buf , "error" , MB_OK );     
				return E_FAIL; 
			}  
 
			// Create a new scene so that it can be populated by the imported file. 
			lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
			// Import the contents of the file into the scene. 
			lImporter->Import( lScene ); 

			//  ���b�V�����Ȃ��A�j���[�V�����̃f�[�^�擾
			AnalyzeNoMeshAnimation( lImporter , lScene , animator , winksMotionLoop[ countAnimation ] );
		}
	}
	else if( m_type == FBXLoader::TYPE::RAUDER )
	{
		for( int countAnimation = 0; countAnimation < ( int )MOTION::MAX - 1; countAnimation++ )
		{
			// Use the first argument as the filename for the importer. 
			if( !lImporter->Initialize( rauderFileName[ countAnimation ] , -1 , lSdkManager->GetIOSettings( ) ) ) 
			{      
				char buf[ 256 ];     
				sprintf( buf , "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" , lImporter->GetStatus( ).GetErrorString( ) );     
				MessageBox( NULL , buf , "error" , MB_OK );     
				return E_FAIL; 
			}  
 
			// Create a new scene so that it can be populated by the imported file. 
			lScene = FbxScene::Create( lSdkManager , "myScene" ); 
 
			// Import the contents of the file into the scene. 
			lImporter->Import( lScene ); 

			//  ���b�V�����Ȃ��A�j���[�V�����̃f�[�^�擾
			AnalyzeNoMeshAnimation( lImporter , lScene , animator , winksMotionLoop[ countAnimation ] );
		}
	}

	//  �A�j���[�V�����f�[�^�̕ۑ�
	m_animator = animator;

	//  �{�[���̃C���f�b�N�X�ԍ��̊��蓖��
	MatchBoneReferenceIndex( );

	lImporter->Destroy( ); 
	lScene->Destroy( ); 
	lSdkManager->Destroy( ); 

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �I������
//--------------------------------------------------------------------------------------
void FBXLoader::Uninit( void )
{

}

//--------------------------------------------------------------------------------------
//  �X�V����
//--------------------------------------------------------------------------------------
void FBXLoader::Update( void )
{
	if( m_animator->motion[ m_motionIndex ].loop )
	{
		m_currentFrame += 1;
	}
	else
	{
		if( m_currentFrame < m_animator->motion[ m_motionIndex ].frame.size( ) - 1 )
		{
			m_currentFrame += 1;
		}
		else
		{
			m_motionFinish = true;
		}
	}
}

//--------------------------------------------------------------------------------------
//  �`�揈��
//--------------------------------------------------------------------------------------
void FBXLoader::Draw( void )
{  
	//  �ċA�I�ɕ`��
    recursiveDraw( ); 
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɕ`�揈��
//--------------------------------------------------------------------------------------
void FBXLoader::recursiveDraw( void )
{ 
	D3DXMATRIX mtxWorld;							//  ���[���h�s��
	D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
	D3DXMATRIX mtxScale;							//  �g��s��
	D3DXMATRIX mtxRot;								//  ��]���W�ϊ��s��

	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �V�F�[�_�[���̎擾
	Shader3D* shader3D = ( Shader3D* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D );

	//  �V�F�[�_�[���̎擾
	Shader3DRimLightTexture* shader3DRimLightTexture = ( Shader3DRimLightTexture* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_RIM_LIGHT_TEXTURE );

	//  �V�F�[�_�[���̎擾
	Shader3DPlayer* shader3DPlayer = ( Shader3DPlayer* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_PLAYER );

	//  �V�F�[�_�[���̎擾
	Shader3DOutline* shader3DOutline = ( Shader3DOutline* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_OUTLINE );

	//  �����_�[�X�e�[�g�̐ݒ�
    pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );				//  ���ʕ`��

	for( int countMesh = 0; countMesh != m_meshes.size( ); ++countMesh ) 
	{  
		if( m_meshes[ countMesh ].materialName == "hair" || 
			m_meshes[ countMesh ].materialName == "eye" || 
			m_meshes[ countMesh ].materialName == "eyelashes" )
		{
			continue;
		}

		// ���_�̍��W�ϊ�             
		std::vector< D3DXVECTOR3 > positions;
		int size = m_meshes[ countMesh ].points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( unsigned int countPoint = 0; countPoint < m_meshes[ countMesh ].points.size( ); ++countPoint ) 
		{
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ m_motionIndex ].frame[ m_currentFrame % m_animator->motion[ m_motionIndex ].frame.size( ) ].matrix[ m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].index ] * m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].weight;
			}

			if( m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = m_meshes[ countMesh ].points[ countPoint ].positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

		if( m_pVtxBuffers.at( countMesh ) != NULL )
		{
			//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
			m_pVtxBuffers.at( countMesh )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
													( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
													0 );							//  ���b�N�̎��

			//  ���]�p�̃��[�v�J�E���g
			int countLoop = 0;

			for( int i = m_meshes[ countMesh ].positionIndices.size( ) - 1; i >= 0; i-- ) 
			{             
				//  ���_���W�̐ݒ�( 3D���W �E �E��� )
				pVtx[ countLoop ].position = positions[ m_meshes[ countMesh ].positionIndices[ i ] ];
				pVtx[ countLoop ].position.x = -pVtx[ countLoop ].position.x;

				//  �@���̎w��
				pVtx[ countLoop ].normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ i ] ];

				//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
				pVtx[ countLoop ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV���W�̎w��
				pVtx[ countLoop ].texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ i ] ];

				countLoop++;
			}

			//  ���_�o�b�t�@�̃A�����b�N
			m_pVtxBuffers.at( countMesh )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  �g��s��̍쐬
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  �g��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  �Œ�|�[�Y�̏ꍇ
		if( m_fixedPose )
		{
			//  ��]�s��̍쐬
			Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}
		else
		{
			//  ��]�s��̍쐬
			Utility::GetLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}

		//  ���s�ړ��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

		//  ���s�ړ��s��̍쐬
		D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

		//  ���s�ړ��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

		Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
		D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );

		//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
		shader3DOutline->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
								  m_pVtxBuffers.at( countMesh ) ,	//  ���_�o�b�t�@�̃A�h���X
								  0 ,								//  �I�t�Z�b�g( byte )
								  sizeof( VERTEX_3D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

		//  �V�F�[�_�[3D�̕`��J�n
		shader3DOutline->DrawBegin( );

		// �|���S���̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,										//  �v���~�e�B�u�̎��
								0 ,															//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
								m_meshes[ countMesh ].positionIndices.size( ) / 3 );		//  �v���~�e�B�u��

		//  �V�F�[�_�[3D�̕`��I��
		ShaderManager::DrawEnd( );
	}

	//  �����_�[�X�e�[�g�̐ݒ�
    pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );			//  �\�ʕ`��

	//  �T���v���[�̃C���f�b�N�X���擾
	UINT textureIndex = shader3DPlayer->GetSamplerTextureIndex( );
	UINT thunIndex = shader3DPlayer->GetSamplerThunIndex( );
	UINT normalIndex = shader3DPlayer->GetSamplerNormalIndex( );
	UINT specularIndex = shader3DPlayer->GetSamplerSpecularIndex( );

	//  �e�N�X�`���N���X�̎擾
	Texture* pTexture = SceneManager::GetTexture( );

	//  �e�N�X�`���̐ݒ�
	pDevice->SetTexture( thunIndex , pTexture->GetTextureImage( THUN_SHADER_TEXTURE_NAME ) );

	for( int countMesh = 0; countMesh != m_meshes.size( ); ++countMesh ) 
	{  
		// ������i�܂胏���X�L���Ȃǁj 
 
		// ���_�̍��W�ϊ�             
		std::vector< D3DXVECTOR3 > positions;
		int size = m_meshes[ countMesh ].points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             

		for( unsigned int countPoint = 0; countPoint < m_meshes[ countMesh ].points.size( ); ++countPoint ) 
		{
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ m_motionIndex ].frame[ m_currentFrame % m_animator->motion[ m_motionIndex ].frame.size( ) ].matrix[ m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].index ] * m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBorn ].weight;
			}

			if( m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = m_meshes[ countMesh ].points[ countPoint ].positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

		if( m_pVtxBuffers.at( countMesh ) != NULL )
		{
			//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
			m_pVtxBuffers.at( countMesh )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
												 ( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
												 0 );							//  ���b�N�̎��

			//  ���]�p�̃��[�v�J�E���g
			int countLoop = 0;

			//for( int i = 0; i < m_meshes[ countMesh ].positionIndices.size( ); i++ ) 
			for( int i = m_meshes[ countMesh ].positionIndices.size( ) - 1; i >= 0; i-- ) 
			{             
				//  ���_���W�̐ݒ�( 3D���W �E �E��� )
				pVtx[ countLoop ].position = positions[ m_meshes[ countMesh ].positionIndices[ i ] ];
				pVtx[ countLoop ].position.x = -pVtx[ countLoop ].position.x;

				//  �@���̎w��
				pVtx[ countLoop ].normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ i ] ];

				//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
				pVtx[ countLoop ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV���W�̎w��
				pVtx[ countLoop ].texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ i ] ];

				countLoop++;
			}

			//  ���_�o�b�t�@�̃A�����b�N
			m_pVtxBuffers.at( countMesh )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  �g��s��̍쐬
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  �g��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  �Œ�|�[�Y�̏ꍇ
		if( m_fixedPose )
		{
			//  ��]�s��̍쐬
			Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}
		else
		{
			//  ��]�s��̍쐬
			Utility::GetLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}

		//  ���s�ړ��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

		//  ���s�ړ��s��̍쐬
		D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

		//  ���s�ړ��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

		//  ���C�g�̕����x�N�g�����擾����
		D3DXVECTOR3 lightDirectWorld = SceneManager::GetLight( )->GetDirection( );
		D3DXVec3Normalize( &lightDirectWorld , &lightDirectWorld );

		//  ���[�J���P�ʃx�N�g���ɕύX
		D3DXVECTOR3 lightDirectLocal;

		D3DXMATRIX worldInverseMatrix;
		D3DXMatrixInverse( &worldInverseMatrix , NULL , &( mtxRot * mtxTrans ) );
		D3DXVec3TransformNormal( &lightDirectLocal , &lightDirectWorld , &worldInverseMatrix );
		D3DXVec3Normalize( &lightDirectLocal , &lightDirectLocal );

		D3DXMATRIX worldInverseTransposeMatrix;
		D3DXMatrixInverse( &worldInverseTransposeMatrix , NULL , &mtxWorld );
		D3DXMatrixTranspose( &worldInverseTransposeMatrix , &worldInverseTransposeMatrix );
		float specularPower = 10.0f;

		Camera* camera = SceneManager::GetCamera( SceneManager::GetLoop( ) );
		D3DXVECTOR3 cameraPositionEyeLocal = camera->GetCameraPosEye( );
		D3DXMATRIX viewMatrix = camera->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = camera->GetProjectionMatrix( );
		D3DXCOLOR lightDiffuseColor = SceneManager::GetLight( )->GetLight( 0 ).Diffuse;

		//  �J�����̃��[�J�����W�̎擾
		D3DXVec3TransformCoord( &cameraPositionEyeLocal , &cameraPositionEyeLocal , &worldInverseMatrix );

		//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
		shader3DPlayer->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix , lightDirectLocal , lightDiffuseColor , cameraPositionEyeLocal );

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource( 0 ,									//  �p�C�v���C���ԍ�
								  m_pVtxBuffers.at( countMesh ) ,		//  ���_�o�b�t�@�̃A�h���X
								  0 ,									//  �I�t�Z�b�g( byte )
								  sizeof( VERTEX_3D ) );				//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

		//  �e�N�X�`�������݂��Ă���ꍇ
		if( m_diffuseMap.find( m_meshes[ countMesh ].materialName ) != m_diffuseMap.end( ) )
		{
			//  �e�N�X�`���̐ݒ�
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( m_diffuseMap[ m_meshes[ countMesh ].materialName ].c_str( ) ) ); 
		}		
		else
		{
			//  �e�N�X�`���̐ݒ�
			pDevice->SetTexture( textureIndex , pTexture->GetTextureImage( NONE_SPECULAR_TEXTURE_NAME ) ); 
		}

		//  �@���}�b�v�e�N�X�`�������݂��Ă���ꍇ
		if( m_normalMap.find( m_meshes[ countMesh ].materialName ) != m_normalMap.end( ) )
		{
			//  �@���}�b�v�e�N�X�`���̐ݒ�
			pDevice->SetTexture( normalIndex , pTexture->GetTextureImage( m_normalMap[m_meshes[ countMesh ].materialName ].c_str( ) ) ); 
		}		
		else
		{
			//  �@���}�b�v�e�N�X�`���̐ݒ�
			pDevice->SetTexture( normalIndex , nullptr ); 
		}

		//  �X�y�L�����[�}�b�v�e�N�X�`�������݂��Ă���ꍇ
		if( m_specularMap.find( m_meshes[ countMesh ].materialName ) != m_specularMap.end( ) )
		{
			//  �X�y�L�����[�}�b�v�e�N�X�`���̐ݒ�
			pDevice->SetTexture( specularIndex , pTexture->GetTextureImage( m_specularMap[ m_meshes[ countMesh ].materialName ].c_str( ) ) ); 
		}		
		else
		{
			//  �X�y�L�����[�}�b�v�e�N�X�`���̐ݒ�
			pDevice->SetTexture( specularIndex , pTexture->GetTextureImage( NONE_SPECULAR_TEXTURE_NAME ) ); 
		}

		//  ���e�X�g�̗L��
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , TRUE );

		//  ������
		pDevice->SetRenderState( D3DRS_ALPHAREF , 1 );

		//  ���Z��
		pDevice->SetRenderState( D3DRS_ALPHAFUNC , D3DCMP_GREATER );

		//  �V�F�[�_�[3D�̕`��J�n
		shader3DPlayer->DrawBegin( );

		// �|���S���̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,										//  �v���~�e�B�u�̎��
								0 ,															//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
								m_meshes[ countMesh ].positionIndices.size( ) / 3 );		//  �v���~�e�B�u��

		//  �V�F�[�_�[3D�̕`��I��
		ShaderManager::DrawEnd( );

		//  ���e�X�g�̖���
		pDevice->SetRenderState( D3DRS_ALPHATESTENABLE , FALSE );

		pDevice->SetRenderState( D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA );					//  ���u�����h�ݒ�( �����ߏ��� )
	}
}

//--------------------------------------------------------------------------------------
//  FBX�̃f�v�X�l�������ݏ���
//--------------------------------------------------------------------------------------
void FBXLoader::DrawDepth( void )
{
	D3DXMATRIX mtxWorld;							//  ���[���h�s��
	D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
	D3DXMATRIX mtxScale;							//  �g��s��
	D3DXMATRIX mtxRot;								//  ��]���W�ϊ��s��

	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	for( int countMesh = 0; countMesh != m_meshes.size( ); ++countMesh ) 
	{  
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture( 0 , nullptr );

		// ������i�܂胏���X�L���Ȃǁj 
 
		// ���_�̍��W�ϊ�             
		std::vector< D3DXVECTOR3 > positions;
		int size = m_meshes[ countMesh ].points.size( );

		positions.reserve( size ); 
 
		D3DXMATRIX mtx;             
				
		for( auto it = m_meshes[ countMesh ].points.begin( ); it != m_meshes[ countMesh ].points.end( ); ++it ) 
		{            
			ZeroMemory( &mtx, sizeof( D3DXMATRIX ) );   

			for( unsigned int countBorn = 0; countBorn < it->bornRefarences.size( ); ++countBorn )
			{
				mtx += m_animator->motion[ m_motionIndex ].frame[ m_currentFrame % m_animator->motion[ m_motionIndex ].frame.size( ) ].matrix[ it->bornRefarences[ countBorn ].index ] * it->bornRefarences[ countBorn ].weight;
			}

			if( it->bornRefarences.size( ) == 0 )
			{
				D3DXMatrixIdentity( &mtx );
			}

			D3DXVECTOR3 pos = it->positions;

			D3DXVec3TransformCoord( &pos , &pos , &mtx );                
			positions.push_back( pos );             
		} 

		VERTEX_3D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^

		if( m_pVtxBuffers.at( countMesh ) != NULL )
		{
			//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
			m_pVtxBuffers.at( countMesh )->Lock( 0 , 0 ,						//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
												 ( void** )&pVtx ,				//  �A�h���X�������ꂽ�������̃A�h���X
												 0 );							//  ���b�N�̎��

			//  ���]�p�̃��[�v�J�E���g
			int countLoop = 0;

			for( int i = m_meshes[ countMesh ].positionIndices.size( ) - 1; i >= 0; i-- ) 
			{             
				//  ���_���W�̐ݒ�( 3D���W �E �E��� )
				pVtx[ countLoop ].position = positions[ m_meshes[ countMesh ].positionIndices[ i ] ];
				pVtx[ countLoop ].position.x = -pVtx[ countLoop ].position.x;

				//  �@���̎w��
				pVtx[ countLoop ].normal = m_meshes[ countMesh ].normals[ m_meshes[ countMesh ].normalIndices[ i ] ];

				//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
				pVtx[ countLoop ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

				//  UV���W�̎w��
				pVtx[ countLoop ].texcoord = m_meshes[ countMesh ].texcoords[ m_meshes[ countMesh ].texcoordIndices[ i ] ];

				countLoop++;
			}

			//  ���_�o�b�t�@�̃A�����b�N
			m_pVtxBuffers.at( countMesh )->Unlock( );     
		}

		D3DXMatrixIdentity( &mtxWorld );
		D3DXMatrixIdentity( &mtxScale );
		D3DXMatrixIdentity( &mtxRot );

		//  �g��s��̍쐬
		D3DXMatrixScaling( &mtxScale , m_scale , m_scale , m_scale );

		//  �g��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

		//  �Œ�|�[�Y�̏ꍇ
		if( m_fixedPose )
		{
			//  ��]�s��̍쐬
			Utility::GetFixedLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}
		else
		{
			//  ��]�s��̍쐬
			Utility::GetLookAtMatrix( &mtxRot , &m_position , &m_positionAt , &D3DXVECTOR3( 0.0f , 1.0f , 0.0f ) );
		}

		//  ���s�ړ��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

		//  ���s�ړ��s��̍쐬
		D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

		//  ���s�ړ��s��̊|���Z
		D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

		//  �V�F�[�_�[���̎擾
		Shader3DShadowMap* shader3DShadowMap = ( Shader3DShadowMap* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_SHADOW_MAP );

		//  �J�����N���X�̎擾
		Camera* pCamera = SceneManager::GetCamera( SceneManager::GetLoop( ) );

		D3DXMATRIX viewMatrix = SceneManager::GetLight( )->GetViewMatrix( );
		D3DXMATRIX projectionMatrix = SceneManager::GetLight( )->GetProjectionMatrix( );
		float fFar = SceneManager::GetLight( )->GetFar( );

		shader3DShadowMap->SetShaderInfo( mtxWorld , viewMatrix * projectionMatrix , fFar );

		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource( 0 ,									//  �p�C�v���C���ԍ�
								  m_pVtxBuffers.at( countMesh ) ,		//  ���_�o�b�t�@�̃A�h���X
								  0 ,									//  �I�t�Z�b�g( byte )
								  sizeof( VERTEX_3D ) );				//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

		//  �V�F�[�_�[�`��J�n
		shader3DShadowMap->DrawBegin( );

		// �|���S���̕`��
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  �v���~�e�B�u�̎��
								0 ,										//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
								m_meshes[ countMesh ].positionIndices.size( ) / 3 );		//  �v���~�e�B�u��
			
		//  �`��I��
		ShaderManager::DrawEnd( );
	}
}

//--------------------------------------------------------------------------------------
//  ���[�V�����̐ݒ肷��֐�
//--------------------------------------------------------------------------------------
void FBXLoader::SetMotion( MOTION motion )
{
	m_motionIndex = ( int )motion;

	m_motionFinish = false;

	m_currentFrame = 0;
}

//--------------------------------------------------------------------------------------
//  ���[�V�����t���[���擾
//--------------------------------------------------------------------------------------
int FBXLoader::GetMotionFrame( void )
{
	return m_currentFrame;
}

//--------------------------------------------------------------------------------------
//  ���[�V�����S�̂���t���[�������擾
//--------------------------------------------------------------------------------------
float FBXLoader::GetMotionFrameRate( void )
{
	float rate = ( float )m_currentFrame / ( float )m_animator->motion[ m_motionIndex ].frame.size( );

	return rate;
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzePosition( FbxMesh* pMesh ) 
{     
	// �R���g���[���|�C���g���̎擾     
	int controlPointsCount = pMesh->GetControlPointsCount( ); 
 
    // ���_���W�p�R���e�i�̗̈�\��     
	m_meshes.back( ).points.reserve( controlPointsCount ); 
 
    // ���_�f�[�^�̎擾     
	FbxVector4* pP = pMesh->GetControlPoints( );     
	
	for( int i = 0; i < pMesh->GetControlPointsCount( ); i++ ) 
	{         
		m_meshes.back( ).points.push_back( D3DXVECTOR3( static_cast< float >( pP[ i ][ 0 ] ) ,                          
													    static_cast< float >( pP[ i ][ 1 ] ) ,                          
													    static_cast< float >( pP[ i ][ 2 ] ) ) );     
	} 
 
    /* ���_�C���f�b�N�X�̎擾 */ 
 
    // �C���f�b�N�X�����擾     
	int polygonVertexCount = pMesh->GetPolygonVertexCount( ); 
 
    // ���_���W�C���f�b�N�X�p�R���e�i�̗̈�\��     
	m_meshes.back( ).positionIndices.reserve( polygonVertexCount );          
	
	// �C���f�b�N�X�o�b�t�@�̎擾     
	for( int i = 0; i < polygonVertexCount; i++ )
	{         
		m_meshes.back( ).positionIndices.push_back( static_cast< unsigned short >( pMesh->GetPolygonVertices( )[ i ] ) );     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzeNormal( FbxMesh* pMesh ) 
{     
	// ���C���[���̎擾     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // �@���̎擾         
		FbxLayerElementNormal* pElementNormal = pMesh->GetElementNormal( layer ); 
 
        if( !pElementNormal ) 
		{ 
			continue; 
		} 
 
        // �@�����̎擾         
		int normalCount = pElementNormal->GetDirectArray( ).GetCount( ); 
 
        // �@���i�[�p�R���e�i�̗̈�\��         
		m_meshes.back( ).normals.reserve( normalCount );
 
        // �@���f�[�^�̎擾         
		for( int i = 0; i < normalCount; i++ ) 
		{             
			m_meshes.back( ).normals.push_back( D3DXVECTOR3( static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 0 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 1 ] ) ,                 
														     static_cast< float >( pElementNormal->GetDirectArray( )[ i ][ 2 ] ) ) );         
		} 
 
        // �}�b�s���O���[�h�E���t�@�����X���[�h�擾         
		FbxLayerElement::EMappingMode mappingMode = pElementNormal->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementNormal->GetReferenceMode( ); 
 
        switch( mappingMode ) 
		{ 
			case FbxLayerElement::eNone:  
			{
				MessageBox( NULL , "Normal MappingMode = mappingMode" , "������" , MB_OK );      
				break; 
			}
			case FbxLayerElement::eByControlPoint: 
			{
				// ���_�o�b�t�@�Ɠ����C���f�b�N�X���g�p 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                 
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					m_meshes.back( ).normalIndices.reserve( m_meshes.back( ).points.size( ) );                 
				
					// ���_�o�b�t�@�Ɠ����C���f�b�N�X���R�s�[                 
					m_meshes.back( ).normalIndices.assign( m_meshes.back( ).positionIndices.begin( ) , m_meshes.back( ).positionIndices.end( ) );             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{                 
					MessageBox( NULL ,  "Normal ReferenceMode = eIndexToDirect or eIndex, MappingMode = eByControlPoint" ,  "������" , MB_OK );             
				}             
			
				break; 
			}
			case FbxLayerElement::eByPolygonVertex:
			{
				/* �@���Ǝ��̃C���f�b�N�X���g�p */ 
 
				if( referenceMode == FbxLayerElement::eDirect ) 
				{                                  
					// �C���f�b�N�X�Q�Ƃ̕K�v�Ȃ� => �C���f�b�N�X���쐬 
 
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					m_meshes.back( ).normalIndices.reserve( normalCount );                 
				
					// �@���C���f�b�N�X�̍쐬                 
					for( int i = 0; i < normalCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( i );                 
					}             
				}             
				else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
				{ 
					// �Ǝ��C���f�b�N�X������ 
 
					// �C���f�b�N�X���̎擾                 
					int normalIndexCount = pElementNormal->GetIndexArray( ).GetCount( );                 
				
					// �@���C���f�b�N�X�i�[�p�R���e�i�̗̈�\��                 
					m_meshes.back( ).normalIndices.reserve( normalIndexCount );                 
				
					// �@���C���f�b�N�X�̎擾                 
					for( int i = 0; i < normalIndexCount; i++ ) 
					{                     
						m_meshes.back( ).normalIndices.push_back( pElementNormal->GetIndexArray( )[ i ] );                 
					}             
				}

				break; 
			}
			case FbxLayerElement::eByPolygon:       
			{
				MessageBox( NULL , "Normal MappingMode = eByPolygon", "������" , MB_OK );             
				break;       
			}
			case FbxLayerElement::eByEdge: 
			{
				MessageBox( NULL , "Normal MappingMode = eByEdge" , "������" , MB_OK );             
				break;         
			}
			case FbxLayerElement::eAllSame:  
			{
				MessageBox( NULL , "Normal MappingMode = eAllSame" , "������" , MB_OK );            
				break;   
			}
			default: 
			{
				MessageBox( NULL , "Normal ???" , "������" , MB_OK );             
				break;         
			}     
		}
	} 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzeTexcoord( FbxMesh* pMesh , bool bRevers ) 
{ 
	int layerCount = pMesh->GetLayerCount( ); 
 
	if( !layerCount ) 
	{ 
		MessageBox(NULL, "���C���[�������Ă��Ȃ����b�V�����m�F", "Analyze Texcoord", MB_OK); 
		return; 
	} 
 
	for( int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // UV�̎擾         
		FbxLayerElementUV* pElementUV = pMesh->GetLayer( layer )->GetUVs( ); 
 
        if( !pElementUV ) 
		{             
			MessageBox( NULL , "...UV�̂Ȃ����b�V�����C���[���m�F" , "Analyze Texcoord" , MB_OK );             
			continue;         
		} 
 
        // UV�Z�b�g�����擾         
		// = pElementUV->GetName(); 
 
        // �}�b�s���O���[�h�E���t�@�����X���[�h�擾         
		FbxLayerElement::EMappingMode mappingMode = pElementUV->GetMappingMode( );         
		FbxLayerElement::EReferenceMode referenceMode = pElementUV->GetReferenceMode( ); 
 
        if( mappingMode == FbxLayerElement::eByPolygonVertex ) 
		{             
			if( referenceMode == FbxLayerElement::eIndexToDirect || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				int uvIndexCount = pElementUV->GetIndexArray( ).GetCount( );                
				m_meshes.back( ).texcoordIndices.reserve( uvIndexCount ); 
 
                for( int i = 0; i < uvIndexCount; i++ ) 
				{                     
					m_meshes.back( ).texcoordIndices.push_back( pElementUV->GetIndexArray( ).GetAt( i ) );                 
				} 
 
                int uvCount = pElementUV->GetDirectArray( ).GetCount( );      

				m_meshes.back().texcoords.reserve( uvCount ); 
 
                for( int i = 0; i < uvCount; i++ ) 
				{ 
                    m_meshes.back( ).texcoords.push_back( D3DXVECTOR2( static_cast< float >( pElementUV->GetDirectArray( ).GetAt( i )[ 0 ] ),                         
																	   static_cast< float >( bRevers ? 1 - pElementUV->GetDirectArray( ).GetAt( i )[ 1 ]  : pElementUV->GetDirectArray( ).GetAt( i )[ 1 ] ) ) );                 
				}             																   
			}             
			else 
			{                 
				MessageBox(NULL, "Texcoord::���Ή��̃��t�@�����X���[�h���擾", "FbxLayerElement::eByPolygonVertex", MB_OK);                 
				break;             
			}         
		}         
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{             
			MessageBox( NULL , "...���Ή��}�b�s���O���[�h[eByControlPoint]���擾����", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByPolygon) 
		{             
			MessageBox( NULL , "...���Ή��}�b�s���O���[�h[eByPolygon]���擾����", "Analyze Texcoord" , MB_OK );         
		}         
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{             
			MessageBox( NULL , "...���Ή��}�b�s���O���[�h[eByEdge]���擾����", "Analyze Texcoord" , MB_OK );         
		}         
		else 
		{             
			MessageBox( NULL , "...�m��Ȃ��}�b�s���O���[�h���擾����", "Analyze Texcoord" , MB_OK );         
		} 
 
        break; // �Ƃ肠�����P�߂���     
	} 
}
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzeMaterial( FbxMesh* pMesh ) 
{     
	int layerCount = pMesh->GetLayerCount( ); 
 
    for(int layer = 0; layer < layerCount; layer++) 
	{ 
		FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer( layer )->GetMaterials( );

		if( !pElementMaterial ) 
		{ 
			continue; 
		} 
 
		int materialIndexCount = pElementMaterial->GetIndexArray( ).GetCount( ); 
 
		if( materialIndexCount == 0 ) 
		{ 
			continue; 
		} 
 
		FbxLayerElement::EMappingMode mappingMode = pElementMaterial->GetMappingMode( );     
		FbxLayerElement::EReferenceMode referenceMode = pElementMaterial->GetReferenceMode( ); 
 
		if( mappingMode == FbxLayerElement::eAllSame ) 
		{         
			if( referenceMode == FbxLayerElement::eIndexToDirect ) 
			{        
				// ���b�V���S�������̃}�e���A���C���f�b�N�X         
				m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );         
			}         
			else 
			{         
				MessageBox(NULL, "...���Ή��̃��t�@�����X���[�h���擾����", "Material MappingMode = eAllSame", MB_OK);         
			}     
		}     
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{         
			MessageBox(NULL, "...���Ή��̃}�b�s���O���[�h[eByControlPoint]���擾����", "Material MappingMode", MB_OK);     
		}     
		else if( mappingMode == FbxLayerElement::eByPolygon ) 
		{         
			// �}�e���A����������Ă���͂�������A��Ԃ͂��߂̂����ł���         
			m_meshes.back( ).materialIndex = pElementMaterial->GetIndexArray( ).GetAt( 0 );     
		}     
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{         
			MessageBox(NULL, "...���Ή��̃}�b�s���O���[�h[eByEdge]���擾����", "Material MappingMode", MB_OK);     
		}     
		else 
		{         
			MessageBox(NULL, "...���Ή��̃}�b�s���O���[�h���擾����", "Material MappingMode", MB_OK);     
		}     
	} 
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
// Get the geometry offset to a node. It is never inherited by the children. 
FbxAMatrix FBXLoader::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );     
	const FbxVector4 lR = pNode->GetGeometricRotation( FbxNode::eSourcePivot );     
	const FbxVector4 lS = pNode->GetGeometricScaling( FbxNode::eSourcePivot ); 
 
    return FbxAMatrix( lT , lR , lS ); 
} 
 
//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzeCluster( FbxMesh* pMesh , int index ) 
{     
	D3DXMATRIX mtxIdentitiy;     
	D3DXMatrixIdentity( &mtxIdentitiy ); 

    // �X�L���̐����擾     
	int skinCount = pMesh->GetDeformerCount( FbxDeformer::eSkin ); 
 
    for( int skinNum = 0; skinNum < skinCount; skinNum++ ) 
	{ 
        // �X�L�����擾         
		FbxSkin* pSkin = ( FbxSkin* )pMesh->GetDeformer( skinNum , FbxDeformer::eSkin ); 
 
        // �N���X�^�[�̐����擾         
		int clusterCount = pSkin->GetClusterCount( ); 

		//  �N���X�^�[�̐����̃������m��
		m_meshes.at( index ).initInvMatrix.resize( clusterCount );
 
        for( int clusterNum = 0; clusterNum < clusterCount; clusterNum++ ) 
		{ 
			// �N���X�^���擾             
			FbxCluster* pCluster = pSkin->GetCluster( clusterNum ); 
 
			// ���̃N���X�^���e�����y�ڂ����_�C���f�b�N�X�̌����擾             
			int pointIndexCount = pCluster->GetControlPointIndicesCount( ); 

			int* pointIndexArray = pCluster->GetControlPointIndices( );             
			double* weightArray = pCluster->GetControlPointWeights( ); 

			std::string name = pCluster->GetLink( )->GetName( );
 
			for( int i = 0 ; i < pointIndexCount; i++ ) 
			{              
				m_meshes.at( index ).points[ pointIndexArray[ i ] ].bornRefarences.push_back( BornRefarence( clusterNum ,
																											 static_cast< float >( weightArray[ i ] ) ,
																											 name ) );     
			}  
		}		
	} 
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
FBXLoader::Animator* FBXLoader::AnalyzeAnimation( FbxImporter* importer , FbxScene* scene ) 
{     
	//  �A�j���[�V�������̎擾
	int numberAnimation = importer->GetAnimStackCount( );

	if( numberAnimation == 0 )
	{
		return nullptr;
	}

	//  �N���X�^�[�̐��̎擾
	int numberCluster = scene->GetMemberCount< FbxCluster >( );

	std::unordered_map< std::string , FbxCluster* > mapCluster;

	for( int countCluster = 0; countCluster < numberCluster; ++countCluster )
	{
		FbxCluster* workCluster = scene->GetSrcObject< FbxCluster >( countCluster );

		std::string clusterName = workCluster->GetLink( )->GetName( );

		//  ���ꖼ�̃N���X�^�[�̌���
		if( mapCluster.find( clusterName ) == mapCluster.end( ) )
		{
			mapCluster.emplace( clusterName , workCluster );
		}
	}

	//  �A�j���[�V�����̐����̃f�[�^����
	Animator* animator = new Animator( );
	animator->motion.reserve( numberAnimation );

	animator->cluster.resize( mapCluster.size( ) );

	int countCluster = 0;

	for( auto pair : mapCluster )
	{
		animator->cluster[ countCluster ].name = pair.first;

		FbxAMatrix meshGlobalInitPosition;
		FbxAMatrix bornGlobalInitPosition;

		pair.second->GetTransformMatrix( meshGlobalInitPosition );
		pair.second->GetTransformLinkMatrix( bornGlobalInitPosition );

		//  �����p���s��
		FbxAMatrix bindPoseInverse = bornGlobalInitPosition.Inverse( ) * meshGlobalInitPosition;

		for( int y = 0; y < 4; y++ ) 
		{                 
			for( int x = 0; x < 4; x++ ) 
			{ 
				animator->cluster[ countCluster ].initInvMatrix( x , y ) = ( float )bindPoseInverse.Get( x , y );                 
			}  
		} 	

		countCluster++;
	}

	//  ���[�V�������̎擾
	FbxArray< FbxString* > motionName;
	scene->FillAnimStackNameArray( motionName );

	//  1�t���[��
	FbxTime oneFrameTime;
	oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

	for( int countAnimation = 0; countAnimation < numberAnimation; ++countAnimation )
	{	
		Motion motion;
		motion.name = motionName[ countAnimation ]->Buffer( );

		//  �A�j���[�V�������̎擾
		FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( motionName[ countAnimation ]->Buffer( ) );

		//  �A�j���[�V�����̊J�n�E�I�����Ԃ̎擾
		FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
		FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

		//  �����t���[�����̎擾
		int allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

		//  �t���[�������̃������m��
		motion.frame.reserve( allTime );

		for( FbxTime currentTime = startTime; currentTime < endTime; currentTime += oneFrameTime )
		{
			Frame frame;
			frame.matrix.resize( mapCluster.size( ) );

			countCluster = 0;

			for( auto pair : mapCluster )
			{
				FbxAMatrix bornMatrix = pair.second->GetLink( )->EvaluateGlobalTransform( currentTime );

				D3DXMATRIX workMatrix;

				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						workMatrix( x , y ) = ( float )bornMatrix.Get( x , y );                 
					}  
				} 	


				frame.matrix[ countCluster ] = animator->cluster[ countCluster ].initInvMatrix * workMatrix;

				countCluster++;
			}

			motion.frame.push_back( frame );
		}

		//  �����������킹��
		motion.frame.shrink_to_fit( );

		animator->motion.push_back( motion );
	}

	//  �����������킹��
	animator->motion.shrink_to_fit( );

	return animator;
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::AnalyzeNoMeshAnimation( FbxImporter* importer , FbxScene* scene , Animator* animator , bool loop )
{
	//  �A�j���[�V�������̎擾
	int numberAnimation = importer->GetAnimStackCount( );

	if( numberAnimation == 0 )
	{
		return;
	}

	std::list< FbxNode* > skeleton;

	//  ���[�g�m�[�h����
	FbxNode* rootNode = scene->GetRootNode( );

	//  �ċA�I�ɃX�P���g������
	FindSkeleton( rootNode , skeleton );

	if( skeleton.size( ) < animator->cluster.size( ) )
	{
		return;
	}

	animator->motion.reserve( numberAnimation + animator->motion.size( ) );

	//  ���[�V�������̎擾
	FbxArray< FbxString* > motionName;
	scene->FillAnimStackNameArray( motionName );

	//  1�t���[��
	FbxTime oneFrameTime;
	oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

	int countCluster = 0;

	for( int countAnimation = 0; countAnimation < numberAnimation; ++countAnimation )
	{	
		Motion motion;
		motion.name = motionName[ countAnimation ]->Buffer( );

		//  ���[�v���̑��
		motion.loop = loop;

		//  �A�j���[�V�������̎擾
		FbxTakeInfo* pTakeInfo = scene->GetTakeInfo( motionName[ countAnimation ]->Buffer( ) );

		//  �A�j���[�V�����̊J�n�E�I�����Ԃ̎擾
		FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
		FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );

		//  �����t���[�����̎擾
		int allTime = ( int )( ( endTime - startTime ) / oneFrameTime ).Get( );

		//  �t���[�������̃������m��
		motion.frame.reserve( allTime );

		for( FbxTime currentTime = startTime; currentTime < endTime; currentTime += oneFrameTime )
		{
			Frame frame;
			frame.matrix.resize( skeleton.size( ) );

			for( auto single : skeleton )
			{
				//  �X�P���g�����̎擾
				std::string name = single->GetName( );

				int clusterNo = -1;

				for( unsigned int countCluster = 0; countCluster < animator->cluster.size( ); ++countCluster )
				{
					if( name == animator->cluster[ countCluster ].name )
					{
						clusterNo = countCluster;

						break;
					}
				}

				if( clusterNo == -1 )
				{
					continue;
				}

				FbxAMatrix bornMatrix = single->EvaluateGlobalTransform( currentTime );

				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						frame.matrix[ clusterNo ]( x , y ) = ( float )bornMatrix.Get( x , y );        
					}  
				} 	

				frame.matrix[ clusterNo ] = animator->cluster[ clusterNo ].initInvMatrix * frame.matrix[ clusterNo ];
			}

			motion.frame.push_back( frame );
		}

		//  �����������킹��
		motion.frame.shrink_to_fit( );

		animator->motion.push_back( motion );
	}

	//  �����������킹��
	animator->motion.shrink_to_fit( );
}

//--------------------------------------------------------------------------------------
//  �m�[�h��H���čċA�I�ɏ����擾����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::FindSkeleton( FbxNode* node , std::list< FbxNode* > &skeletonList )
{
	if( node == nullptr )
	{
		return;
	}

	for( int countNode = 0; countNode < node->GetNodeAttributeCount( ); ++countNode )
	{
		if( node->GetNodeAttributeByIndex( countNode )->GetAttributeType( ) == FbxNodeAttribute::eSkeleton )
		{
			skeletonList.push_back( node );
		}
	}

	for( int countChild = 0; countChild < node->GetChildCount( ); ++countChild )
	{
		FindSkeleton( node->GetChild( countChild ) , skeletonList );
	}
}

//--------------------------------------------------------------------------------------
//  �{�[���̃C���f�b�N�X�ԍ��̊��蓖��
//--------------------------------------------------------------------------------------
void FBXLoader::MatchBoneReferenceIndex( void )
{
	for( unsigned int countMesh = 0; countMesh < m_meshes.size( ); ++countMesh )
	{
		for( unsigned int countPoint = 0; countPoint < m_meshes[ countMesh ].points.size( ); ++countPoint )
		{
			for( unsigned int countBone = 0; countBone < m_meshes[ countMesh ].points[ countPoint ].bornRefarences.size( ); ++countBone )
			{
				for( unsigned int countCluster = 0; countCluster < m_animator->cluster.size( ); ++countCluster )
				{
					if( m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBone ].name == 
						m_animator->cluster[ countCluster ].name )
					{
						m_meshes[ countMesh ].points[ countPoint ].bornRefarences[ countBone ].index = countCluster;

						break;
					}
				}
			}
		}
	}
}

//--------------------------------------------------------------------------------------
//  ���݂̃t�@�C���|�C���^���炠�镶����̐����J�E���g����֐�
//--------------------------------------------------------------------------------------
std::string FBXLoader::GetAttributeTypeName(FbxNodeAttribute::EType type) 
{ 
    switch(type) { 
        case FbxNodeAttribute::eUnknown: return "unidentified"; 
        case FbxNodeAttribute::eNull: return "null"; 
        case FbxNodeAttribute::eMarker: return "marker"; 
        case FbxNodeAttribute::eSkeleton: return "skeleton"; 
        case FbxNodeAttribute::eMesh: return "mesh"; 
        case FbxNodeAttribute::eNurbs: return "nurbs"; 
        case FbxNodeAttribute::ePatch: return "patch"; 
        case FbxNodeAttribute::eCamera: return "camera"; 
        case FbxNodeAttribute::eCameraStereo: return "stereo"; 
        case FbxNodeAttribute::eCameraSwitcher: return "camera switcher"; 
        case FbxNodeAttribute::eLight: return "light"; 
        case FbxNodeAttribute::eOpticalReference: return "optical reference"; 
        case FbxNodeAttribute::eOpticalMarker: return "marker"; 
        case FbxNodeAttribute::eNurbsCurve: return "nurbs curve"; 
        case FbxNodeAttribute::eTrimNurbsSurface: return "trim nurbs surface"; 
        case FbxNodeAttribute::eBoundary: return "boundary"; 
        case FbxNodeAttribute::eNurbsSurface: return "nurbs surface"; 
        case FbxNodeAttribute::eShape: return "shape"; 
        case FbxNodeAttribute::eLODGroup: return "lodgroup"; 
        case FbxNodeAttribute::eSubDiv: return "subdiv"; 
        default: return "unknown"; 
    } 
}

//--------------------------------------------------------------------------------------
//  ���_�o�b�t�@���쐬����֐�
//--------------------------------------------------------------------------------------
void FBXLoader::MakeVertex( int size )
{     
	//  �f�o�C�X���̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ���_�o�b�t�@�̍쐬
	if( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * size ,				//  �쐬���������_�o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY ,				//  �g�p���@
											 0 ,								//  
											 D3DPOOL_MANAGED ,					//  �������Ǘ����@( MANAGED �� ���C�� )
											 &m_pVtxBuffers.back( ) ,			//  �o�b�t�@
											 NULL ) )
	{
		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

		return;
	}
}

//--------------------------------------------------------------------------------------
//  ��������֐�
//--------------------------------------------------------------------------------------
FBXLoader* FBXLoader::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 positionAt , float scale )
{
	FBXLoader* fbxLoader = new FBXLoader;

	//  ��ޑ��
	fbxLoader->m_type = type;

	//  ���W���
	fbxLoader->m_position = position;

	//  �����_���
	fbxLoader->m_positionAt = positionAt;

	//  �傫���{�����
	fbxLoader->m_scale = scale;

	//  ������
	fbxLoader->Init( );

	return fbxLoader;
}