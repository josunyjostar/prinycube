#ifndef BTMenuBack_h__
#define BTMenuBack_h__

#include "Button.h"

class CBTMenuBack : public CButton
{
protected:
	explicit CBTMenuBack():m_bInterval(false) {}
	virtual ~CBTMenuBack() {}

public:
	DeclRTTI(CBTMenuBack, CButton);

public:	
	void Update() {
		CButton::Update();
		if(!m_bInterval) m_bInterval = true;
		else if(Engine::KeyInput->IsDown(VK_ESCAPE)) this->OnMouseButtonUp();
	}

	virtual void	OnMouseButtonUp()	{
		Engine::CTimerManager::Inst().FindTimer("MainApp_Frame")->SetFrameStop(false);
		Engine::CSceneManager::Inst().GetRunningScene()->Remove_Layer("MenuLayer");
	}

private:
	bool	m_bInterval;

public:
	CREATE(CBTMenuBack);
};

TYPEDEF_RCP(CBTMenuBack, BTMENUBACK);

#endif // BTMenuBack_h__
