#pragma once

#include"MyHierarchy.h"
#include"includeheader.h"

class CRenderModel
{
private:
	bool Init(LPCTSTR ModelName);
	void Release();

	LPD3DXFRAME					m_FrameRoot;	// ���[�g �t���[�� �I�u�W�F�N�g
	LPD3DXANIMATIONCONTROLLER	m_AnimCtrl;	// �A�j���[�V���� �R���g���[�� �I�u�W�F�N�g
	UINT						m_NumAnimSet;	// �A�j���[�V���� �Z�b�g��
	LPD3DXANIMATIONSET*			m_AnimSet;	// �A�j���[�V���� �Z�b�g
	CMyHierarchy				m_Hierarchy;	// �K�w�������m��/����N���X
	DWORD						m_Prev;		// ���O�̎���
	bool						m_AnimationFlag;
public:
	CRenderModel(LPCTSTR ModelName);
	~CRenderModel();

	void AnimationUpdate(D3DXMATRIX& world);
	void RenderModel();
	void UpdateFrameMatrices(LPD3DXFRAME pFrameBase, LPD3DXMATRIX pParentMatrix);
	void DrawFrame(LPD3DXFRAME pFrame);
	void RenderMeshContainer(LPD3DXMESHCONTAINER pMeshContainerBase, LPD3DXFRAME pFrameBase);
	HRESULT AllocBoneMatrix(LPD3DXMESHCONTAINER pMeshContainerBase);
	HRESULT AllocAllBoneMatrix(LPD3DXFRAME pFrameBase);
	void SwitchAnimSet(UINT uAnimSet);
	void SetTime(double dTime);
	D3DXMATRIX* SerchBorn(LPSTR BornName);
	D3DXMATRIX* GetBorn(LPD3DXFRAME pFrameBase, LPSTR BornName);

};