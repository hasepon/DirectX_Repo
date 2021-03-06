#pragma once

#include<d3d9.h>
#include<d3dx9.h>

#ifndef SAFE_DELETE_ARRAY
#define	SAFE_DELETE_ARRAY(p) {if(p){delete[]p;p=NULL;}}
#endif
#ifndef SAFE_DELETE
#define	SAFE_DELETE(p) {if(p){delete p;p=NULL;}}
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) {if(p){p->Release();p=NULL;}}
#endif

// フレームクラス
struct MYFRAME : public D3DXFRAME
{	// 最終的なマトリックスを格納するための変数を追加.
	D3DXMATRIX	CombinedTransformationMatrix;
};

// メッシュコンテナクラス
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{	// テクスチャを保存するための変数を追加
	LPDIRECT3DTEXTURE9*		ppTextures;
	// 属性テーブル
	LPD3DXATTRIBUTERANGE	pAttributeTable;
	DWORD					NumAttributeGroups;
	// 以下はスキンメッシュ用
	DWORD			dwWeight;	// 重みの数
	DWORD			dwBone;		// ボーンの数
	LPD3DXBUFFER	pBoneBuffer;// ボーン・テーブル
	LPD3DXMATRIX*	ppBoneMatrix;// ボーンのワールド行列の先頭
	LPD3DXMATRIX	pBoneOffsetMatrix;// フレームとしてのボーンのワールド行列
};

//////////////////////////////////////////////////////////////////////////////////
//
//	CMyHierarchy
//		Xファイル内に定義されているポリゴンメッシュのフレーム階層の読み込み
//		2016年頃から一切修正変更かかっていないです。
//////////////////////////////////////////////////////////////////////////////////
class CMyHierarchy : public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)(LPCSTR Name, LPD3DXFRAME *ppNewFrame);
	STDMETHOD(CreateMeshContainer)(
		LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstances,
		DWORD NumMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer);
	STDMETHOD(DestroyFrame)(LPD3DXFRAME pFrameToFree);
	STDMETHOD(DestroyMeshContainer)(LPD3DXMESHCONTAINER pMeshContainerBase);

	CMyHierarchy(void);
	void SetDirectory(LPCTSTR pszDir);

private:
	TCHAR m_szDir[_MAX_PATH];	// Xファイルのあるディレクトリ
};

