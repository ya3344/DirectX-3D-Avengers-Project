#ifndef UI_H__
#define UI_H__

#include "GameObject.h"
#include "Include.h"


namespace Engine
{
	class Transform;
	class UITex;
	class Texture;
	class Renderer;
}

_BEGIN(Client)

typedef struct UIInfo
{
	explicit UIInfo() {}
	explicit UIInfo(const float X, const float Y, const float sizeX, const float sizeY)
	{
		this->X = X;
		this->Y = Y;
		this->sizeX = sizeX;
		this->sizeY = sizeY;
	}

	float X = 0.f;
	float Y = 0.f;
	float sizeX = 0.f;
	float sizeY = 0.f;

}UI_INFO;

class UI : public Engine::GameObject
{
public:
	enum UI_TYPE { PART_UI, FULL_UI, END };

protected:
	explicit UI(LPDIRECT3DDEVICE9 pDevice);
	explicit UI(const UI& rhs);
	virtual ~UI() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	void Render(const bool bExit = false);
	virtual HRESULT ReadyPrototype() override;

protected:
	HRESULT ReadyComponent(void);

public:
	void SetTextureNum(const size_t textureNum) { mTextureNum = textureNum; }

public:
	virtual UI* Clone() = 0;
	virtual DWORD Free() override;

protected:
	Engine::Transform* m_pTransform = nullptr;
	Engine::Renderer* m_pRenderer = nullptr;
	Engine::UITex* m_pBuffer = nullptr;
	Engine::Texture* m_pTexture = nullptr;
protected:
	//DH_SoundMgrSetting_180705_Setting to SoundMgr
	Engine::SoundMgr* m_pSoundMgr = nullptr;

protected:
	UI_INFO mUIInfo;

protected:
	size_t mTextureNum = 0;
	float mLastTime = 0.f;
protected:
	//DH_SoundMgrSetting_180705_Setting to SoundMgr
	float mSoundTime = 0.f;
};

_END

#endif
