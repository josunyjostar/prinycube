#ifndef BTSceneChange_h__
#define BTSceneChange_h__

#include "Button.h"

class CBTSceneChange : public CButton
{
protected:
	explicit CBTSceneChange()
		:m_pSceneName(NULL)
		,m_bClear(true)
		,m_vFadeColor(0,0,0,1)
		,m_vFadeTargetColor(0,0,0,1)
		,m_fSceneChangeTime(0.5f)
		,m_eFilterType(FADE_NORMAL)
		,m_bAction(false)
		,m_bDisable(false) {}
	virtual ~CBTSceneChange() {}

public:
	DeclRTTI(CBTSceneChange, CButton);

	void SetDisable(bool _bDisable)								{ m_bDisable = _bDisable; }
	void SetFadeColor(const _vec4& _vFadeColor)					{ m_vFadeColor = _vFadeColor; }
	void SetFadeTargetColor(const _vec4& _vFadeTargetColor)		{ m_vFadeTargetColor = _vFadeTargetColor; }
	void SetScene(const char* _pSceneName, bool _bClear = true) { 
		m_pSceneName = _pSceneName; 
		m_bClear = _bClear;
	}
	void SetFadeFilter(float _fTime, FADE_FILTER_TYPE _eFilterType = FADE_NORMAL) {
		m_fSceneChangeTime = _fTime;
		m_eFilterType = _eFilterType;
	}
	template<typename T>
	void SetScene(bool _bClear = false) {
		Engine::CSceneManager::Inst().CreateScene<T>();
		m_pSceneName = T::GetRTTIInst()->GetClassName();
		m_bClear = _bClear;
	}

public:
	virtual void	OnMouseButtonUp()	{
		if(m_bDisable) {
			// Play Button Error Sound
			return;
		}
		if(m_bAction) return;
		Engine::SoundMgr->Play(SND_SCENE_CHANGE);
		Engine::CSceneManager::Inst().GetRunningScene()->Remove_Layer("MenuLayer");
		Engine::CTimerManager::Inst().FindTimer("MainApp_Frame")->SetFrameStop(false);
		m_bAction = true;
		RCP_FADEINOUTFILTER pFadeInOutFilter = Engine::CSceneManager::Inst().GetRunningScene()->Create_Layer("UI_Layer")->Add_GameObject<CFadeOutFilter>(true);
		pFadeInOutFilter->SetColor(m_vFadeColor);
		pFadeInOutFilter->SetTargetColor(m_vFadeTargetColor);
		pFadeInOutFilter->FadeIn(m_fSceneChangeTime, m_eFilterType);
		Engine::CSceneManager::Inst().StartScene(m_pSceneName, m_fSceneChangeTime, m_bClear);
	}

private:
	const char*			m_pSceneName;
	bool				m_bClear;
	_vec4				m_vFadeColor;
	_vec4				m_vFadeTargetColor;
	float				m_fSceneChangeTime;
	FADE_FILTER_TYPE	m_eFilterType;
	bool				m_bAction;
	bool				m_bDisable;

public:
	CREATE(CBTSceneChange);
};

TYPEDEF_RCP(CBTSceneChange, BTSCENECHANGE);

#endif // BTSceneChange_h__
