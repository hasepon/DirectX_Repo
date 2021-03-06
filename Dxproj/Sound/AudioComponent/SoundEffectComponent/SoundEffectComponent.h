#pragma once

class CSoundComponent;
#include"..\..\Sound.h"
#include "..\SoundComponent\SoundComponent.h"

#include<XAPOFX.h>
#pragma comment(lib,"XAPOFX.lib")
/*
*	
*	サウンド一つ一つに付属するサウンドエフェクト
*	Cue全体には波及しないので注意
*
*/
class CSoundEffectComponent : public CSoundComponent
{
public:
	CSoundEffectComponent();
	~CSoundEffectComponent();

	void Init(IXAudio2SourceVoice* voice);

	// 更新処理
	void ComponentUpdate() override;

	// リバーブ用
	void SetReverbVolume(float walltype, float roomsize);
	void offsetReverbVolume();

	// エコー用
	void SetEchoVolume(float Delay, float feedback, float wetdry);
	void offsetEchoVolume();

	// ピッチレート用
	void SetPitchRate(float PitchRate);
	void offsetPitchRate();

	// EQイコライザー用
	void SetEQVolume(float freqcenter[4], float gain[4], float bandwidth[4]);
	void offSetEQVolume();

	// 宇宙空間用
	void SetSpaceEffect();
	void offsetSpaceEffect();

	// トンネル空間用
	void SetTunnelEffect();
	void offsetTunnelEffect();

private:

	IXAudio2SourceVoice* m_voice;		// 登録されるボイスエフェクト
	IUnknown* m_Effect[3];

	XAUDIO2_EFFECT_CHAIN m_chain;
	XAUDIO2_EFFECT_DESCRIPTOR m_desc[3];

	FXREVERB_PARAMETERS m_Reverbpram;	// リバーブエフェクト
	FXECHO_PARAMETERS m_EchoPram;		// エコーエフェクト
	FXEQ_PARAMETERS m_EqPram;			// イコライザ

	const int XAPOFX_EFFECT_TYPE = 3;	// XAPOFX用の定義

	std::function< void() > Soundfunc;	// voiceのfunc

	//
	float m_SpaceDlay = 1.6f;
	float m_SpaceFeedback = 0.3f;
	float m_SpaceDlywet = 0.3f;

	// トンネル内１
	float m_TunnelWall = 0.2f;
	float m_TunnelRoomsize = 0.9f;
};

