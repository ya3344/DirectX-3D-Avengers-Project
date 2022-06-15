#ifndef FULL_UI_H__
#define FULL_UI_H__

#include "UI.h"

_BEGIN(Client)

class FullUI final : public UI
{
private:
	explicit FullUI(LPDIRECT3DDEVICE9 pDevice);
	explicit FullUI(const FullUI& rhs);
	virtual ~FullUI() = default;

public:
	virtual HRESULT Init() override;
	virtual UPDATE Update(const float timeDelta) override;
	virtual UPDATE LastUpdate(const float timeDelta) override;
	virtual void Render() override;
	virtual HRESULT ReadyPrototype() override;

public:
	static FullUI* Create(LPDIRECT3DDEVICE9 pDevice);
	virtual FullUI* Clone() override;
	virtual DWORD Free() override;
};

_END

#endif

