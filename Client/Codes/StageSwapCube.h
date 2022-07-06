#ifndef StageSwapCube_h__
#define StageSwapCube_h__

#include "BasicCube.h"
#include "Player.h"
#include "BackGround.h"

class CStageSwapCube : public CBasicCube
{
protected:
	explicit CStageSwapCube();
	virtual ~CStageSwapCube();

public:
	DeclRTTI(CStageSwapCube, CBasicCube);

	void AddCube(RCP_BASICCUBE _pBasicCube)			{ m_CubeList.push_back(_pBasicCube); }
	void SetPlayer(RCP_PLAYER _pPlayer)				{ m_pPlayer = _pPlayer; }
	void SetBackGround(RCP_BACKGROUND _pBackGround, const _ushort& _wTexIdx, const _char* _pTexTag, const _ushort& _wTargetTexIdx, const _char* _pTargetTexTag);
	void SwapReset();
	void Clear();
	bool GetIsDayOrNight() {return m_bNight;};

public:
	virtual HRESULT Init();
	virtual void	Update();
	void			StageSwap();

public:
	CREATE(CStageSwapCube);

private:
	typedef list<RCP_BASICCUBE> LIST_CUBE;
	LIST_CUBE		m_CubeList;
	RCP_PLAYER		m_pPlayer;
	RCP_BACKGROUND	m_pBackGround;
	_ushort			m_wTexID;
	_ushort			m_wTargetTexID;
	const _char*	m_pTexTag;
	const _char*	m_pTargetTexTag;
	bool			m_bAction;
	float			m_fActionDelay;
	bool			m_bReset;
	bool			m_bNight;

};

TYPEDEF_RCP(CStageSwapCube, STAGESWAPCUBE);

#endif // StageSwapCube_h__
