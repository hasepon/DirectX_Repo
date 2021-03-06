#pragma once


#include"includeheader.h"
class CRenderModel;
#include"RenderModel.h"
class ColBox;
#include"Collision.h"
#include"SoundInclude.h"
#include"BlackoutFade.h"

class CPlayer : public ObjBase
{
public:
	CPlayer();
	~CPlayer();
	
	void Init();
	void Update();
	void Draw();
	void Release();

private:

	void Move();
	void RotateDir();
	void ChangeDir();
	void MoveResult();
	void HitPlayer();

	enum Controller
	{
		Forwad = 0x00000001,
		Left = 0x00000010,
		Right = 0x00000100,
		Back = 0x00001000,
		FL = 0x00000011,
		FR = 0x00000101,
		BL = 0x00001010,
		BR = 0x00001100,
	};
		
	float m_MoveSpd;
	float m_DirectionAngle;

	int m_Inputstate;
	bool m_Alive;
	bool m_Invisible;
	D3DXVECTOR3 m_Movement;
	D3DXVECTOR3 m_Direction;
	ColBox*	m_Collision;
	CRenderModel* m_RenderModel;
	
	AudioComponent m_FallVoice;
	AudioComponent m_BombVoice;

	std::list<ObjBase*> m_HitObj;

};