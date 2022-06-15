#include "Texture.h"

_USING(Engine);


Texture::Texture(LPDIRECT3DDEVICE9 pDevice)
	: Component(pDevice)
{
}

Texture::Texture(const Texture & rhs)
	: Component(rhs)
	, mTextureSpace(rhs.mTextureSpace)
{
	for (IDirect3DBaseTexture9* pTexture : mTextureSpace)
	{
		pTexture->AddRef();
	}
}

HRESULT Texture::ReadyTexture(TEXTURE_TYPE type, const TCHAR * pTextureFilePath, const size_t textureNum)
{
	// (L"../Bin/Texture/Effect/Effect.png", 10)

	TCHAR fullPath[MAX_PATH] = L"";

	mTextureSpace.reserve(textureNum);

	for (size_t i = 0; i < textureNum; i++)
	{
		IDirect3DBaseTexture9* pTexture = nullptr;

		wsprintf(fullPath, pTextureFilePath, i);

		switch (type)
		{
		case GENERAL:
			if (FAILED(D3DXCreateTextureFromFile(m_pDevice, fullPath, (LPDIRECT3DTEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		case CUBE:
			if (FAILED(D3DXCreateCubeTextureFromFile(m_pDevice, fullPath, (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
				return E_FAIL;
			break;
		}

		mTextureSpace.emplace_back(pTexture);
	}

	return NOERROR;
}

HRESULT Texture::SetUp_Device(const size_t index)
{
	if (mTextureSpace.size() <= index)
		return E_FAIL;

	if (mTextureSpace[index] != nullptr)
		m_pDevice->SetTexture(0, mTextureSpace[index]);

	return NOERROR;
}

Texture * Texture::Create(LPDIRECT3DDEVICE9 pDevice, TEXTURE_TYPE type, const TCHAR * pTextureFilePath, const size_t textureNum)
{
	Texture* pInstance = new Texture(pDevice);

	if (FAILED(pInstance->ReadyTexture(type, pTextureFilePath, textureNum)))
	{
		_MSG_BOX("Texture Created Failed");
		Safe_Release(pInstance);
	}
	return pInstance;
}

Component * Texture::Clone()
{
	return new Texture(*this);
}

DWORD Texture::Free(void)
{
	for (IDirect3DBaseTexture9* pTexture : mTextureSpace)
	{
		Safe_Release(pTexture);
	}
	mTextureSpace.clear();

	Component::Free();

	return DWORD();
}
