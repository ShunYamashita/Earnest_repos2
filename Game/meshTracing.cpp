//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h   ( meshField.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "meshTracing.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "light.h"
#include "game.h"
#include "keyboard.h"
#include <stdio.h>

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
#define MESH_FIELD_TEXTURENAME0		"data/TEXTURE/EFFECT/effect000.jpg"				//  �e�N�X�`����

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
MeshTracing::MeshTracing( void ) : Scene( 5 )
{
	m_pVtxBuff = NULL;
	m_pIndexBuff = NULL;
	m_size = D3DXVECTOR3( 0.0f , 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�N���X�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
MeshTracing::~MeshTracing( )
{

}

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�̏���������
//--------------------------------------------------------------------------------------
HRESULT MeshTracing::Init( void )
{
	//  �e�N�X�`���N���X�̃|�C���^�擾
	Texture* pTexture = SceneManager::GetTexture( );

	m_nDivideSide = 5;
	m_nDivideVertical = 1;

	//  �e�N�X�`���̐ݒ�
	pTexture->SetTextureImage( MESH_FIELD_TEXTURENAME0 );

	for( int nCntSide = 0; nCntSide < m_nDivideSide; nCntSide++ )
	{
		//  ���_���W�̐ݒ�( 2D���W �E �E��� )
		m_vertexPosition[ nCntSide * 2 ] = m_newVertexPositionUp; 
		m_vertexPosition[ nCntSide * 2 + 1 ] = m_newVertexPositionDown; 
	}

	//  ���_�̍쐬
	MakeVertex( );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�̏I������
//--------------------------------------------------------------------------------------
void MeshTracing::Uninit( void )
{
	// ���_�o�b�t�@�̔j��
	if( m_pVtxBuff != NULL )
	{
		m_pVtxBuff->Release( );
		m_pVtxBuff = NULL;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if( m_pIndexBuff != NULL )
	{
		m_pIndexBuff->Release( );
		m_pIndexBuff = NULL;
	}
}

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�̍X�V����
//--------------------------------------------------------------------------------------
void MeshTracing::Update( void )
{
	//for( int nCntSide = 0; nCntSide < m_nDivideSide - 1; nCntSide++ )
	//{
	//	//  ���W�̓���ւ�
	//	m_vertexPosition[ nCntSide * 2 ] = m_vertexPosition[ ( nCntSide + 1 ) * 2 ]; 
	//	m_vertexPosition[ nCntSide * 2 + 1 ] = m_vertexPosition[ ( nCntSide + 1 ) * 2 + 1 ]; 
	//}

	//m_vertexPosition[ ( m_nDivideSide - 1 ) * 2 ] = m_newVertexPositionUp;
	//m_vertexPosition[ ( m_nDivideSide - 1 ) * 2 + 1 ] = m_newVertexPositionDown;

	//  ���_���̐ݒ�
	SetVertex( );

	m_countFrame++;

	if( m_countFrame > 300 )
	{
		m_countFrame = 0;
	}
}

//--------------------------------------------------------------------------------------
//  ���b�V���t�B�[���h�̕`�揈��
//--------------------------------------------------------------------------------------
void MeshTracing::Draw( void )
{
	//  �e�N�X�`�����̎擾
	Texture* pTexture = SceneManager::GetTexture( );

	D3DMATERIAL9 material;

	//  ���C������f�o�C�X�����擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DXMATRIX mtxWorld;							//  ���[���h�s��
	D3DXMATRIX mtxTrans;							//  �ړ��s��
	D3DXMATRIX mtxRot;								//  ��]�s��

	//  �s���P�ʍs��ɕϊ�
	D3DXMatrixIdentity( &mtxWorld );

	//  ���[���h���W�ϊ�
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	//  ���C�g������
	Light* pLight = SceneManager::GetLight( );
	pLight->LightOn( );

	//  �ގ��N���X�̏�����
	ZeroMemory( &material , sizeof( D3DMATERIAL9 ) );

	material.Diffuse = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );
	material.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

	//  �ގ��̐ݒ�
	pDevice->SetMaterial( &material );

	//  GPU��VRAM�̐ڑ�
	pDevice->SetStreamSource( 0 ,													//  �p�C�v���C���ԍ�
							  m_pVtxBuff ,											//  ���_�o�b�t�@�̃A�h���X
						  	  0 ,													//  �I�t�Z�b�g( byte )
							  sizeof( VERTEX_3D ) );								//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

	//  �C���f�b�N�X�o�b�t�@�̐ݒ�
	//pDevice->SetIndices( m_pIndexBuff );

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( MESH_FIELD_TEXTURENAME0 ) ); 

	//  ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_3D );

	// �|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  �v���~�e�B�u�̎��
							0 ,									//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
							NUM_POLYGON * m_nDivideSide );		//  �v���~�e�B�u��

	////  �v���~�e�B�u�̕`��
	//pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP ,									//  �v���~�e�B�u�̎��
	//							   0 ,														//  �ŏ��̒��_�C���f�b�N�X�ԍ��̃I�t�Z�b�g
	//							   0 ,														//  �ŏ��̒��_�C���f�b�N�X�ԍ��̃I�t�Z�b�g
	//							   ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 )					//  ���_��
	//							   * m_nDivideVertical						
	//							   + m_nDivideVertical - 1 ) ) ,														
	//							   0 ,														//  �X�^�[�g�C���f�b�N�X
	//							   NUM_POLYGON * m_nDivideSide *							//  �v���~�e�B�u��
	//							   m_nDivideVertical +						
	//							   4 * ( m_nDivideVertical - 1 ) );	

	//  ���C�g��_����
	pLight->LightOff( );
}

//--------------------------------------------------------------------------------------
//  �C���X�^���X����������֐�
//--------------------------------------------------------------------------------------
MeshTracing* MeshTracing::Create( D3DXCOLOR color , D3DXVECTOR3 positionUp , D3DXVECTOR3 positionDown )
{
	MeshTracing *pMeshTracing;

	//  �C���X�^���X����
	pMeshTracing = new MeshTracing;

		//  �F�̑��
	pMeshTracing->m_color = color;

	//  ���W�̑��
	pMeshTracing->m_newVertexPositionUp = positionUp;

	//  �_�E�����W�̑��
	pMeshTracing->m_newVertexPositionDown = positionDown;

	//  ������
	pMeshTracing->Init( );

	return pMeshTracing;
}

//--------------------------------------------------------------------------------------
//  ���_���쐬����֐�
//--------------------------------------------------------------------------------------
void MeshTracing::MakeVertex( void )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  ���_�o�b�t�@�̍쐬
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * ( m_nDivideSide ) *
																   ( m_nDivideVertical + 1 ) ,		//  �쐬���������_�o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY ,									//  �g�p���@
											 FVF_VERTEX_3D ,										//  
											 D3DPOOL_MANAGED ,										//  �������Ǘ����@( MANAGED �� ���C�� )
											 &m_pVtxBuff ,											//  �o�b�t�@
											 NULL ) ) )
	{
		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

		return;
	}

	////  �C���f�b�N�X�o�b�t�@�̍쐬
	//if( FAILED( pDevice->CreateIndexBuffer( sizeof( WORD ) * ( NUM_VERTEX2 * ( ( m_nDivideSide + 1 ) 
	//														 * m_nDivideVertical								//  �쐬���������_�o�b�t�@�̃T�C�Y
	//														 + m_nDivideVertical - 1 ) ) ,											
	//										D3DUSAGE_WRITEONLY ,												//  �g�p���@
	//										D3DFMT_INDEX16 ,													//  
	//										D3DPOOL_MANAGED ,													//  �������Ǘ����@( MANAGED �� ���C�� )
	//										&m_pIndexBuff ,														//  �o�b�t�@
	//										NULL ) ) )
	//{
	//	MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

	//	return;
	//}

	VERTEX_3D* pVtx;												//  ���_�o�b�t�@�̉��z�A�h���X�p�|�C���^

	//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	m_pVtxBuff->Lock( 0 , 0 ,										//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
					  ( void** )&pVtx ,								//  �A�h���X�������ꂽ�������̃A�h���X
					  0 );											//  ���b�N�̎��

	for( int nCntVertical = 0; nCntVertical < m_nDivideVertical + 1; nCntVertical++ )
	{
		for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
		{
			//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
			pVtx[ 0 ].color = m_color;

			//  UV���W�̎w��
			pVtx[ 0 ].tex = D3DXVECTOR2( ( float )nCntSide  , ( float )nCntVertical );

			//  �@���x�N�g���̐ݒ�
			pVtx[ 0 ].normal = D3DXVECTOR3( 0.0f , 1.0f , 0.0f );

			//  �A�h���X��i�߂�
			pVtx++;
		}
	}

	//  ���_�o�b�t�@�̃A�����b�N
	m_pVtxBuff->Unlock( );

	////  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	//m_pVtxBuff->Lock( 0 , 0 ,										//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
	//				  ( void** )&pVtx ,								//  �A�h���X�������ꂽ�������̃A�h���X
	//				  0 );											//  ���b�N�̎��

	//for( int nCntSide = 0; nCntSide < m_nDivideSide; nCntSide++ )
	//{
	//	//  ���_���W�̐ݒ�( 2D���W �E �E��� )
	//	pVtx[ nCntSide * 2 ].position = m_vertexPosition[ nCntSide * 2 ];
	//	pVtx[ nCntSide * 2 + 1 ].position = m_vertexPosition[ nCntSide * 2 + 1 ];
	//}

	////  ���_�o�b�t�@�̃A�����b�N
	//m_pVtxBuff->Unlock( );

	//WORD* pIndexBuffer;											//  �C���f�b�N�X�o�b�t�@�̉��z�A�h���X�p�|�C���^

	//m_pIndexBuff->Lock( 0 , 0 ,									//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
	//					( void** )&pIndexBuffer ,				//  �A�h���X�������ꂽ�������̃A�h���X
	//					0 );									//  ���b�N�̎��

	//for( int nCntVertical = 0; nCntVertical < m_nDivideVertical; nCntVertical++ )
	//{
	//	for( int nCntSide = 0; nCntSide < m_nDivideSide + 1; nCntSide++ )
	//	{
	//		pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 1 ) + nCntSide;
	//		pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;

	//		pIndexBuffer += 2;

	//		if( nCntSide == m_nDivideSide &&
	//			nCntVertical != ( m_nDivideVertical - 1 ) )
	//		{
	//			pIndexBuffer[ 0 ] = ( m_nDivideSide + 1 ) * nCntVertical + nCntSide;
	//			pIndexBuffer[ 1 ] = ( m_nDivideSide + 1 ) * ( nCntVertical + 2 );

	//			pIndexBuffer += 2;
	//		}	
	//	}
	//}

	////  �C���f�b�N�X�o�b�t�@�̃A�����b�N
	//m_pIndexBuff->Unlock( );
}

//--------------------------------------------------------------------------------------
//  ���_����ݒ肷��֐�
//--------------------------------------------------------------------------------------
void MeshTracing::SetVertex( void )
{
	VERTEX_3D* pVtx;												//  ���_�o�b�t�@�̉��z�A�h���X�p�|�C���^

	if( m_pVtxBuff != NULL )
	{
		//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		m_pVtxBuff->Lock( 0 , 0 ,										//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
						  ( void** )&pVtx ,								//  �A�h���X�������ꂽ�������̃A�h���X
						  0 );											//  ���b�N�̎��

		for( int nCntSide = 0; nCntSide < m_nDivideSide * 2; nCntSide++ )
		{
			//  ���_���W�̐ݒ�( 2D���W �E �E��� )
			pVtx[ nCntSide ].position = m_vertexPosition[ nCntSide ];
		}

		//  ���_�o�b�t�@�̃A�����b�N
		m_pVtxBuff->Unlock( );
	}
}

//--------------------------------------------------------------------------------------
//  �V�������_���W�̃g�b�v���W�̐ݒ�
//
//  ��1���� : ���W  --  D3DXVETOR3
//  �Ԃ�l  : �Ȃ�
//--------------------------------------------------------------------------------------
void MeshTracing::SetNewVertexPositionUp( D3DXVECTOR3 position )
{
	m_newVertexPositionUp = position;
}

//--------------------------------------------------------------------------------------
//  �V�������_���W�̃_�E�����W�̐ݒ�
//
//  ��1���� : ���W  --  D3DXVETOR3
//  �Ԃ�l  : �Ȃ�
//--------------------------------------------------------------------------------------
void MeshTracing::SetNewVertexPositionDown( D3DXVECTOR3 position )
{
	m_newVertexPositionDown = position;
}