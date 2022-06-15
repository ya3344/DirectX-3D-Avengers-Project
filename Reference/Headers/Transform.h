#ifndef TRANSFORM_H__
#define TRANSFORM_H__

#include "Component.h"

_BEGIN(Engine)
class VIBuffer;
class _ENGINE_DLL Transform final : public Component
{
public:
	enum INFO { INFO_RIGHT, INFO_UP, INFO_LOOK, INFO_POSITION };

private:
	explicit Transform(LPDIRECT3DDEVICE9 pDevice);
	explicit Transform(const Transform& rhs);
	virtual ~Transform() = default;

public:
	HRESULT Init();

public:
	HRESULT SetUp_World();
	void SetInfo(INFO info, const D3DXVECTOR3* pVector);
	void SetInfo(INFO info, const float X, const float Y, const float Z);

public:
	const D3DXVECTOR3* GetInfo(INFO info);
	const D3DXMATRIX& GetWolrd() const { return m_matWorld; }
	D3DXVECTOR3 GetScale();
	//TY_GetWolrdPointer_180613_Add Function GetWolrdPointer
	D3DXMATRIX* GetWolrdPointer() { return &m_matWorld; }
	//DH_GetAngle_180622_Get Angle
	const float* GetAngle() { return &mAngle; }
	//DH_SetupBillBoard_180622_Set up to BillBoard
	void SetupBillBoard();

public:
	void TargetStep(D3DXVECTOR3 vTargetPos, const float speedPerSec);
	void GoStraight(const float speedPerSec);
	void RightStep(const float speedPerSec);
	void LeftStep(const float speedPerSec);
	void SetScale(const float fX, const float fY, const float fZ); //TY_UI_180627_ Add Function SetScale
	void SetRotation(const float radian);
	void RotationUp(const float radian);

public:
	HRESULT ComputeY_OnBuffer(Engine::VIBuffer* pBuffer);

public:
	static Transform* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	// 이 객체를 가지고 있는 객체의 상태(월드변환)를 가진다.
	D3DXMATRIX m_matWorld;
	float mAngle = 0.0f;
};

_END


#endif

