//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[( �������C�g )   ( shader3DRimLightTexture.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �w�b�_�[�t�@�C��
//--------------------------------------------------------------------------------------
#include "shader3DRimLightTexture.h"
#include "manager.h"
#include "renderer.h"

//--------------------------------------------------------------------------------------
//  �}�N����`
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  �C���X�^���X����
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[( �������C�g )�̏���������
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::Init( void )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	D3DVERTEXELEMENT9 element[ ] = 
	{  
		//  ����( 1 : �p�C�v���C���ԍ� , 2 : �I�t�Z�b�g , 3 : �f�[�^�T�C�Y( �^ ) , 4 : , 5 : �g�p�p�r , 6 : , ( 5�`6�̓Z�}���e�B�N�X ) )
		//  ( �����ōl�����f�[�^�𑗂�ꍇ��TEXCOORD�ő���悤�� )
		{ 0 , 0 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_POSITION , 0 } ,
		{ 0 , sizeof( float ) * 3 , D3DDECLTYPE_FLOAT3 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_NORMAL, 0 } ,
		{ 0 , sizeof( float ) * 6 , D3DDECLTYPE_D3DCOLOR , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_COLOR , 0 } ,
		{ 0 , sizeof( float ) * 6 + sizeof( D3DCOLOR ) , D3DDECLTYPE_FLOAT2 , D3DDECLMETHOD_DEFAULT , D3DDECLUSAGE_TEXCOORD , 0 } ,		
		D3DDECL_END( )  //  �I��
	};

	//  ���_�錾�i�[�C���^�[�t�F�[�X����
	device->CreateVertexDeclaration( element , &m_vertexDeclaration );

	/*------------------------------------------------------------
		�o�[�e�b�N�X�V�F�[�_�[�̐���
	------------------------------------------------------------*/

	HRESULT hr; 

	LPD3DXBUFFER error = NULL;				//  �G���[�p
	LPD3DXBUFFER codeBinary = NULL;			//  �o�C�i���[( ���������m�ۂ��Ċi�[�p )

	hr = D3DXCompileShaderFromFile( "data/HLSL/3D/rimLightTextureVS.hlsl" ,				//  �t�@�C����
									NULL ,												//  
									NULL ,												//  
									"main" ,											//  �G���g���[�֐���( ��ԏ��߂ɍs����֐� )
									"vs_3_0" ,											//  �V�F�[�_���f����
									0 ,													//  
									&codeBinary ,										//  �R���p�C����̃o�C�i���R�[�h�i�[�ꏊ
									&error ,											//  �G���[���b�Z�[�W( �R���p�C�����ς߂�NULL , �G���[�Ȃ烁�b�Z�[�W )
									&m_vertexShaderConstantTable );						//  

	//  �G���[�`�F�b�N
	if( FAILED( hr ) )
	{
		MessageBox( NULL , ( char* )error->GetBufferPointer( ) , "D3DXCompileShaderFromFile�ł̃G���[" , MB_OK );
		//OutputDebugStringA( "Error!!" );

		//  �G���[���b�Z�[�W�̉��
		error->Release( );

		return;
	}

	//  �o�[�e�b�N�X�V�F�[�_�[�̐���
	hr = device->CreateVertexShader( ( LPDWORD )codeBinary->GetBufferPointer( ) ,		//  �R�[�h�o�C�i���[�f�[�^�[
												&m_vertexShader );						//  �o�[�e�b�N�X�V�F�[�_�[�̊Ǘ��҂ɑ��

	//  �G���[�`�F�b�N
	if( FAILED( hr ) )
	{
		MessageBox( NULL , "�o�[�e�b�N�X�V�F�[�_�[����肭�����o���܂���ł����B" , "CreateVertexShader�ł̃G���[" , MB_OK );

		return;
	}

	//  �G���[�R�[�h�̃��������
	if( error != NULL )
	{
		error->Release( );
	}

	//  �R�[�h�o�C�i���[�̃��������
	if( codeBinary != NULL )
	{
		codeBinary->Release( );
	}

	/*------------------------------------------------------------
		�s�N�Z���V�F�[�_�[�̐���
	------------------------------------------------------------*/

	error = NULL;				//  �G���[�p
	codeBinary = NULL;			//  �o�C�i���[( ���������m�ۂ��Ċi�[�p )

	hr = D3DXCompileShaderFromFile( "data/HLSL/3D/rimLightTexturePS.hlsl" ,				//  �t�@�C����
									NULL ,												//  
									NULL ,												//  
									"main" ,											//  �G���g���[�֐���( ��ԏ��߂ɍs����֐� )
									"ps_3_0" ,											//  �V�F�[�_���f����
									0 ,													//  
									&codeBinary ,										//  �R���p�C����̃o�C�i���R�[�h�i�[�ꏊ
									&error ,											//  �G���[���b�Z�[�W( �R���p�C�����ς߂�NULL , �G���[�Ȃ烁�b�Z�[�W )
									&m_pixelShaderConstantTable );						//  

	//  �G���[�`�F�b�N
	if( FAILED( hr ) )
	{
		MessageBox( NULL , ( char* )error->GetBufferPointer( ) , "D3DXCompileShaderFromFile�ł̃G���[" , MB_OK );
		//OutputDebugStringA( "Error!!" );

		//  �G���[���b�Z�[�W�̉��
		error->Release( );

		return;
	}

	//  �s�N�Z���V�F�[�_�[�̐���
	hr = device->CreatePixelShader( ( LPDWORD )codeBinary->GetBufferPointer( ) ,		//  �R�[�h�o�C�i���[�f�[�^�[
											   &m_pixelShader );								//  �s�N�Z���V�F�[�_�[�̊Ǘ��҂ɑ��

	//  �G���[�`�F�b�N
	if( FAILED( hr ) )
	{
		MessageBox( NULL , "�s�N�Z���V�F�[�_�[����肭�����o���܂���ł����B" , "CreatePixelShader�ł̃G���[" , MB_OK );

		return;
	}

	//  �G���[�R�[�h�̃��������
	if( error != NULL )
	{
		error->Release( );
	}

	//  �R�[�h�o�C�i���[�̃��������
	if( codeBinary != NULL )
	{
		codeBinary->Release( );
	}
}

//--------------------------------------------------------------------------------------
//  3D�V�F�[�_�[( �������C�g )�̏I������
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::Uninit( void )
{
	//  �V�F�[�_�[�̏I��
	Shader::Uninit( );
}

//--------------------------------------------------------------------------------------
//  ���_�t�H�[�}�b�g�̒�`
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetVertexDeclaration( void )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device->SetVertexDeclaration( m_vertexDeclaration );
}

//--------------------------------------------------------------------------------------
//  ���_�V�F�[�_�[�̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetVertexShader( void )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShader != nullptr )
	{
		device->SetVertexShader( m_vertexShader );
	}
}

//--------------------------------------------------------------------------------------
//  �s�N�Z���V�F�[�_�[�̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetPixelShader( void )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShader != nullptr )
	{
		device->SetPixelShader( m_pixelShader );
	}
}


//--------------------------------------------------------------------------------------
//  ���[���h�s��̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetWorldMatrix( const D3DXMATRIX &worldMatrix )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "worldMatrix" , &worldMatrix );
	}
}

//--------------------------------------------------------------------------------------
//  �r���[�s��̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetViewMatrix( const D3DXMATRIX &viewMatrix )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "viewMatrix" , &viewMatrix );
	}
}

//--------------------------------------------------------------------------------------
//  �v���W�F�N�V�����s��̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetProjectionMatrix( const D3DXMATRIX &projectionMatrix )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "projectionMatrix" , &projectionMatrix );
	}
}

//--------------------------------------------------------------------------------------
//  ���[���h�t�s��̓]�u�s��̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetWorldInverseTransposeMatrix( const D3DXMATRIX &matrix )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_vertexShaderConstantTable != nullptr )
	{
		m_vertexShaderConstantTable->SetMatrix( device , "worldInverseTransposeMatrix" , &matrix );
	}
}

//--------------------------------------------------------------------------------------
//  �J�����̃��[���h���W�̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetWorldPositionEye( const D3DXVECTOR3 &position )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetValue( device , "worldPosEye" , &position , sizeof( position ) );
	}
}

//--------------------------------------------------------------------------------------
//  ���C�g�̃��[���h���W�n�̕����x�N�g���̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetLightDirectWorld( const D3DXVECTOR3 &lightDirectWorld )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetValue( device , "lightDirectWorld" , &lightDirectWorld , sizeof( lightDirectWorld ) );
	}
}

//--------------------------------------------------------------------------------------
//  �X�y�L�����[�̓x�����̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetSpecularPower( const float &specularPower )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetFloat( device , "specularPower" , specularPower );
	}
}

//--------------------------------------------------------------------------------------
//  �}�e���A���g�U���̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetMaterialDiffuseColor( const D3DXCOLOR &materialDiffuseColor )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	if( m_pixelShaderConstantTable != nullptr )
	{
		m_pixelShaderConstantTable->SetValue( device , "materialDiffuseColor" , &materialDiffuseColor , sizeof( materialDiffuseColor ) );
	}
}

//--------------------------------------------------------------------------------------
//  �V�F�[�_�[�ɕK�v�ȏ��̐ݒ�
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::SetShaderInfo( const D3DXMATRIX &worldMatrix ,
											 const D3DXMATRIX &viewMatrix ,
											 const D3DXMATRIX &projectionMatrix ,
											 const D3DXMATRIX &worldInverseTransposeMatrix ,
											 const D3DXVECTOR3 &cameraPositionEye ,
											 const D3DXVECTOR3 &lightDirectWorld ,
											 const float &specularPower ,
											 const D3DXCOLOR &materialDiffuseColor  )
{
	SetWorldMatrix( worldMatrix );
	SetViewMatrix( viewMatrix );
	SetProjectionMatrix( projectionMatrix );
	SetWorldInverseTransposeMatrix( worldInverseTransposeMatrix );
	SetWorldPositionEye( cameraPositionEye );
	SetLightDirectWorld( lightDirectWorld );
	SetSpecularPower( specularPower );
	SetMaterialDiffuseColor( materialDiffuseColor );
}

//--------------------------------------------------------------------------------------
//  �V�F�[�_�[�`��J�n
//--------------------------------------------------------------------------------------
void Shader3DRimLightTexture::DrawBegin( void )
{
	//  �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 device = SceneManager::GetRenderer( )->GetDevice( );

	//  ���_�V�F�[�_�[�̐ݒ�
	if( m_vertexShader != nullptr )
	{
		device->SetVertexShader( m_vertexShader );
	}

	//  �s�N�Z���V�F�[�_�[�̐ݒ�
	if( m_pixelShader != nullptr )
	{
		device->SetPixelShader( m_pixelShader );
	}

	// ���_�t�H�[�}�b�g�̐ݒ�
	device->SetVertexDeclaration( m_vertexDeclaration );
}