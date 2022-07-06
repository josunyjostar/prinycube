#include "Texture.h"
#include "ResourceManager.h"
using namespace Engine;

#include <string>

CTexture::CTexture()
:m_ppTexture(NULL)
,m_TextureSize(0)
{
}

CTexture::CTexture(const CTexture& rhs)
:CResource(rhs)
{
	m_TextureSize = rhs.m_TextureSize;
	m_ppTexture = new IDirect3DBaseTexture9*[m_TextureSize];

	memcpy(m_ppTexture, rhs.m_ppTexture, sizeof(IDirect3DBaseTexture9*)*m_TextureSize);

	for (_ushort i = 0; i < m_TextureSize; ++i)
		m_ppTexture[i]->AddRef();
}

CTexture::~CTexture()
{
	for(_ushort i = 0; i < m_TextureSize; ++i)
		Safe_Release(m_ppTexture[i]);
	Safe_Delete_Array(m_ppTexture);
}

CTexture& CTexture::operator=(const CTexture& rhs)
{
	CResource::operator=(rhs);
	if(m_pGraphicDev)
	{
		for(_ushort i = 0; i < m_TextureSize; ++i)
			Safe_Release(m_ppTexture[i]);
		Safe_Delete_Array(m_ppTexture);

		m_TextureSize = rhs.m_TextureSize;
		m_ppTexture = new IDirect3DBaseTexture9*[m_TextureSize];

		memcpy(m_ppTexture, rhs.m_ppTexture, sizeof(IDirect3DBaseTexture9*)*m_TextureSize);

		for (_ushort i = 0; i < m_TextureSize; ++i)
			m_ppTexture[i]->AddRef();
	}
	return *this;
}

void CTexture::Render(RCP_RESOURCE _pResource, const _ushort& iIndex /*= 0*/)
{
	if(m_TextureSize > iIndex)
		m_pGraphicDev->SetTexture(0, m_ppTexture[iIndex]);
	_pResource->Render();
	m_pGraphicDev->SetTexture(0, NULL);
}

HRESULT	CTexture::CreateTexture(TEXTURE_TYPE _eType, const _char* _pFilePath, _ushort _wCnt /*= 1*/, _ushort _wStartNumber /*= 0*/)
{
	for(_ushort i = 0; i < m_TextureSize; ++i)
		Safe_Release(m_ppTexture[i]);
	Safe_Delete_Array(m_ppTexture);

	m_TextureSize = _wCnt;
	m_ppTexture = new IDirect3DBaseTexture9*[m_TextureSize];

	for(_ushort i = 0; i < m_TextureSize; ++i)
	{
		char szFileName[256] = "";

		sprintf_s(szFileName, _pFilePath, i+_wStartNumber);	

		switch(_eType)
		{
		case TEX_NORMAL:
			if(FAILED(D3DXCreateTextureFromFileA(&*m_pGraphicDev, szFileName, (LPDIRECT3DTEXTURE9*)&m_ppTexture[i])))
			{
				ERR_MSG("Normal Texture Load Failed");
				return E_FAIL;
			}
			break;
		case TEX_CUBE:
			if(FAILED(D3DXCreateCubeTextureFromFileA(&*m_pGraphicDev, szFileName, (LPDIRECT3DCUBETEXTURE9*)&m_ppTexture[i])))
			{
				ERR_MSG("Cube Texture Load Failed");
				return E_FAIL;
			}
			break;
		}
	}

	return S_OK;
}