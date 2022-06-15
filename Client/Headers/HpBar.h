#ifndef HPBAR_H__
#define HPBAR_H__

#include "GameObject.h"
#include "Include.h"

namespace Engine
{
	class Transform;
	class CubeTex;
	class CubeColor;
	class Texture;
	class Camera;
}

_BEGIN(Client)

class HpBar final : public Engine::GameObject
{
private:
	explicit HpBar(LPDIRECT3DDEVICE9 pDevice);
	explicit HpBar(const HpBar& rhs);
	virtual ~HpBar() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT ReadyComponent(void);

public:
	void Set_GreenWorldMatrix();
	void Set_RedWorldMatrix();
	void SetGameObject(Engine::Transform* pTransform) { m_pTargeInfo = pTransform; }
	void SetSize(const D3DXVECTOR3& vSize) { m_vSize = vSize; }
	void SetHeight(const float height) { mHeight = height; }
	void SetHp(const float hp) { mHp = hp; if (mHp < 0.f) mHp = 0.f; }
	void SetDeathCount(const int count) { mDeathCount = count; }

public:
	void AddDeathCount() { if(mDeathCount < DEATH_COUNT::DEATH_END) ++mDeathCount;}
	const int GetDeathCount() { return mDeathCount; }

public:
	static HpBar* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual HpBar* Clone() override;
	virtual DWORD Free() override;

private:
	Engine::Transform* m_pTransform = nullptr;
	Engine::CubeTex* m_pBufferGreen = nullptr;
	Engine::Texture* m_pTexture = nullptr;
	Engine::CubeTex* m_pBufferRed = nullptr;

private:
	Engine::Camera* m_pCamera = nullptr;
	Engine::MATRIX_INFO mMatrixInfo;
	D3DXVECTOR3 m_vSize = D3DXVECTOR3{ 0.f, 0.f, 0.f };
	D3DXVECTOR3 m_vPos = D3DXVECTOR3{ 0.f, 0.f, 0.f };
	float mHeight = 0.f;
	float mHp = 0.f;
	
private: // TY_HP_180628_Death Related Variable
	int mDeathCount = DEATH_END;

private: 
	Engine::Transform* m_pTargeInfo = nullptr;

};
_END
#endif
