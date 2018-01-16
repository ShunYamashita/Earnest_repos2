////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�{�b�N�X   ( meshField.cpp )
////
////  Author : SHUN YAMASHITA
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  �w�b�_�[�t�@�C��
////--------------------------------------------------------------------------------------
//#include "meshBoundingBox.h"
//#include "manager.h"
//#include "renderer.h"
//#include "texture.h"
//#include "shaderManager.h"
//#include "camera.h"
//#include <stdio.h>
//
////--------------------------------------------------------------------------------------
////  �}�N����`
////--------------------------------------------------------------------------------------
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�N���X�̃R���X�g���N�^
////--------------------------------------------------------------------------------------
//MeshBoundingBox::MeshBoundingBox( void ) : Scene( 2 )
//{
//	m_pVtxBuff = NULL;
//	m_pIndexBuff = NULL;
//}
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�N���X�̃f�X�g���N�^
////--------------------------------------------------------------------------------------
//MeshBoundingBox::~MeshBoundingBox( )
//{
//
//}
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�̏���������
////--------------------------------------------------------------------------------------
//HRESULT MeshBoundingBox::Init( void )
//{
//	//  ���_�̍쐬
//	MakeVertex( );
//
//	return S_OK;
//}
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�̏I������
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::Uninit( void )
//{
//	// ���_�o�b�t�@�̔j��
//	if( m_pVtxBuff != NULL )
//	{
//		m_pVtxBuff->Release( );
//		m_pVtxBuff = NULL;
//	}
//
//	// �C���f�b�N�X�o�b�t�@�̔j��
//	if( m_pIndexBuff != NULL )
//	{
//		m_pIndexBuff->Release( );
//		m_pIndexBuff = NULL;
//	}
//}
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�̍X�V����
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::Update( void )
//{
//	//  ���_���̐ݒ�
//	SetVertex( );
//}
//
////--------------------------------------------------------------------------------------
////  ���b�V���o�E���f�B���O�̕`�揈��
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::Draw( void )
//{
//	//  �e�N�X�`�����̎擾
//	Texture* pTexture = SceneManager::GetTexture( );
//
//	//  ���C������f�o�C�X�����擾
//	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
//
//	//  ���[���h�s��
//	D3DXMATRIX mtxWorld;							
//
//	//  �s���P�ʍs��ɕϊ�
//	D3DXMatrixIdentity( &mtxWorld );
//
//	//  GPU��VRAM�̐ڑ�
//	pDevice->SetStreamSource( 0 ,													//  �p�C�v���C���ԍ�
//							  m_pVtxBuff ,											//  ���_�o�b�t�@�̃A�h���X
//						  	  0 ,													//  �I�t�Z�b�g( byte )
//							  sizeof( VERTEX_3D ) );								//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )
//
//	//  �C���f�b�N�X�o�b�t�@�̐ݒ�
//	pDevice->SetIndices( m_pIndexBuff );
//
//	//  �V�F�[�_�[���̎擾
//	Shader3DNoLight* shader3DNoLight = ( Shader3DNoLight* )ShaderManager::GetShader( ShaderManager::TYPE::SHADER_3D_NO_LIGHT );
//
//	//  �r���[�ƃv���W�F�N�V�����s��̎擾
//	D3DXMATRIX viewMatrix = SceneManager::GetCamera( SceneManager::GetLoop( ) )->GetViewMatrix( );
//	D3DXMATRIX projectionMatrix = SceneManager::GetCamera( SceneManager::GetLoop( ) )->GetViewMatrix( );
//
//	//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
//	shader3DNoLight->SetShaderInfo( mtxWorld , viewMatrix , projectionMatrix );
//
//	//  �V�F�[�_�[3D�̕`��J�n
//	shader3DNoLight->DrawBegin( );
//
//	//  �v���~�e�B�u�̕`��
//	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST ,				//  �v���~�e�B�u�̎��
//								   0 ,								//  �ŏ��̒��_�C���f�b�N�X�ԍ��̃I�t�Z�b�g
//								   0 ,								//  �ŏ��̒��_�C���f�b�N�X�ԍ��̃I�t�Z�b�g
//								   8 ,								//  ���_��												
//								   0 ,								//  �X�^�[�g�C���f�b�N�X
//								   NUM_POLYGON * 8 );				//  �v���~�e�B�u��	
//
//	//  �V�F�[�_�[3D�̕`��I��
//	ShaderManager::DrawEnd( );
//}
//
////--------------------------------------------------------------------------------------
////  �C���X�^���X����������֐�
////--------------------------------------------------------------------------------------
//MeshBoundingBox* MeshBoundingBox::Create( const D3DXVECTOR3& min , const D3DXVECTOR3& max )
//{
//	MeshBoundingBox *pMeshBoundingBox;
//
//	//  �C���X�^���X����
//	pMeshBoundingBox = new MeshBoundingBox;
//
//	//  ������
//	pMeshBoundingBox->Init( );
//
//	return pMeshBoundingBox;
//}
//
////--------------------------------------------------------------------------------------
////  ���_���쐬����֐�
////--------------------------------------------------------------------------------------
//void MeshBoundingBox::MakeVertex( void )
//{
//	//  �f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );
//
//	//  ���_�o�b�t�@�̍쐬
//	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * 8 ,		//  �쐬���������_�o�b�t�@�̃T�C�Y
//											 D3DUSAGE_WRITEONLY ,			//  �g�p���@
//											 0 ,							//  
//											 D3DPOOL_MANAGED ,				//  �������Ǘ����@( MANAGED �� ���C�� )
//											 &m_pVtxBuff ,					//  �o�b�t�@
//											 NULL ) ) )
//	{
//		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );
//
//		return;
//	}
//
//	//  �C���f�b�N�X�o�b�t�@�̍쐬
//	if( FAILED( pDevice->CreateIndexBuffer( sizeof( WORD ) * 48 ,			//  �쐬���������_�o�b�t�@�̃T�C�Y											
//											D3DUSAGE_WRITEONLY ,			//  �g�p���@
//											D3DFMT_INDEX16 ,				//  
//											D3DPOOL_MANAGED ,				//  �������Ǘ����@( MANAGED �� ���C�� )
//											&m_pIndexBuff ,					//  �o�b�t�@
//											NULL ) ) )
//	{
//		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );
//
//		return;
//	}
//
//	VERTEX_3D* pVtx;												//  ���_�o�b�t�@�̉��z�A�h���X�p�|�C���^
//
//	//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
//	m_pVtxBuff->Lock( 0 , 0 ,										//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
//					  ( void** )&pVtx ,								//  �A�h���X�������ꂽ�������̃A�h���X
//					  0 );											//  ���b�N�̎��
//
//	for( int countCorner = 0; countCorner < 8; countCorner++ )
//	{
//		//  ���_���W�̐ݒ�( 2D���W �E �E��� )
//		pVtx[ 0 ].position = m_corners[ countCorner ];
//
//		//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
//		pVtx[ 0 ].color = m_color;
//
//		//  UV���W�̎w��
//		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f  , 0.0f );
//
//		//  �@���x�N�g���̐ݒ�
//		pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );
//
//		//  �A�h���X��i�߂�
//		pVtx++;
//	}
//
//	//  ���_�o�b�t�@�̃A�����b�N
//	m_pVtxBuff->Unlock( );
//
//	WORD* pIndexBuffer;											//  �C���f�b�N�X�o�b�t�@�̉��z�A�h���X�p�|�C���^
//
//	m_pIndexBuff->Lock( 0 , 0 ,									//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
//						( void** )&pIndexBuffer ,				//  �A�h���X�������ꂽ�������̃A�h���X
//						0 );									//  ���b�N�̎��
//
//	for( int i = 0; i < 48; i++ )
//	{
//		pIndexBuffer[ i ] = i;
//	}
//
//	//  �C���f�b�N�X�o�b�t�@�̃A�����b�N
//	m_pIndexBuff->Unlock( );
//}