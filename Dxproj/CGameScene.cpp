#include "CGameScene.h"



CGameScene::CGameScene()
{
	CObjMgr::Getintance().PushObj(new CPlayer      , ID_PLAYER);
	CObjMgr::Getintance().PushObj(new CEnemyMgr    , ID_OTHER);
	CObjMgr::Getintance().PushObj(new CTunnelCreate, ID_OTHER);
	CObjMgr::Getintance().PushObj(new CSkybox      , ID_OTHER);
}


CGameScene::~CGameScene()
{
}

void CGameScene::Init()
{
	if (m_Initflag == true)
	{
		return;
	}
	CObjMgr::Getintance().Init();
	CCamera::Getintance().Init();
	m_Initflag = true;
}

void CGameScene::Update()
{
	CObjMgr::Getintance().Update();
}

void CGameScene::LateUpdate()
{
	CObjMgr::Getintance().LateUpdate();
}

void CGameScene::Draw()
{
	CObjMgr::Getintance().Draw();
}

void CGameScene::LateDraw()
{
	CObjMgr::Getintance().LateDraw();
}

void CGameScene::UIDraw()
{
	CObjMgr::Getintance().UIDraw();
}

void CGameScene::Release()
{
	CObjMgr::Getintance().Release();
}

void CGameScene::Pause()
{
	CObjMgr::Getintance().Pause();
}