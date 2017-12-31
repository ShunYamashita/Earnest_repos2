//--------------------------------------------------------------------------------------
//  レンダリング処理   ( sceneFBX.cpp )
//
//  Author : SHUN YAMASHITA
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//  ヘッダーファイル
//--------------------------------------------------------------------------------------
#include "sceneFBX.h"
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include <algorithm>

FbxTime		SceneFBX::myNode::m_startTime = 0;
FbxTime		SceneFBX::myNode::m_endTime = 0;
int			SceneFBX::myNode::m_currentFrame = 0;
int			SceneFBX::myNode::m_allTime = 0;
bool		SceneFBX::myNode::m_makeVertrx = false;

//--------------------------------------------------------------------------------------
//  コンストラクタ
//--------------------------------------------------------------------------------------
SceneFBX::SceneFBX( )
{
}

//--------------------------------------------------------------------------------------
//  デスストラクタ
//--------------------------------------------------------------------------------------
SceneFBX::~SceneFBX( )
{

}

//--------------------------------------------------------------------------------------
//  初期化処理
//--------------------------------------------------------------------------------------
HRESULT SceneFBX::Init( void )
{
	//  FBXモデル読み込み実験用プログラム
	FbxManager* lSdkManager = FbxManager::Create(); 
 
	// Create the IO settings object. 
	FbxIOSettings *ios = FbxIOSettings::Create(lSdkManager, IOSROOT); lSdkManager->SetIOSettings(ios); 
 
	// Create an importer using the SDK manager. 
	FbxImporter* lImporter = FbxImporter::Create(lSdkManager,""); 
 
	// Use the first argument as the filename for the importer. 
	if(!lImporter->Initialize("data/dude/dude.fbx", -1, lSdkManager->GetIOSettings())) 
	{      
		char buf[256];     
		sprintf(buf, "Call to FbxImporter::Initialize() failed.\nError returned: %s\n\n" ,lImporter->GetStatus().GetErrorString());     
		MessageBox(NULL, buf, "error", MB_OK);     
		return E_FAIL; 
	}  
 
	// Create a new scene so that it can be populated by the imported file. 
	FbxScene* lScene = FbxScene::Create(lSdkManager,"myScene"); 
 
	// Import the contents of the file into the scene. 
	lImporter->Import(lScene); 
 
	FbxGeometryConverter converter(lSdkManager); 
	
	// 三角ポリゴン化 
	converter.Triangulate(lScene, true); 
	
	// 縮退メッシュを削除 
	converter.RemoveBadPolygonsFromMeshes(lScene); 
	
	// マテリアルごとにメッシュ分離 
	converter.SplitMeshesPerMaterial(lScene, true); 

	//  アニメーション個数の取得
	lImporter->GetAnimStackCount( );

	//  アニメーション一覧の取得
	FbxArray< FbxString* > animationNames;
	lScene->FillAnimStackNameArray( animationNames );

	//  アニメーション情報の取得
	FbxTakeInfo* pTakeInfo = lScene->GetTakeInfo( animationNames[ 0 ]->Buffer( ) );

	//  アニメーションの開始・終了時間の取得
	FbxTime startTime = pTakeInfo->mLocalTimeSpan.GetStart( );
	FbxTime endTime = pTakeInfo->mLocalTimeSpan.GetStop( );
 
	// 再帰的にノードを漁る 
	m_pTopNode = SceneFBX::myNode::recursiveNode(lSdkManager, lScene->GetRootNode(), true , startTime , endTime );

	lImporter->Destroy(); 
	lScene->Destroy(); 
	lSdkManager->Destroy(); 

	return S_OK;
}

//--------------------------------------------------------------------------------------
//  終了処理
//--------------------------------------------------------------------------------------
void SceneFBX::Uninit( void )
{
	Scene::Release( );
}

//--------------------------------------------------------------------------------------
//  更新処理
//--------------------------------------------------------------------------------------
void SceneFBX::Update( void )
{
	myNode::m_currentFrame += 1;
}

//--------------------------------------------------------------------------------------
//  描画処理
//--------------------------------------------------------------------------------------
void SceneFBX::Draw( void )
{  
	//  再帰的に描画
    m_pTopNode->recursiveDraw(); 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に描画処理
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::recursiveDraw( void )
{ 
    for( auto itm = meshes.begin(); itm != meshes.end(); ++itm ) 
	{  
		if( itm->matrixes.empty() ) 
		{
			//// 骨なし（つまり剛体の塊）   
			//for( size_t i = 0; i < itm->positionIndices.size(); i++ )
			//{ 
			//	if( !itm->normals.empty()   ) 
			//		glNormal3fv(itm->normals[itm->normalIndices[i]]);                 
			//		
			//	if( !itm->texcoords.empty() ) 
			//		glTexCoord2fv(itm->texcoords[itm->texcoordIndices[i]]);           
			//	
			//	glVertex3fv(itm->points[itm->positionIndices[i]].positions);            
			//}       
		}      
		else 
		{
			// 骨あり（つまりワンスキンなど） 
 
			// 頂点の座標変換             
			std::vector<D3DXVECTOR3> positions;            
			positions.reserve(itm->points.size()); 
 
			D3DXMATRIX mtx;             
				
			for( auto it = itm->points.begin(); it != itm->points.end(); ++it ) 
			{                
				ZeroMemory(&mtx, sizeof(D3DXMATRIX));   

				FbxTime allFrame = ( m_endTime - m_startTime );
				int nAllFrame = allFrame.GetFrameCount( );
					
				for( auto itb = it->bornRefarences.begin(); itb != it->bornRefarences.end(); ++itb ) 
				{                    
					//mtx += itm->matrixes[itb->index] * itb->weight;
					mtx += itm->matrixes[itb->index][ m_currentFrame % 73 ] * itb->weight;
				}

				D3DXVECTOR3 pos;                 
				D3DXVec3TransformCoord(&pos, &it->positions, &mtx);                
				positions.push_back(pos);             
			} 

			if( m_makeVertrx == false )
			{
				MakeVertex( itm->positionIndices.size( ) );

				m_makeVertrx = true;
			}

			VERTEX_3D* pVtx = NULL;				//  頂点バッファのポインタ

			if( m_pVtxBuff != NULL )
			{
				//  頂点バッファをロックして、仮想アドレスを取得する
				m_pVtxBuff->Lock( 0 , 0 ,									//  取る先頭と、サイズ( 0 , 0 で全部 )
								  ( void** )&pVtx ,							//  アドレスが書かれたメモ帳のアドレス
								  0 );										//  ロックの種類

				for( size_t i = 0; i < itm->positionIndices.size( ); i++ ) 
				{             
					//  頂点座標の設定( 3D座標 ・ 右回り )
					pVtx[ i ].position = positions[ itm->positionIndices[ i ] ];

					//  法線の指定
					pVtx[ i ].normal = itm->normals[itm->normalIndices[ i ] ];

					//  頂点色の設定( 0 ～ 255 の整数値 )
					pVtx[ i ].color = D3DXCOLOR( 1.0f , 1.0f , 1.0f , 1.0f );

					//  UV座標の指定
					pVtx[ i ].texcoord = itm->texcoords[ itm->texcoordIndices[ i ] ];
				}

				//  頂点バッファのアンロック
				m_pVtxBuff->Unlock( );     
			}

			LPDIRECT3DDEVICE9 pDevice;

			D3DXMATRIX mtxWorld;							//  ワールド行列
			D3DXMATRIX mtxTrans;							//  平行移動行列
			D3DXMATRIX mtxScale;							//  拡大行列
			D3DXMATRIX mtxRot;								//  ビュー座標変換行列

			//  デバイス情報の取得
			pDevice = SceneManager::GetRenderer( )->GetDevice( );

			//  テクスチャクラスの取得
			Texture* pTexture = SceneManager::GetTexture( );

			D3DXMatrixIdentity( &mtxWorld );
			D3DXMatrixIdentity( &mtxScale );
			D3DXMatrixIdentity( &mtxRot );

			//  拡大行列の作成
			D3DXMatrixScaling( &mtxScale , 0.3f , 0.3f , 0.3f );

			//  拡大行列の掛け算
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxScale );

			//  平行移動行列の作成
			D3DXMatrixTranslation( &mtxTrans , 0.0f , 0.0f , 100.0f );

			//  平行移動行列の掛け算
			D3DXMatrixMultiply( &mtxWorld , &mtxWorld , &mtxTrans );

			//  ワールド座標変換
			pDevice->SetTransform( D3DTS_WORLD , &mtxWorld );

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource( 0 ,								//  パイプライン番号
									  m_pVtxBuff ,						//  頂点バッファのアドレス
									  0 ,								//  オフセット( byte )
									  sizeof( VERTEX_3D ) );			//  一個分の頂点データのサイズ( ストライド )

			// 頂点フォーマットの設定
			pDevice->SetFVF( FVF_VERTEX_3D );

			std::string fileName = textures[ itm->materialIndex ];

			// テクスチャの設定
			pDevice->SetTexture( 0 , pTexture->GetTextureImage( textures[ itm->materialIndex ].c_str( ) ) ); 

			// ポリゴンの描画
			pDevice->DrawPrimitive( D3DPT_TRIANGLELIST ,					//  プリミティブの種類
									0 ,										//  オフセット( 何番目の頂点から描画するか選べる )
									itm->positionIndices.size( ) / 3 );		//  プリミティブ数
		}
	}

	std::for_each(children.begin(), children.end(), [](myNode* p){ p->recursiveDraw(); }); 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
SceneFBX::myNode* SceneFBX::myNode::recursiveNode( FbxManager* pManager, FbxNode* pNode, bool bTexcoordVReverse, FbxTime startTime, FbxTime endTime )
{
    myNode* p = NULL; 

    if(pNode) 
	{ 
        p = new myNode;         
		p->name = pNode->GetName();         
		p->translation.x = static_cast<float>(pNode->LclTranslation.Get()[0]);         
		p->translation.y = static_cast<float>(pNode->LclTranslation.Get()[1]);         
		p->translation.z = static_cast<float>(pNode->LclTranslation.Get()[2]);         
		p->rotation.x    = static_cast<float>(pNode->LclRotation.Get()[0]);         
		p->rotation.y    = static_cast<float>(pNode->LclRotation.Get()[1]);         
		p->rotation.z    = static_cast<float>(pNode->LclRotation.Get()[2]);         
		p->scaling.x     = static_cast<float>(pNode->LclScaling.Get()[0]);         
		p->scaling.y     = static_cast<float>(pNode->LclScaling.Get()[1]);         
		p->scaling.z     = static_cast<float>(pNode->LclScaling.Get()[2]); 

        for(int i = 0; i < pNode->GetNodeAttributeCount(); i++) 
		{ 
            FbxNodeAttribute::EType type = pNode->GetNodeAttributeByIndex(i)->GetAttributeType();            
			p->attributeNames.push_back(GetAttributeTypeName(type)); 
 
            if( type == FbxNodeAttribute::eMesh ) 
			{ 
				// マテリアル情報の解析（マテリアルリスト化）                 
				p->analyzeMaterial(pNode); 
 
                p->meshes.push_back(Mesh()); 
 
                // メッシュ情報の取得                 
				FbxMesh* pMesh = FbxCast<FbxMesh>(pNode->GetNodeAttributeByIndex(i)); 
 
                // 頂点座標解析                 
				p->analyzePosition(pMesh);                 
				
				// 法線解析                 
				p->analyzeNormal(pMesh);                 
				
				// UV解析                 
				p->analyzeTexcoord(pMesh, bTexcoordVReverse);   

				// マテリアル解析（参照情報の取得）                 
				p->analyzeMaterial(pMesh); 
				
				m_startTime = startTime;
				m_endTime = endTime;

				// ボーン解析 
				p->analyzeCluster(pMesh);             
			}             
			else 
			{                 
				// メッシュではないアトリビュート                 
				//MessageBox(NULL, GetAttributeTypeName(type).c_str(), "アトリビュート", MB_OK);              
			}         
		} 
 
        for(int i = 0; i < pNode->GetChildCount(); i++) 
		{             
			p->children.push_back(recursiveNode(pManager, pNode->GetChild(i), bTexcoordVReverse , startTime , endTime )); 
		}     
	} 
 
    return p; 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::analyzePosition(FbxMesh* pMesh) 
{     
	// コントロールポイント数の取得     
	int controlPointsCount = pMesh->GetControlPointsCount(); 
 
    // 頂点座標用コンテナの領域予約     
	meshes.back().points.reserve(controlPointsCount); 
 
    // 頂点データの取得     
	FbxVector4* pP = pMesh->GetControlPoints();     
	
	for( int i = 0; i < pMesh->GetControlPointsCount(); i++ ) 
	{         
		meshes.back().points.push_back( D3DXVECTOR3( static_cast<float>(pP[i][0]) ,                          
													 static_cast<float>(pP[i][1]) ,                          
													 static_cast<float>(pP[i][2]) ) );     
	} 
 
    /* 頂点インデックスの取得 */ 
 
    // インデックス数を取得     
	int polygonVertexCount = pMesh->GetPolygonVertexCount(); 
 
    // 頂点座標インデックス用コンテナの領域予約     
	meshes.back().positionIndices.reserve(polygonVertexCount);          
	
	// インデックスバッファの取得     
	for( int i = 0; i < polygonVertexCount; i++ ) 
	{         
		meshes.back().positionIndices.push_back(static_cast<unsigned short>(pMesh->GetPolygonVertices()[i]));     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::analyzeNormal(FbxMesh* pMesh) 
{     
	// レイヤー数の取得     
	int layerCount = pMesh->GetLayerCount(); 
 
    for(int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // 法線の取得         
		FbxLayerElementNormal* pElementNormal = pMesh->GetElementNormal(layer); 
 
        if( !pElementNormal ) 
		{ 
			continue; 
		} 
 
        // 法線数の取得         
		int normalCount = pElementNormal->GetDirectArray().GetCount(); 
 
        // 法線格納用コンテナの領域予約         
		meshes.back().normals.reserve(normalCount); 
 
        // 法線データの取得         
		for( int i = 0; i < normalCount; i++ ) 
		{             
			meshes.back().normals.push_back( D3DXVECTOR3( static_cast<float>(pElementNormal->GetDirectArray()[i][0]),                 
														  static_cast<float>(pElementNormal->GetDirectArray()[i][1]),                 
														  static_cast<float>(pElementNormal->GetDirectArray()[i][2])));         
		} 
 
 
        // マッピングモード・リファレンスモード取得         
		FbxLayerElement::EMappingMode mappingMode = pElementNormal->GetMappingMode();         
		FbxLayerElement::EReferenceMode referenceMode = pElementNormal->GetReferenceMode(); 
 
        switch(mappingMode) 
		{ 
 
        case FbxLayerElement::eNone:             
			MessageBox(NULL, "Normal MappingMode = mappingMode", "未実装", MB_OK);             
			break; 
 
        case FbxLayerElement::eByControlPoint: 
 
            // 頂点バッファと同じインデックスを使用 
 
            if( referenceMode == FbxLayerElement::eDirect ) 
			{                 
				// 法線インデックス格納用コンテナの領域予約                 
				meshes.back().normalIndices.reserve(meshes.back().points.size());                 
				
				// 頂点バッファと同じインデックスをコピー                 
				meshes.back().normalIndices.assign( meshes.back().positionIndices.begin(), meshes.back().positionIndices.end());             
			}             
			else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				MessageBox(NULL,  "Normal ReferenceMode = eIndexToDirect or eIndex, MappingMode = eByControlPoint",  "未実装", MB_OK);             
			}             
			
			break; 
 
        case FbxLayerElement::eByPolygonVertex: 
 
            /* 法線独自のインデックスを使用 */ 
 
            if( referenceMode == FbxLayerElement::eDirect ) 
			{                                  
				// インデックス参照の必要なし => インデックスを作成 
 
                // 法線インデックス格納用コンテナの領域予約                 
				meshes.back().normalIndices.reserve(normalCount);                 
				
				// 法線インデックスの作成                 
				for( int i = 0; i < normalCount; i++ ) 
				{                     
					meshes.back().normalIndices.push_back(i);                 
				}             
			}             
			else if( referenceMode == FbxLayerElement::eIndexToDirect  || referenceMode == FbxLayerElement::eIndex ) 
			{ 
                // 独自インデックスを所持 
 
                // インデックス数の取得                 
				int normalIndexCount = pElementNormal->GetIndexArray().GetCount();                 
				
				// 法線インデックス格納用コンテナの領域予約                 
				meshes.back().normalIndices.reserve(normalIndexCount);                 
				
				// 法線インデックスの取得                 
				for( int i = 0; i < normalIndexCount; i++ ) 
				{                     
					meshes.back().normalIndices.push_back(pElementNormal->GetIndexArray()[i]);                 
				}             
			}

            break; 
 
        case FbxLayerElement::eByPolygon:             
			MessageBox(NULL, "Normal MappingMode = eByPolygon", "未実装", MB_OK);             
			break;       

		case FbxLayerElement::eByEdge:             
			MessageBox(NULL, "Normal MappingMode = eByEdge", "未実装", MB_OK);             
			break;         

		case FbxLayerElement::eAllSame:             
			MessageBox(NULL, "Normal MappingMode = eAllSame", "未実装", MB_OK);            
			break;         
		
		default:             
			MessageBox(NULL, "Normal ???", "未実装", MB_OK);             
			break;         
		}     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::analyzeTexcoord(FbxMesh* pMesh, bool bRevers) 
{ 
	int layerCount = pMesh->GetLayerCount(); 
 
	if( !layerCount ) 
	{ 
		MessageBox(NULL, "レイヤーを持っていないメッシュを確認", "Analyze Texcoord", MB_OK); 
		return; 
	} 
 
	for( int layer = 0; layer < layerCount; layer++ ) 
	{ 
        // UVの取得         
		FbxLayerElementUV* pElementUV = pMesh->GetLayer(layer)->GetUVs(); 
 
        if( !pElementUV ) 
		{             
			MessageBox(NULL, "...UVのないメッシュレイヤーを確認", "Analyze Texcoord", MB_OK);             
			continue;         
		} 
 
        // UVセット名を取得         
		// = pElementUV->GetName(); 
 
        // マッピングモード・リファレンスモード取得         
		FbxLayerElement::EMappingMode mappingMode = pElementUV->GetMappingMode();         
		FbxLayerElement::EReferenceMode referenceMode = pElementUV->GetReferenceMode(); 
 
        if( mappingMode == FbxLayerElement::eByPolygonVertex ) 
		{             
			if( referenceMode == FbxLayerElement::eIndexToDirect || referenceMode == FbxLayerElement::eIndex ) 
			{                 
				int uvIndexCount = pElementUV->GetIndexArray().GetCount();                
				meshes.back().texcoordIndices.reserve(uvIndexCount); 
 
                for( int i = 0; i < uvIndexCount; i++ ) 
				{                     
					meshes.back().texcoordIndices.push_back(pElementUV->GetIndexArray().GetAt(i));                 
				} 
 
                int uvCount = pElementUV->GetDirectArray().GetCount();      

				meshes.back().texcoords.reserve(uvCount); 
 
                for( int i = 0; i < uvCount; i++ ) 
				{ 
                    meshes.back().texcoords.push_back(D3DXVECTOR2( static_cast<float>(pElementUV->GetDirectArray().GetAt(i)[0]),                         
																   static_cast<float>(bRevers ? 1 - pElementUV->GetDirectArray().GetAt(i)[1] : pElementUV->GetDirectArray().GetAt(i)[1])));                 
				}             
			}             
			else 
			{                 
				MessageBox(NULL, "Texcoord::未対応のリファレンスモードを取得", "FbxLayerElement::eByPolygonVertex", MB_OK);                 
				break;             
			}         
		}         
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{             
			MessageBox(NULL, "...未対応マッピングモード[eByControlPoint]を取得した", "Analyze Texcoord", MB_OK);         
		}         
		else if( mappingMode == FbxLayerElement::eByPolygon) 
		{             
			MessageBox(NULL, "...未対応マッピングモード[eByPolygon]を取得した", "Analyze Texcoord", MB_OK);         
		}         
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{             
			MessageBox(NULL, "...未対応マッピングモード[eByEdge]を取得した", "Analyze Texcoord", MB_OK);         
		}         
		else 
		{             
			MessageBox(NULL, "...知らないマッピングモードを取得した", "Analyze Texcoord", MB_OK);         
		} 
 
        break; // とりあえず１個めだけ     
	} 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::analyzeMaterial(FbxNode* pNode) 
{     
	// マテリアル数の取得     
	int materialCount = pNode->GetMaterialCount(); 
 
    textures.reserve(materialCount); 
 
    for( int i = 0; i < materialCount; i++ ) 
	{ 
		// マテリアル情報の取得     
		FbxSurfaceMaterial* pMaterial = pNode->GetMaterial(i); 
    
		// ディフューズ情報の取得     
		FbxProperty diffuseProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse); 
 
		// プロパティが持っているレイヤードテクスチャの枚数をチェック     
		int layeredTextureCount = diffuseProperty.GetSrcObjectCount<FbxLayeredTexture>(); 
 
		// レイヤードテクスチャが無ければ通常テクスチャ     
		if ( layeredTextureCount == 0 ) 
		{ 
			// 通常テクスチャの枚数をチェック         
			int textureCount = diffuseProperty.GetSrcObjectCount<FbxFileTexture>(); 
   
			// 各テクスチャについてテクスチャ情報をゲット         
			for( int i = 0; i < textureCount; i++ ) 
			{ 
				// i番目のテクスチャオブジェクト取得         
				FbxFileTexture* pTexture = diffuseProperty.GetSrcObject<FbxFileTexture>(i); 
 
				// テクスチャファイル名の取得         
				// std::string fileName = pTexture->GetFileName();         
				std::string relFileName = pTexture->GetRelativeFileName(); 
 
				// UVSet名の取得         
				std::string uvSetName = pTexture->UVSet.Get().Buffer(); 
 
				// 辞書登録         
				// texfile2UVset[relFileName] = uvSetName;         
				std::string strPathName = "data/dude/" + relFileName;        

				//  テクスチャクラスの取得
				Texture* pTexture2 = SceneManager::GetTexture( );

				// テクスチャの読み込みと管理     
				pTexture2->SetTextureImage( strPathName.c_str( ) );

				// テクスチャの読み込みと管理         
				textures.push_back( strPathName );    
			}         
		}     
		else 
		{ 
			// レイヤードテクスチャあり         
			MessageBox(NULL, "レイヤードテクスチャ", "マテリアルの取得", MB_OK);     
		}     
	} 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::analyzeMaterial(FbxMesh* pMesh) 
{     
	int layerCount = pMesh->GetLayerCount(); 
 
    for(int layer = 0; layer < layerCount; layer++) 
	{ 
		FbxLayerElementMaterial* pElementMaterial = pMesh->GetLayer(layer)->GetMaterials();

		if( !pElementMaterial ) 
		{ 
			continue; 
		} 
 
		int materialIndexCount = pElementMaterial->GetIndexArray().GetCount(); 
 
		if( materialIndexCount == 0 ) 
		{ 
			continue; 
		} 
 
		FbxLayerElement::EMappingMode mappingMode = pElementMaterial->GetMappingMode();     
		FbxLayerElement::EReferenceMode referenceMode = pElementMaterial->GetReferenceMode(); 
 
		if( mappingMode == FbxLayerElement::eAllSame ) 
		{         
			if( referenceMode == FbxLayerElement::eIndexToDirect ) 
			{        
				// メッシュ全部がこのマテリアルインデックス         
				meshes.back().materialIndex = pElementMaterial->GetIndexArray().GetAt(0);         
			}         
			else 
			{         
				MessageBox(NULL, "...未対応のリファレンスモードを取得した", "Material MappingMode = eAllSame", MB_OK);         
			}     
		}     
		else if( mappingMode == FbxLayerElement::eByControlPoint ) 
		{         
			MessageBox(NULL, "...未対応のマッピングモード[eByControlPoint]を取得した", "Material MappingMode", MB_OK);     
		}     
		else if( mappingMode == FbxLayerElement::eByPolygon ) 
		{         
			// マテリアル分割されているはずだから、一番はじめのだけでいい         
			meshes.back().materialIndex = pElementMaterial->GetIndexArray().GetAt(0);     
		}     
		else if( mappingMode == FbxLayerElement::eByEdge ) 
		{         
			MessageBox(NULL, "...未対応のマッピングモード[eByEdge]を取得した", "Material MappingMode", MB_OK);     
		}     
		else 
		{         
			MessageBox(NULL, "...未対応のマッピングモードを取得した", "Material MappingMode", MB_OK);     
		}     
	} 
}

//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
// Get the geometry offset to a node. It is never inherited by the children. 
FbxAMatrix SceneFBX::myNode::GetGeometry(FbxNode* pNode) 
{     
	const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);     
	const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);     
	const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot); 
 
    return FbxAMatrix(lT, lR, lS); 
} 
 
//--------------------------------------------------------------------------------------
//  ノードを辿って再帰的に情報を取得する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::analyzeCluster(FbxMesh* pMesh) 
{     
	D3DXMATRIX mtxIdentitiy;     
	D3DXMatrixIdentity(&mtxIdentitiy); 
 
    // スキンの数を取得     
	int skinCount = pMesh->GetDeformerCount(FbxDeformer::eSkin); 
 
    for( int skinNum = 0; skinNum < skinCount; skinNum++ ) 
	{ 
        // スキンを取得         
		FbxSkin* pSkin = (FbxSkin*)pMesh->GetDeformer(skinNum, FbxDeformer::eSkin); 
 
        // クラスターの数を取得         
		int clusterCount = pSkin->GetClusterCount(); 
 
        for( int clusterNum = 0; clusterNum < clusterCount; clusterNum++ ) 
		{ 
			// クラスタを取得             
			FbxCluster* pCluster = pSkin->GetCluster(clusterNum); 
 
			// このクラスタが影響を及ぼす頂点インデックスの個数を取得             
			int pointIndexCount = pCluster->GetControlPointIndicesCount(); 
 
			meshes.back( ).matrixes.emplace_back( 0 );

			if( !pointIndexCount ) 
			{                 
				// このメッシュにおいて、このクラスタは無視していいと思う...                 
				meshes.back().matrixes.back().push_back(mtxIdentitiy);         

				continue;             
			} 
 
			// 初期姿勢行列の取得             
			FbxAMatrix lReferenceGlobalInitPosition;             
			FbxAMatrix lReferenceGlobalCurrentPosition;             
			FbxAMatrix lReferenceGeometry;             
			FbxAMatrix lClusterGlobalInitPosition;             
			FbxAMatrix lClusterGlobalCurrentPosition;             
			FbxAMatrix lClusterRelativeInitPosition;             
			FbxAMatrix lClusterRelativeCurrentPositionInverse; 
 
            pCluster->GetTransformMatrix(lReferenceGlobalInitPosition);             
			
			// lReferenceGlobalCurrentPosition = pGlobalPosition; // <- たぶんワールド座標変換行列ではないかと                          
			
			// Multiply lReferenceGlobalInitPosition by Geometric Transformation             
			lReferenceGeometry = GetGeometry(pMesh->GetNode());             
			lReferenceGlobalInitPosition *= lReferenceGeometry; 
 
            // Get the link initial global position and the link current global position.             
			pCluster->GetTransformLinkMatrix(lClusterGlobalInitPosition);             
			
			// lClusterGlobalCurrentPosition = GetGlobalPosition(pCluster->GetLink(), pTime, pPose); // <- ポーズ行列の取り方？？？             
			
			FbxTime oneFrameTime;
			oneFrameTime.SetTime( 0 , 0 , 0 , 1 , 0 , 0 , FbxTime::eFrames60 );

			for( FbxTime currentTime = m_startTime; currentTime < m_endTime; currentTime += oneFrameTime )
			{
				lClusterGlobalCurrentPosition = pCluster->GetLink()->EvaluateGlobalTransform(currentTime); 
 
				// Compute the initial position of the link relative to the reference.             
				lClusterRelativeInitPosition = lClusterGlobalInitPosition.Inverse() * lReferenceGlobalInitPosition; 
 
				// Compute the current position of the link relative to the reference.             
				lClusterRelativeCurrentPositionInverse = lReferenceGlobalCurrentPosition.Inverse() * lClusterGlobalCurrentPosition; 
 
				// Compute the shift of the link relative to the reference.            
				FbxAMatrix VertexTransformMatrix = lClusterRelativeCurrentPositionInverse * lClusterRelativeInitPosition; 
				// ↑ 初期姿勢行列も考慮されたモーションボーン行列なので、これで頂点座標を変換するだけで良い 
 
				D3DXMATRIX d3dMtx; 
			
				for( int y = 0; y < 4; y++ ) 
				{                 
					for( int x = 0; x < 4; x++ ) 
					{ 
						d3dMtx(x, y) = (float)VertexTransformMatrix.Get(x, y);                 
					}  
				} 	
 
				meshes.back().matrixes.back().push_back(d3dMtx); 
			}
 
			int* pointIndexArray = pCluster->GetControlPointIndices();             
			double* weightArray = pCluster->GetControlPointWeights(); 
 
			for( int i = 0 ; i < pointIndexCount; i++ ) 
			{                 
				meshes.back().points[pointIndexArray[i]].bornRefarences.push_back(BornRefarence(clusterNum, static_cast<float>(weightArray[i])));            
			}     
		}		
	} 
}

//--------------------------------------------------------------------------------------
//  現在のファイルポインタからある文字列の数をカウントする関数
//--------------------------------------------------------------------------------------
std::string SceneFBX::myNode::GetAttributeTypeName(FbxNodeAttribute::EType type) 
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
//  頂点バッファを作成する関数
//--------------------------------------------------------------------------------------
void SceneFBX::myNode::MakeVertex( int size )
{     
	m_pVtxBuff = nullptr;

	//  デバイス情報の取得
	LPDIRECT3DDEVICE9 pDevice = SceneManager::GetRenderer( )->GetDevice( );

	//  頂点バッファの作成
	if( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D ) * size ,				//  作成したい頂点バッファのサイズ
											 D3DUSAGE_WRITEONLY ,				//  使用方法
											 FVF_VERTEX_3D ,					//  
											 D3DPOOL_MANAGED ,					//  メモリ管理方法( MANAGED → お任せ )
											 &m_pVtxBuff ,						//  バッファ
											 NULL ) )
	{
		MessageBox( NULL , "頂点バッファインターフェースを正しく取得出来ませんでした。" , "エラーメッセージ" , MB_OK );

		return;
	}
}