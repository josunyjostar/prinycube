#ifndef Player_h__
#define Player_h__

#include "Include.h"
#include "Animator.h"
#include "ShadowLight.h"
#include "StageScene.h"

class CPlayer : public Engine::CGameObject
{
public:
	enum ANI_STATE{ ANI_STAND, ANI_PICKUP, ANI_ROTATE, ANI_THROW, ANI_WALK, ANI_JUMP_UP, ANI_JUMP_DOWN, ANI_WIN, ANI_END };

protected:
	explicit CPlayer();
	virtual ~CPlayer();

public:
	DeclRTTI(CPlayer, Engine::CGameObject);

	void				SetShadow(RCP_STAGESCENE _pStageScene, RCP_SHADOWLIGHT _pShadowLight);
	void				SetSpawnPos(const _vec3& _vPos ) { m_vSpawnPos = _vPos; }
	VIEW_DIR			GetViewDir() const		{ return m_eViewDir; }
	RCP_ANIMATOR		GetAnimator() const		{ return m_pAnimator;}
	RCP_TRANSFORM		GetTransform() const	{ return m_pTransform; }
	float				GetCurFoot() const		{ return m_fSwitchFoot; }
	void				SwitchFoot()			{ m_fSwitchFoot *= -1.f; }
	void				ArrowPushStart(_vec3 _scalar)	{ m_vPushScalar = _scalar, m_bArrowPush = true, m_fPushDelay = 1.f; }
	bool				CubeSwapResetComplete() { return m_bCubeSwapReset; }

public:
	virtual HRESULT		Init();
	virtual void		Update();
	virtual void		Render();

	virtual void		Collision(Engine::COL_INFO& _ColInfo);
	virtual void		CollisionExit(Engine::COL_INFO& _ColInfo);

protected:
	RCP_TIMER			m_pTimer;
	RCP_DEVICE			m_pGraphicDev;
	RCP_TRANSFORM		m_pTransform;
	RCP_RENDERER		m_pRenderer;

	Engine::CResourceManager* pResourceMgr;
	RCP_RC_TEX			m_pResource;
	RCP_SHADER			m_pShader;
	RCP_TEXTURE			m_pPlayTexture;
	RCP_ANIMATOR		m_pAnimator;

	RCP_SHADER			m_pShadowCreateShader;
	RCP_D3DTEXTURE		m_pShadowRenderTarget[WALL_END];
	RCP_SURFACE			m_pShadowDepthStencil[WALL_END];
	RCP_SHADOWLIGHT		m_pShadowLight;
	_matrix				m_matLightProj;

	float				m_fDeltaTime;
	_vec3				m_PrevPosition;

	float				m_fMoveSpeed;
	float				m_fVelocity;
	float				m_fJumpPower;

	VIEW_DIR			m_eViewDir;
	VIEW_DIR			m_ePrevViewDir;
	ANI_STATE			m_eState;
	ANI_STATE			m_ePrevState;
	bool				m_bJump;
	bool				m_bWin;

	bool				m_bHide;
	float				m_fHideVal;
	float				m_fSwitchFoot;

	ANI_STATE			m_eOldState;

	bool				m_bArrowPush;
	_vec3				m_vPushScalar;
	float				m_fPushDelay;

	bool				m_bPickUp;
	bool				m_bThrow;

	bool				m_bCubeSwapReset;
	_vec3				m_vSpawnPos;

private:
	void KeyInput();
	void Animation();
	void EffectFootPrint();
	void EffectDust();
	void EffectAfterImage(void* pData);

public:
	CREATE(CPlayer);
};

TYPEDEF_RCP(CPlayer, PLAYER);

#endif // Player_h__
