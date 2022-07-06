#ifndef StageScene_h__
#define StageScene_h__

#include "Include.h"
#include "ShadowLight.h"


class CStageScene : public Engine::CScene
{
protected:
	explicit CStageScene();
	virtual ~CStageScene();

public:
	DeclRTTI(CStageScene, Engine::CScene);

	RCP_D3DTEXTURE* GetShadowRenderTarget()		{ return m_pShadowRenderTarget; }
	RCP_SURFACE*	GetShadowDepthBuffer()		{ return m_pShadowDepthStencil; }
	RCP_SHADOWLIGHT	GetShadowLight() const		{ return m_pShadowLight; }
	_matrix			GetLightProjMatrix() const	{ return m_matLightProj; }

public:
	virtual HRESULT Init();
	virtual void	Update();
	virtual void	Reset()	{ 
		this->Remove_Layer("MenuLayer");
		m_pTimer->SetFrameStop(false);

		m_pFadeInOutFilter->FadeIn(1.f, FADE_IN);
		m_pFadeInOutFilter->SetColor(_vec4(1,1,1,1));
		m_pFadeInOutFilter->SetTargetColor(_vec4(1,1,1,1));
		Engine::CSceneManager::Inst().StartScene(this->GetTag(), 1.f, false);
	}
	virtual void	ClickEvent();

protected:
	RCP_TIMER		m_pTimer;
	RCP_DEVICE		m_pDev;
	RCP_D3DTEXTURE	m_pShadowRenderTarget[WALL_END];
	RCP_SURFACE		m_pShadowDepthStencil[WALL_END];
	RCP_SHADOWLIGHT	m_pShadowLight;
	_matrix			m_matLightView[WALL_END];
	_matrix			m_matLightProj;
	_matrix			m_matView;
	_matrix			m_matProj;
	float			m_fDeltaTime;

	RCP_FADEINOUTFILTER m_pFadeInOutFilter;

protected:
	virtual void InitSeaFood();
	virtual void RegenSeaFood();
	virtual void MakeFish();
	virtual void MakeJellyFish();

	virtual void InitBubble();
	virtual void RegenBubble();
	virtual	void MakeBubble();
	virtual	void MakeBubbleEX();

public:
	CREATE(CStageScene);
};

TYPEDEF_RCP(CStageScene, STAGESCENE);

#endif // StageScene_h__
