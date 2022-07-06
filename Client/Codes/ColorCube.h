#ifndef ColorCube_h__
#define ColorCube_h__

#include "BasicCube.h"

class CColorCube : public CBasicCube
{
protected:
	explicit CColorCube();
	virtual ~CColorCube();

public:
	DeclRTTI(CColorCube, CBasicCube);

	void AddCube(RCP_BASICCUBE _pBasicCube)			{ m_CubeList.push_back(_pBasicCube); }
	void SetState(const _vec3& _vState) { m_vState += _vState; }
	_vec3 GetColor() { _vec3 tempColor(m_vColor.x, m_vColor.y, m_vColor.z); return tempColor;}

	void StageSwap(bool bSet);
	void StageSwap();

public:
	virtual HRESULT Init();
	virtual void Update();
	virtual void Render();

private:
	typedef list<RCP_BASICCUBE> LIST_CUBE;
	LIST_CUBE			m_CubeList;
	_vec3				m_vState;
	float				m_fDeltaTime;

public:
	CREATE(CColorCube);
};

TYPEDEF_RCP(CColorCube, COLORCUBE);

#endif // ColorCube_h__
