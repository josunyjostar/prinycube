#ifndef BackGround_h__
#define BackGround_h__

#include "Include.h"

class CBackGround : public Engine::CGameObject
{
protected:
	explicit CBackGround();
	virtual ~CBackGround();

public:
	DeclRTTI(CBackGround, Engine::CGameObject);

	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag);
	void SwitchTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag, float _fSwitchTime /*= 1.f*/);

public:
	virtual HRESULT Init();
	virtual void	Render();

private:
	RCP_DEVICE		m_pGraphicDev;
	RCP_RESOURCE	m_pResource;
	RCP_SHADER		m_pShader;
	RCP_TEXTURE		m_pTexture;
	RCP_TEXTURE		m_pTargetTexture;
	RCP_TIMER		m_pTimer;
	bool			m_bSwitch;
	float			m_fSwitchTime;
	float			m_fMaxSwitchTime;

public:
	CREATE(CBackGround);
};

TYPEDEF_RCP(CBackGround, BACKGROUND);

#endif // BackGround_h__
