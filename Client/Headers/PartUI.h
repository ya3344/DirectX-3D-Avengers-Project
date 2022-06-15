#ifndef PART_UI_H__
#define PART_UI_H__

#include "UI.h"

_BEGIN(Client)

class PartUI final : public UI
{
private:
	explicit PartUI(LPDIRECT3DDEVICE9 pDevice);
	explicit PartUI(const PartUI& rhs);
	virtual ~PartUI() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

public:
	static PartUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual PartUI* Clone() override;
	virtual DWORD Free() override;

private:
	float mAcceleration = 0.f;


};

_END

#endif

