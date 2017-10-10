#pragma once
#include"includeheader.h"

/// <summary>
///	
/// </summary>
class CCamera : public CSingleton<CCamera>
{
public:
	CCamera();
	~CCamera();

	void Init();
	void LateUpdate();
	void LateDraw();
	void Release();
	void SetTargetObj(ObjBase* _target);

private:
	
	void CameraInput();
	void TpsCamera();

	// �J�����̐ݒ�ϐ�
	D3DXVECTOR3 m_Position;			// ���W(X,Y,Z)
	D3DXVECTOR3 m_LookPosDeviation;	// �����_�̈ړ�����
	D3DXVECTOR3 m_DirDef;			// �J�����̌���
	D3DXVECTOR3 m_Upvec;			// ��
	D3DXVECTOR3 m_LookAtPos;
	D3DXVECTOR3 m_CameraMove;		// ���x

	D3DXMATRIX m_WorldMatrix;			// Obj�̃��[���h�s��
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_PoseMatrix;
	D3DXMATRIX m_ProjectionMatrix;

	float m_Distance;
	float m_Zrot;
	float m_AngleUnit;			// �ړ����̉�]�P�� = �J�����̈ړ����x
	float m_OffsetZ;

	ObjBase* m_TargetObj;

};
