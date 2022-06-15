#include "..\Headers\SoundMgr.h"

_USING(Engine)
_IMPLEMENT_SINGLETON(SoundMgr)

SoundMgr::SoundMgr()
{
	Init();
}

HRESULT SoundMgr::Init()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, CHANNEL_END, FMOD_INIT_NORMAL, nullptr);

	//LoadSoundFile();
	return NO_ERROR;
}

HRESULT SoundMgr::Update()
{
	FMOD_System_Update(m_pSystem);

	return NO_ERROR;
}

HRESULT SoundMgr::LoadSoundFile()
{
	_finddata_t fd;

	long handle = 0;
	handle = _findfirst("../Sound/*.*", &fd);

	if (-1 == handle)
	{
		_MSG_BOX("Not Found Sounds")
		return  FALSE;
	}

	int result = 0;

	while (-1 != result)
	{
		char szFullPath[128] = "../Sound/";
		strcat_s(szFullPath, fd.name);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT res = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_HARDWARE
			, nullptr, &pSound);

		if (FMOD_OK == res)
		{
			int iLength = strlen(fd.name);
			TCHAR* pSoundKey = new TCHAR[iLength + 1];

			MultiByteToWideChar(CP_ACP, 0, fd.name, iLength + 1, pSoundKey, iLength + 1);
			mSoundSpace.insert({ pSoundKey, pSound });
		}

		result = _findnext(handle, &fd);
	}

	_findclose(handle);
	FMOD_System_Update(m_pSystem);
	return NO_ERROR;

}

void SoundMgr::SoundPlay(const TCHAR * pSoundKey, CHANNEL channel)
{
	auto iter_find = find_if(mSoundSpace.begin(), mSoundSpace.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (mSoundSpace.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[channel]));
}

void SoundMgr::SoundPlay(const TCHAR * pSoundKey)
{
	auto iter_find = find_if(mSoundSpace.begin(), mSoundSpace.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (mSoundSpace.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[LOOP_EFFECT]));

	FMOD_Channel_SetMode(m_pChannelArr[LOOP_EFFECT], FMOD_LOOP_NORMAL);

}

void SoundMgr::SoundPlay_LoopUI(const TCHAR * pSoundKey)
{
	auto iter_find = find_if(mSoundSpace.begin(), mSoundSpace.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (mSoundSpace.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[UI_EFFECT]));

	FMOD_Channel_SetMode(m_pChannelArr[UI_EFFECT], FMOD_LOOP_NORMAL);

}

void SoundMgr::PlayBGM(const TCHAR * pSoundKey)
{
	auto iter_find = find_if(mSoundSpace.begin(), mSoundSpace.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (mSoundSpace.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, FMOD_CHANNEL_FREE
		, iter_find->second, FALSE, &(m_pChannelArr[BGM]));

	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);

}

void SoundMgr::StopSound(CHANNEL channel)
{
	FMOD_Channel_Stop(m_pChannelArr[channel]);
}

void SoundMgr::StopAll()
{
	for (int i = 0; i < CHANNEL_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);

}

void SoundMgr::SetVolume(CHANNEL channel, float val)
{
	FMOD_Channel_SetVolume(m_pChannelArr[channel], val);

}

void SoundMgr::Set_SoundEffect(const FRAME_INFO* pFrameInfo, float& soundTime, const size_t frame, const TCHAR* pSoundName, const CHANNEL channel)
{
	//if (pFrameInfo == nullptr)
	//{
	//	if (m_pSoundName != pSoundName)
	//	{
	//		if (mCheckOnly[channel])
	//		{
	//			SoundPlay(pSoundName, channel);
	//			m_pSoundName = pSoundName;
	//			mCheckOnly[channel] = false;
	//		}
	//	}
	//	else 
	//	{
	//		mCheckOnly[channel] = true;
	//	}
	//}
	//else {
		if (pFrameInfo->rotFrame == frame)
		{
			if (m_pSoundName != pSoundName)
			{
				if (mCheckOnly[channel])
				{
					SoundPlay(pSoundName, channel);
					m_pSoundName = pSoundName;
					mCheckOnly[channel] = false;
				}
			}
		}
		else
		{
			mCheckOnly[channel] = true;
		}
	//}
}

DWORD SoundMgr::Free()
{
	for (auto& MyPair : mSoundSpace)
	{
		delete[] MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	}

	mSoundSpace.clear();
	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);

	return 0;
}
