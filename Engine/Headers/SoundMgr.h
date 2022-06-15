#ifndef SOUNDMGR_H__
#define SOUNDMGR_H__

#include "Base.h"
#include "AnimationStruct.h"

_BEGIN(Engine)
class _ENGINE_DLL SoundMgr final : public Engine::Base
{
public:
	_DECLARE_SINGLETON(SoundMgr)

private:
	explicit SoundMgr();
	virtual ~SoundMgr() = default;

public:
	enum CHANNEL { BGM, PLAYER, BOSS, EFFECT, UI, UI_EFFECT, LOOP_EFFECT, CHANNEL_END = 35 };

public:
	HRESULT Init();
	HRESULT Update();

public:
	HRESULT LoadSoundFile();
	void SoundPlay(const TCHAR* pSoundKey, CHANNEL channel);
	void SoundPlay(const TCHAR* pSoundKey);
	void SoundPlay_LoopUI(const TCHAR* pSoundKey);

	void PlayBGM(const TCHAR* pSoundKey);
	void StopSound(CHANNEL channel);
	void StopAll();
	void SetVolume(CHANNEL channel, float val);

public:
	void Set_SoundEffect(const FRAME_INFO* pFrameInfo, float& soundTime,  const size_t frame, const TCHAR* pSoundName, const CHANNEL channel);


public:
	virtual DWORD Free();

private:
	FMOD_SYSTEM* m_pSystem;
	FMOD_CHANNEL* m_pChannelArr[CHANNEL_END];
	const TCHAR* m_pSoundName = nullptr;
	map<const TCHAR*, FMOD_SOUND*>	mSoundSpace;

private:
	size_t mPreFrame = 0;
	bool mCheckOnly[CHANNEL_END] = {true};



};

_END

#endif