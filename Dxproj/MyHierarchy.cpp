#include "MyHierarchy.h"


////////////////////////////////////////////////////////////////////////
//
//		フレームを生成するよ
//
////////////////////////////////////////////////////////////////////////
HRESULT CMyHierarchy::CreateFrame(LPCSTR pMesh, LPD3DXFRAME* ppNewFrame)
{
	// フレームを初期化
	*ppNewFrame = NULL;

	// フレーム用のメモリ確保
	MYFRAME* pFrame = new MYFRAME;
	if (pFrame == NULL)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pFrame,sizeof(MYFRAME));

	// フレーム名のコピー
	if (pMesh)
	{
		// フレーム名あり
		pFrame->Name = new char[lstrlenA(pMesh) + 1];
		if (pFrame->Name == NULL)
		{
			delete pFrame;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pFrame->Name , pMesh);

	}
	else{
		// 名前がなかったというわけだ
		pFrame->Name = NULL;
	}


	// マトリックスの初期化ってやつだ
	D3DXMatrixIdentity(&pFrame->TransformationMatrix);
	D3DXMatrixIdentity(&pFrame->CombinedTransformationMatrix);

	// フレームをレシーブするよ
	*ppNewFrame = pFrame;
	return S_OK;

}

HRESULT CMyHierarchy::CreateMeshContainer(LPCSTR Name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstance,
	DWORD NumMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer)
{
	// まずはNULLセット
	*ppNewMeshContainer = NULL;

	// メッシュがおかしい場合はエラーで返します
	if (pMeshData->Type != D3DXMESHTYPE_MESH || pMeshData->pMesh->GetFVF() == 0)
	{
		return E_FAIL;
	}

	//　メッシュオブジェクトを入れなおします
	LPD3DXMESH pMesh = pMeshData->pMesh;

	// メッシュコンテナ用メモリの確保
	MYMESHCONTAINER* pMeshContainer = new MYMESHCONTAINER;
	// エラーしてたら処理を終了します
	if (pMeshContainer == NULL)
	{
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer, sizeof(MYMESHCONTAINER));
	
	// メッシュ名をコピーするよ
	if (Name){
		pMeshContainer->Name = new char[lstrlenA(Name) + 1];
		// エラーしてたら処理を終了します
		if (pMeshContainer->Name == NULL)
		{
			delete pMeshContainer;
			return E_OUTOFMEMORY;
		}
		lstrcpyA(pMeshContainer->Name , Name);
	}
	else{
		pMeshContainer->Name = NULL;
	}

	// D3Dデバイスの取得よ
	LPDIRECT3DDEVICE9 pDevice = NULL;
	pMesh->GetDevice(&pDevice);

	// 総ポリゴン(三角)数の取得よ
	int nFacesAmount = pMesh->GetNumFaces();

	// 法線のチェック
	if (pMesh->GetFVF() & D3DFVF_NORMAL)
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMesh->AddRef();
	}
	else{
		if (FAILED(pMesh->CloneMeshFVF(pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL, pDevice, &pMeshContainer->MeshData.pMesh)))
		{
			pDevice->Release();
			DestroyMeshContainer(pMeshContainer);
			return E_FAIL;
		}
		pMesh = pMeshContainer->MeshData.pMesh;
		D3DXComputeNormals(pMesh,NULL);
	}

	// マテリアル用にメモリを確保するよ
	pMeshContainer->NumMaterials = max(1,NumMaterials);
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[nFacesAmount * 3];

	if (pMeshContainer->pAdjacency == NULL || pMeshContainer->ppTextures == NULL || pMeshContainer->pMaterials == NULL)
	{
		DestroyMeshContainer(pMeshContainer);
		pDevice->Release();
		return E_OUTOFMEMORY;
	}
	ZeroMemory(pMeshContainer->ppTextures, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials);
	CopyMemory(pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * nFacesAmount * 3);

	// マテリアルの設定
	if (NumMaterials > 0)
	{
		// カレントディレクトリを変更
		TCHAR szCurrentDir[_MAX_PATH];
		if (m_szDir[0])
		{
			GetCurrentDirectory(_MAX_PATH,szCurrentDir);
			SetCurrentDirectory(m_szDir);
		}
		CopyMemory(pMeshContainer->pMaterials,pMaterials,sizeof(D3DXMATERIAL)*NumMaterials);
		for (DWORD i = 0; i < NumMaterials;++i)
		{
			pMeshContainer->pMaterials[i].MatD3D.Ambient = pMeshContainer->pMaterials[i].MatD3D.Diffuse;
			if (FAILED(D3DXCreateTextureFromFileA(pDevice, pMeshContainer->pMaterials[i].pTextureFilename, &pMeshContainer->ppTextures[i])))
			{
				// テクスチャのない場合の設定
				pMeshContainer->ppTextures[i] = NULL;
			}
		}

		// カレントディレクトリの修正
		if (m_szDir[0])
			SetCurrentDirectory(szCurrentDir);
		else{
			ZeroMemory(pMeshContainer->pMaterials,sizeof(D3DXMATERIAL));
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
			pMeshContainer->pMaterials[0].MatD3D.Ambient = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
		}

		// 属性テーブルの取得
		HRESULT hr = pMeshContainer->MeshData.pMesh->CloneMeshFVF(D3DXMESH_MANAGED,
			pMeshContainer->MeshData.pMesh->GetFVF(), pDevice, &pMesh);
		if (FAILED(hr))
		{
			pDevice->Release();
			return hr;
		}

		SAFE_RELEASE(pMeshContainer->MeshData.pMesh);
		pMeshContainer->MeshData.pMesh = pMesh;
		hr = pMesh->GetAttributeTable(NULL,&pMeshContainer->NumAttributeGroups);

		if (FAILED(hr))
		{
			pDevice->Release();
			return hr;
		}

		pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->NumAttributeGroups];
		if (pMeshContainer->pAttributeTable == NULL)
		{
			pMeshContainer->NumAttributeGroups = 0;
			pDevice->Release();
			return E_OUTOFMEMORY;
		}

		hr = pMesh->GetAttributeTable(pMeshContainer->pAttributeTable,NULL);
		if (FAILED(hr))
		{
			pDevice->Release();
			return hr;
		}
		pDevice->Release();

		// スキン情報の確認
		if (pSkinInfo)
		{
			// スキン情報の保存
			pMeshContainer->pSkinInfo = pSkinInfo;
			pSkinInfo->AddRef();


			// ワールドマトリックスの場所の確保
			DWORD dwBoneNum = pSkinInfo->GetNumBones();
			pMeshContainer->pBoneOffsetMatrix = new D3DXMATRIX[dwBoneNum];
			if (pMeshContainer->pBoneOffsetMatrix == NULL)
			{
				return E_OUTOFMEMORY;
			}

			// ワールドマトリックスの初期化
			for (DWORD i = 0; i < dwBoneNum; i++)
			{
				pMeshContainer->pBoneOffsetMatrix[i] = *pSkinInfo->GetBoneOffsetMatrix(i);
			}

			// スキン用に変換
			hr = pSkinInfo->ConvertToBlendedMesh(
				pMesh, NULL, pMeshContainer->pAdjacency,
				NULL, NULL, NULL,
				&pMeshContainer->dwWeight,
				&pMeshContainer->dwBone,
				&pMeshContainer->pBoneBuffer,
				&pMeshContainer->MeshData.pMesh
				);
			if (FAILED(hr))
				return hr;

		}

		// 確保したメッシュコンテナを返す
		*ppNewMeshContainer = pMeshContainer;
		return S_OK;

	}
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//
//		フレームを消し飛ばす！
//
////////////////////////////////////////////////////////////////////////
HRESULT CMyHierarchy::DestroyFrame(LPD3DXFRAME pFrametoFree)
{
	MYFRAME *pFrame = (MYFRAME*)pFrametoFree;
	SAFE_DELETE_ARRAY(pFrame->Name);
	SAFE_DELETE(pFrame);
	return S_OK;
}

////////////////////////////////////////////////////////////////////////
//
//		メッシュコンテナを破棄するよ
//
////////////////////////////////////////////////////////////////////////
HRESULT CMyHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	MYMESHCONTAINER* pMeshContainer = (MYMESHCONTAINER*)pMeshContainerToFree;

	SAFE_DELETE_ARRAY(pMeshContainer->Name);
	SAFE_DELETE_ARRAY(pMeshContainer->pAdjacency);
	SAFE_DELETE_ARRAY(pMeshContainer->pMaterials);

	// 全テクスチャの解放
	if (pMeshContainer->ppTextures) {
		for (DWORD i = 0; i < pMeshContainer->NumMaterials; ++i) {
			SAFE_RELEASE(pMeshContainer->ppTextures[i]);
		}
	}
	SAFE_DELETE_ARRAY(pMeshContainer->ppTextures);

	SAFE_RELEASE(pMeshContainer->MeshData.pMesh);

	SAFE_DELETE_ARRAY(pMeshContainer->pAttributeTable);
	pMeshContainer->NumAttributeGroups = 0;

	SAFE_RELEASE(pMeshContainer->pBoneBuffer);
	SAFE_DELETE_ARRAY(pMeshContainer->pBoneOffsetMatrix);
	SAFE_DELETE_ARRAY(pMeshContainer->ppBoneMatrix);

	SAFE_DELETE(pMeshContainer);
	return S_OK;
}

//---------------------------------------------------------------------------------------
// コンストラクタ
//---------------------------------------------------------------------------------------
CMyHierarchy::CMyHierarchy(void)
{
	m_szDir[0] = 0;
}

//---------------------------------------------------------------------------------------
// Xファイルのあるディレクトリを設定
//---------------------------------------------------------------------------------------
void CMyHierarchy::SetDirectory(LPCTSTR pszDir)
{
	if (pszDir)
		lstrcpy(m_szDir, pszDir);
	else
		m_szDir[0] = 0;
}