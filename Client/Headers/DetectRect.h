#ifndef DETECT_RECT_H__
#define DETECT_RECT_H__

#include "Gameobject.h"
#include "Include.h"

namespace Engine
{
	class Renderer;
}

_BEGIN(Client)

class Player;
class DetectRect final : public Engine::GameObject
{
public:  // JH_AnimFrame_180624 Detect EnumColor Add
	enum DETECT_COLOR { ORANGE, RED, END };

private:
	explicit DetectRect(LPDIRECT3DDEVICE9 pDevice);
	explicit DetectRect(const DetectRect& rhs);
	virtual ~DetectRect() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

private:
	HRESULT ReadyComponent(void);

public:
	void SetTarget(Player* pPlayer);
	void SetFencePoint(D3DXVECTOR3* vPoint);
	// JH_SuperSetting SUPER_CUBE_INFO Set Function 
	void SetBossInfo(const SUPER_CUBE_INFO* pBossInfo) { m_pBossInfo = pBossInfo; };

public: // JH_AnimFrame_180625 SetOperation Function
	void SetOperation(const bool bOperation) { m_bOperation = bOperation; }
	bool GetOperation() { return m_bOperation; }

public: // JH_AnimFrame_180624 PlayerDetect Related Funciton
	const bool* Get_PlayerDetect() const { return m_bPlayerDetect; }

private:
	void AdjustDetectRect();
	void DetectObject();

public:
	static DetectRect* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual GameObject* Clone() override;
	virtual DWORD Free() override;

private: // Component Related Variable
	Engine::Renderer* m_pRenderer = nullptr;

private:
	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	DWORD mVtxSize = 0;
	DWORD mVtxCnt = 0;
	DWORD mVtxFVF = 0;
	Engine::VTXCOL* m_pVertex = nullptr;

private:
	LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
	DWORD mIdxSize = 0;
	DWORD mTriCnt = 0;
	D3DFORMAT mIdxFmt = D3DFMT_UNKNOWN;

private:
	D3DXMATRIX m_matWorld;
	const D3DXVECTOR3* m_pCenterPos = nullptr;
	const D3DXVECTOR3* m_pCenterDir = nullptr;
	const D3DXVECTOR3* m_pTargetPos = nullptr;
	// JH_SuperSetting SUPER_CUBE_INFO Variable
	const SUPER_CUBE_INFO* m_pBossInfo = nullptr;

private:
	float mRangeRadian = 0.f;
	float mDirRadian = 0.f;
	float mDetectRadius = 0.f;
	DWORD mOldColor = D3DCOLOR_ARGB(0, 0, 0, 0);
	DWORD mRectColor = D3DCOLOR_ARGB(0, 0, 0, 0);

private: // Fence of Terrain
	D3DXVECTOR3 vFencePos[4];

private: // JH_AnimFrame_180624 Player Detect Variable
	bool m_bPlayerDetect[END] = { 0, };

private: // JH_AnimFrame_180625 Detect Operation Vriable
	bool m_bOperation = false;



};
_END
#endif
