#ifndef UI_h__
#define UI_h__

#include "Include.h"

class CUI : public Engine::CGameObject
{
protected:
	explicit CUI();
	virtual ~CUI();

public:
	DeclRTTI(CUI, Engine::CGameObject);

	void SetTexture(const _ushort& _wContainerIdx, const _char* _pResourceTag);
	void SetPosition(_vec3 _vUIPos);
	void SetPosition(_vec2 _vUIPos);
	void SetScale(_vec3 _vUIScale);
	void SetScale(_vec2 _vUIScale);
	void SetTexIndex(int _iTexIndex)	{ m_iTexIndex = _iTexIndex; }
	void SetColor(_vec4 _vColor)		{ m_vColor = _vColor; }
	void SetZOrder(int _iZOrder)		{ this->AddComponent<Engine::CRenderer>()->SetZOrder(_iZOrder); }

public:
	virtual HRESULT Init();
	virtual void	Render();
public:
	RCP_DEVICE		m_pGraphicDev;
	RCP_TRANSFORM	m_pTransform;
	RCP_RESOURCE	m_pResource;
	RCP_SHADER		m_pShader;
	RCP_TEXTURE		m_pTexture;
	_vec4			m_vColor;
	int				m_iTexIndex;

public:
	CREATE(CUI);
};

TYPEDEF_RCP(CUI, UI);

#endif // UI_h__
