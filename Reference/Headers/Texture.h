#ifndef TEXTURE_H__
#define TEXTURE_H__

#include "Component.h"

_BEGIN(Engine)

class _ENGINE_DLL Texture final : public Component
{
public:
	enum TEXTURE_TYPE { GENERAL, CUBE };

public:
	explicit Texture(LPDIRECT3DDEVICE9 pDevice);
	explicit Texture(const Texture& rhs);
	virtual ~Texture() = default;

public:
	HRESULT ReadyTexture(TEXTURE_TYPE type, const TCHAR* pTextureFilePath, const size_t textureNum);
	HRESULT SetUp_Device(const size_t index);

public:
	static Texture* Create(LPDIRECT3DDEVICE9 pDevice, TEXTURE_TYPE type, const TCHAR* pTextureFilePath, const size_t textureNum = 1);
	virtual Component* Clone() override;
	virtual DWORD Free() override;

private:
	vector<IDirect3DBaseTexture9*> mTextureSpace;

};

_END

#endif // TEXTURE_H__
