#ifndef BTMenu_h__
#define BTMenu_h__

#include "Button.h"
#include "Menu.h"

class CBTMenu : public CButton
{
protected:
	explicit CBTMenu() {}
	virtual ~CBTMenu() {}

public:
	DeclRTTI(CBTMenu, CButton);

	virtual HRESULT Init() {
		CButton::Init();

		this->SetTexture(RESOURCE_STATIC, "TEX_PAUSE");

		this->SetZOrder(9);
		this->SetScale(_vec2(96.f, 96.f));
		this->SetPosition(_vec2(50.f, 50.f));
		return S_OK;
	}

	void Update() {
		if(!this->GetActive()) return;
		CButton::Update();
		if(Engine::KeyInput->IsDown(VK_ESCAPE)) this->OnMouseButtonUp();
	}

	virtual void	OnMouseButtonUp()	{
		if(!this->GetActive()) return;
		Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("MenuLayer")->Add_GameObject<CMenu>(true)->SetMenuButton(this);
		this->SetActive(false);
	}

public:
	CREATE(CBTMenu);
};

TYPEDEF_RCP(CBTMenu, BTMENU);

#endif // BTMenu_h__
