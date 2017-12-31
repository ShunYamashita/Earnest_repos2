//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E   ( stencilShadow.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "stencilShadow.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "keyboard.h"
#include "sceneBG.h"
#include "shaderManager.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------
#define	MODEL_FILENAME		"data/MODEL/SHADOW/sphere.x"
#define	MODEL_FILENAME2		"data/MODEL/SHADOW/pillar.x"
#define	MODEL_FILENAME3		"data/MODEL/SHADOW/test.x"

#define TEXTURE_FILEPATH	"data\\TEXTURE"						//  �e�N�X�`���ւ̃t�@�C���p�X

#define MAX_MODEL_PARTS		( 20 )								//  �}�e���A���ő吔

#define SHADOW_TEXTURENAME	"data/TEXTURE/fade.png"				//  �e�N�X�`����

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
StencilShadow::StencilShadow( ) : Scene( 4 )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_scale = D3DXVECTOR3( 1.0f, 1.0f , 1.0f );
	m_rot = D3DXVECTOR3( 0.0f, 0.0f , 0.0f );
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�N���X�̃R���X�g���N�^
//--------------------------------------------------------------------------------------
StencilShadow::StencilShadow( D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	m_pMeshModel = NULL;
	m_pBuffMatModel = NULL;
	m_nNumMatModel = 0;

	m_position = position;
	m_rot = rot;
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̃f�X�g���N�^
//--------------------------------------------------------------------------------------
StencilShadow::~StencilShadow( )
{

}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̏���������
//--------------------------------------------------------------------------------------
HRESULT StencilShadow::Init( void )
{
	D3DXMATERIAL*	pMat = NULL;									//  �}�e���A��
	char			aTextureFileName[ 128 ] = { };
	int				nIndex = 0;

	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �f�o�C�X���̎擾
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  �e�N�X�`���N���X�̎擾
	Texture* pTexture = SceneManager::GetTexture( );

	//  ���_�o�b�t�@�̍쐬
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_2D ) * NUM_VERTEX ,		//  �쐬���������_�o�b�t�@�̃T�C�Y
											 D3DUSAGE_WRITEONLY ,					//  �g�p���@
											 0 ,									//  
											 D3DPOOL_MANAGED ,						//  �������Ǘ����@( MANAGED �� ���C�� )
											 &m_shadowVtxBuff ,						//  �o�b�t�@
											 NULL ) ) )
	{
		MessageBox( NULL , "���_�o�b�t�@�C���^�[�t�F�[�X�𐳂����擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

		return E_FAIL;
	}

	VERTEX_2D* pVtx = NULL;				//  ���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 LeftTop;				//  ������W
	D3DXVECTOR3 RightTop;				//  �E����W
	D3DXVECTOR3 LeftButtom;				//  �������W
	D3DXVECTOR3 RightButtom;			//  �E�����W

	//  ������W�̎Z�o
	LeftTop = D3DXVECTOR3( 0.0f ,
						   0.0f ,
						   0.0f );

	//  �E����W�̎Z�o
	RightTop = D3DXVECTOR3( SCREEN_WIDTH ,
							0.0f ,
							0.0f );

	//  �������W�̎Z�o
	LeftButtom = D3DXVECTOR3( 0.0f ,
							  SCREEN_HEIGHT ,
							  0.0f );

	//  �E�����W�̎Z�o
	RightButtom = D3DXVECTOR3( SCREEN_WIDTH ,
							   SCREEN_HEIGHT ,
							   0.0f );

	if( m_shadowVtxBuff != NULL )
	{
		//  ���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
		m_shadowVtxBuff->Lock( 0 , 0 ,								//  ���擪�ƁA�T�C�Y( 0 , 0 �őS�� )
						  ( void** )&pVtx ,							//  �A�h���X�������ꂽ�������̃A�h���X
						  0 );										//  ���b�N�̎��

		//  ���_���W�̐ݒ�( 2D���W �E �E��� )
		pVtx[ 0 ].position = LeftTop;
		pVtx[ 1 ].position = RightTop;
		pVtx[ 2 ].position = LeftButtom;
		pVtx[ 3 ].position = RightButtom;

		//  rhw�̐ݒ�( �K��1.0f )
		pVtx[ 0 ].rhw =
		pVtx[ 1 ].rhw =
		pVtx[ 2 ].rhw =
		pVtx[ 3 ].rhw = 1.0f;

		//  ���_�F�̐ݒ�( 0 �` 255 �̐����l )
		pVtx[ 0 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );
		pVtx[ 1 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );
		pVtx[ 2 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );
		pVtx[ 3 ].color = D3DXCOLOR( 0.0f , 0.0f , 0.0f , 0.5f );

		//  UV���W�̎w��
		pVtx[ 0 ].texcoord = D3DXVECTOR2( 0.0f , 0.0f );
		pVtx[ 1 ].texcoord = D3DXVECTOR2( 1.0f , 0.0f );
		pVtx[ 2 ].texcoord = D3DXVECTOR2( 0.0f , 1.0f );
		pVtx[ 3 ].texcoord = D3DXVECTOR2( 1.0f , 1.0f );

		//  ���_�o�b�t�@�̃A�����b�N
		m_shadowVtxBuff->Unlock( );
	}

	if( m_type == StencilShadow::TYPE::SPHERE )
	{
		//  x�t�@�C���̓ǂݍ���
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME ,				//  �t�@�C����
		 							   D3DXMESH_MANAGED ,			//  �I�v�V����
									   pDevice ,					//  �f�o�C�X
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  �}�e���A�����
									   NULL ,						//  
									   &m_nNumMatModel ,			//  �}�e���A����
									   &m_pMeshModel ) ) )			//  ���b�V�����
		{
			MessageBox( NULL , "x�t�@�C������肭�擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == StencilShadow::TYPE::PILLAR )
	{
		//  x�t�@�C���̓ǂݍ���
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME2 ,			//  �t�@�C����
		 							   D3DXMESH_MANAGED ,			//  �I�v�V����
									   pDevice ,					//  �f�o�C�X
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  �}�e���A�����
									   NULL ,						//  
									   &m_nNumMatModel ,			//  �}�e���A����
									   &m_pMeshModel ) ) )			//  ���b�V�����
		{
			MessageBox( NULL , "x�t�@�C������肭�擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

			return E_FAIL;
		}
	}
	else if( m_type == StencilShadow::TYPE::THIEF )
	{
		//  x�t�@�C���̓ǂݍ���
		if( FAILED( D3DXLoadMeshFromX( MODEL_FILENAME3 ,			//  �t�@�C����
		 							   D3DXMESH_MANAGED ,			//  �I�v�V����
									   pDevice ,					//  �f�o�C�X
									   NULL ,						//  
									   &m_pBuffMatModel ,			//  �}�e���A�����
									   NULL ,						//  
									   &m_nNumMatModel ,			//  �}�e���A����
									   &m_pMeshModel ) ) )			//  ���b�V�����
		{
			MessageBox( NULL , "x�t�@�C������肭�擾�o���܂���ł����B" , "�G���[���b�Z�[�W" , MB_OK );

			return E_FAIL;
		}
	}

	//  �}�e���A�����̐擪�A�h���X���擾
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	//  �}�e���A���̐����̃��[�v
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  �e�N�X�`���|�C���^������ꍇ
		if( pMat[ nCntMaterial ].pTextureFilename != NULL )
		{
			//  �e�N�X�`���t�@�C���p�X�p�̕�����
			char aTextureFilePath[ 128 ] = { TEXTURE_FILEPATH };

			//  ���������̃��[�v
			for( int nCntLength = strlen( pMat[ nCntMaterial ].pTextureFilename ) - 1; nCntLength >= 0 ; nCntLength-- )
			{
				//  �e�N�X�`�����̎��o��
				aTextureFileName[ nIndex ] = pMat[ nCntMaterial ].pTextureFilename[ nCntLength ];

				nIndex++;

				//  \������ƃ��[�v�𔲂��o��
				if( pMat[ nCntMaterial ].pTextureFilename[ nCntLength ] == '\\' )
				{
					break;
				}
			}

			//  \0�̊i�[
			aTextureFileName[ nIndex ] = '\0';

			//  �C���f�b�N�X�ԍ��̏�����
			nIndex = 0;

			char aWork;											//  �P�������̊i�[�p
			int	 nLength = strlen( aTextureFileName );			//  �����̒���

			//  ���������̃��[�v
			for( int nCntLength = 0; nCntLength < nLength / 2; nCntLength++ )
			{
				//  �e�N�X�`�����̔��]
				aWork = aTextureFileName[ nCntLength ];
				aTextureFileName[ nCntLength ] = aTextureFileName[ nLength - nCntLength - 1 ];
				aTextureFileName[ nLength - nCntLength - 1 ] = aWork;
			}

			//  �e�N�X�`�������e�N�X�`���p�X����
			strcat( aTextureFilePath , aTextureFileName );

			//  �e�N�X�`�����̃R�s�[
			strcpy( m_aTextureName[ nCntMaterial ] , aTextureFilePath );

			//  �e�N�X�`���̓o�^
			pTexture->SetTextureImage( aTextureFilePath );
		}
	}

	//  �e�N�X�`���̓o�^
	pTexture->SetTextureImage( SHADOW_TEXTURENAME );

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̏I������
//--------------------------------------------------------------------------------------
void StencilShadow::Uninit( void )
{
	//  ���b�V�����̉��
	SAFE_RELEASE( m_pMeshModel );

	//  �}�e���A�����̉��
	SAFE_RELEASE( m_pBuffMatModel );
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̍X�V����
//--------------------------------------------------------------------------------------
void StencilShadow::Update( void )
{
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̕`�揈��
//--------------------------------------------------------------------------------------
void StencilShadow::Draw( void )
{
	//  �e�̕`��
	//DrawShadow( );
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̉e�̕`�揈��
//--------------------------------------------------------------------------------------
void StencilShadow::DrawShadow( void )
{
	LPDIRECT3DDEVICE9 pDevice;

	//  �f�o�C�X���̎擾
	pDevice = SceneManager::GetRenderer( )->GetDevice( );

	D3DMATERIAL9	matDef;							//  �}�e���A���̏������
	D3DXMATERIAL*	pMat = NULL;					//  �}�e���A��

	D3DXMATRIX mtxWorld;							//  ���[���h�s��
	D3DXMATRIX mtxTrans;							//  ���s�ړ��s��
	D3DXMATRIX mtxScale;							//  �X�P�[���s��
	D3DXMATRIX mtxRot;								//  ��]�s��

	D3DXMATRIX mtxView;								//  �r���[���W�ϊ��s��
	D3DXMATRIX mtxProj;								//  �v���W�F�N�V�������W�s��

	//  �e�N�X�`���N���X�̎擾
	Texture* pTexture = SceneManager::GetTexture( );

	//  �s���P�ʍs��ɕϊ�
	D3DXMatrixIdentity( &mtxWorld );

	//  �g��s��̍쐬
	D3DXMatrixScaling( &mtxScale , m_scale.x , m_scale.y , m_scale.z );

	//  �g��s��̊|���Z
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

	D3DXMatrixRotationYawPitchRoll( &mtxRot , m_rot.y , m_rot.x , m_rot.z );

	//  ��]�s��̊|���Z
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxRot );

	//  ���s�ړ��s��̍쐬
	D3DXMatrixTranslation( &mtxTrans , m_position.x , m_position.y , m_position.z );

	//  ���s�ړ��s��̊|���Z
	D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

	//  ���[���h���W�ϊ�
	pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

	//  �}�e���A���̏������̎擾
	pDevice->GetMaterial( &matDef );

	//  �}�e���A�����̐擪�A�h���X���擾
	pMat = ( D3DXMATERIAL* )m_pBuffMatModel->GetBufferPointer( );

	pDevice->SetRenderState( D3DRS_STENCILENABLE , TRUE );
	pDevice->SetRenderState( D3DRS_ZWRITEENABLE , FALSE );
	pDevice->SetRenderState( D3DRS_COLORWRITEENABLE , 0 );
	pDevice->SetRenderState( D3DRS_STENCILFUNC , D3DCMP_ALWAYS );
	pDevice->SetRenderState( D3DRS_STENCILREF , 1 );
	pDevice->SetRenderState( D3DRS_STENCILPASS , D3DSTENCILOP_INCR );
	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );

	//  �}�e���A���̐����̃��[�v
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  �g�U���̐ݒ�
		pMat[ nCntMaterial ].MatD3D.Diffuse = m_color;

		//  �����̐ݒ�
		pMat[ nCntMaterial ].MatD3D.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

		//  �}�e���A���̐ݒ�
		pDevice->SetMaterial( &pMat[ nCntMaterial ].MatD3D );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			//  �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , NULL );
		}
		else
		{
			//  �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );
		}

		//  ���b�V���̕`��
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  �ގ��̐ݒ�
		pDevice->SetMaterial( &matDef );
	}

	pDevice->SetRenderState( D3DRS_STENCILPASS , D3DSTENCILOP_DECR );
	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CW );

	//  �}�e���A���̐����̃��[�v
	for( int nCntMaterial = 0; nCntMaterial < ( int )m_nNumMatModel; nCntMaterial++ )
	{
		//  �g�U���̐ݒ�
		pMat[ nCntMaterial ].MatD3D.Diffuse = m_color;

		//  �����̐ݒ�
		pMat[ nCntMaterial ].MatD3D.Ambient = D3DXCOLOR( 0.6f , 0.6f , 0.6f , 1.0f );

		//  �}�e���A���̐ݒ�
		pDevice->SetMaterial( &pMat[ nCntMaterial ].MatD3D );

		if( pMat[ nCntMaterial ].pTextureFilename == NULL )
		{
			//  �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , NULL );
		}
		else
		{
			//  �e�N�X�`���̐ݒ�
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( m_aTextureName[ nCntMaterial ] ) );
		}

		//  ���b�V���̕`��
		m_pMeshModel->DrawSubset( nCntMaterial );

		//  �ގ��̐ݒ�
		pDevice->SetMaterial( &matDef );
	}

	pDevice->SetRenderState( D3DRS_CULLMODE , D3DCULL_CCW );
	pDevice->SetRenderState( D3DRS_COLORWRITEENABLE , 0xf );
	pDevice->SetRenderState( D3DRS_STENCILFUNC , D3DCMP_EQUAL );

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource( 0 ,								//  �p�C�v���C���ԍ�
							  m_shadowVtxBuff ,					//  ���_�o�b�t�@�̃A�h���X
							  0 ,								//  �I�t�Z�b�g( byte )
							  sizeof( VERTEX_2D ) );			//  ����̒��_�f�[�^�̃T�C�Y( �X�g���C�h )

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF( FVF_VERTEX_2D );

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture( 0 , pTexture->GetTextureImage( SHADOW_TEXTURENAME ) ); 

	// �|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP ,				//  �v���~�e�B�u�̎��
							0 ,									//  �I�t�Z�b�g( ���Ԗڂ̒��_����`�悷�邩�I�ׂ� )
							NUM_POLYGON );						//  �v���~�e�B�u��

	pDevice->SetRenderState( D3DRS_ZWRITEENABLE , TRUE );
	pDevice->SetRenderState( D3DRS_STENCILENABLE , FALSE );
}

//--------------------------------------------------------------------------------------
//  �X�e���V���V���h�E�̑傫���ݒ������֐�
//--------------------------------------------------------------------------------------
void StencilShadow::SetScale( D3DXVECTOR3 scale )
{
	m_scale = scale;
}

//--------------------------------------------------------------------------------------
//  x�|���S���̑傫�����擾����֐�
//--------------------------------------------------------------------------------------
D3DXVECTOR3 StencilShadow::GetScale( void )
{
	return m_scale;
}

//--------------------------------------------------------------------------------------
//  �C���X�^���X����������֐�
//--------------------------------------------------------------------------------------
StencilShadow* StencilShadow::Create( TYPE type , D3DXVECTOR3 position , D3DXVECTOR3 rot , D3DXVECTOR3 scale )
{
	StencilShadow *pStencilShadow;

	//  �C���X�^���X����
	pStencilShadow = new StencilShadow;

	//  ��ނ̑��
	pStencilShadow->m_type = type;

	//  ���W�̑��
	pStencilShadow->m_position = position;

	//  ��]�p�̑��
	pStencilShadow->m_rot = rot;

	//  �傫���{���̑��
	pStencilShadow->m_scale = scale;

	//  ������
	pStencilShadow->Init( );

	return pStencilShadow;
}