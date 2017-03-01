#pragma once

#include"Sprite.h"
#include<functional>

//////////////////////////////////////////////////////////////
//
//		FadeIn,Outのクラス
//
/////////////////////////////////////////////////////////////
class CFade : public ObjBase
{
public:

	CFade();
	~CFade();

	// Update
	void Update();
	// FADE描画用
	void UIDraw();

private:
	//==============================
	//	変数
	int Fadenum;		// 現在のFade
	CSprite *sprite;	// SpriteClass

	//==============================
	//	関数
	// FadeInを呼ぶクラス
	void FadeIn();
	// FadeOutを呼ぶクラス
	void FadeOut();
	// functionのFade用(返り値無し)
	std::function< void(void) > Fadefunc;
	
};
