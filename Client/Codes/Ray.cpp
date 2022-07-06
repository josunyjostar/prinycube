#include "stdafx.h"
#include "Ray.h"
#include "BasicCube.h"
#include "ColorCube.h"

CRay::CRay()
: m_pGraphicDev(Engine::CGraphicDev::Inst().GetDev())
, m_pRenderer(AddComponent<Engine::CRenderer>())
, m_vColor(1,1,0)
, m_pCollisionMgr(&Engine::CCollisionManager::Inst())
{
}

CRay::~CRay()
{
}

HRESULT CRay::Init()
{
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_BLEND);
	m_pRenderer->SetZOrder(1);
	return S_OK;
}

enum RayCol{RAY_NOCOL = 0, RAY_MIRROR, RAY_COLOR, RAY_TILE};

void CRay::Update()
{
	m_RayList.clear();
	Normalize(m_StartRay.Dir);
	_float fDest = 100.f;
	_float fsqrDest = 10000.f;

	RCP_TRANSFORM pTransform;
	_vec3 vNormal;

	Engine::COL_INFO MirrorColInfo;
	MirrorColInfo = Engine::RayCast->Picking(m_StartRay, "Mirror");
	Engine::COL_LIST* pColList = m_pCollisionMgr->GetColList("Mirror");

	Engine::COL_LIST::iterator Iter		= pColList->begin();
	Engine::COL_LIST::iterator EndIter	= pColList->end();

	CBasicCube* PrevCol;

	_int iCol = 0;
	_bool bCol = false;

	while(Iter != EndIter)
	{
		if(((CBasicCube*)(*Iter)->GetGameObject())->GetNormalAndDest(m_StartRay, vNormal, fDest))
		{
			fsqrDest = SQR(fDest);

			if(fsqrDest < 10000.f)
			{
				PrevCol = ((CBasicCube*)(*Iter)->GetGameObject());
				iCol = RAY_MIRROR;
			}

		}
		Iter++;
	}
	Engine::COL_INFO TileColInfo = Engine::RayCast->Picking(m_StartRay, "Tile");
	if(TileColInfo.pObj)
	{
		_float tempDest = sqrMagnitude(m_StartRay.Pos - TileColInfo.vColPos);
		if(IsDynamicCast(CColorCube, TileColInfo.pObj))
		{
			if(tempDest <= fsqrDest)
			{
				iCol = RAY_COLOR;
				bCol = true;
			}
		}
		else
		{		
			if(tempDest <= fsqrDest)
			{
				iCol = RAY_TILE;
				m_vEndPos = TileColInfo.vColPos;
				bCol = true;
			}
		}
	}
	if(!bCol)
	{
		TileColInfo = Engine::RayCast->Picking(m_StartRay, "Wall");
		if(TileColInfo.pObj)
		{
			_float tempDest = sqrMagnitude(m_StartRay.Pos - TileColInfo.vColPos);
			if(tempDest <= fsqrDest)
			{
				iCol = RAY_TILE;
			}
		}
	}

	switch(iCol)
	{
	case RAY_NOCOL:
		m_vEndPos = m_StartRay.Pos + m_StartRay.Dir * 100.f;
		break;
	case RAY_MIRROR:
		{
			_vec3 reflect = (2.f * Dot(-m_StartRay.Dir, vNormal) * vNormal) + m_StartRay.Dir;
			Normalize(reflect);
			_vec3 NextPos = m_StartRay.Pos + m_StartRay.Dir * fDest;
			_ray NextRay(&NextPos, &reflect);

			m_RayList.push_back(NextRay);
			m_vEndPos = NextRay.Pos + NextRay.Dir * 300.f;
		}
		break;
	case RAY_COLOR:
		((CColorCube*)TileColInfo.pObj)->SetState(m_vColor);
		m_vEndPos = TileColInfo.vColPos;
		break;
	case RAY_TILE:
		m_vEndPos = TileColInfo.vColPos;
		break;
	}

	list<_ray>::iterator iter = m_RayList.begin();
	list<_ray>::iterator iterend = m_RayList.end();

	iCol = 0;
	fDest = 100.f;
	fsqrDest = 10000.f;
	_bool bSizeOver = false;
	if(iter != iterend)
	{
		for(iter; iter!=iterend; ++iter)
		{
			iCol = 0;
			fDest = 1000.f;
			fsqrDest = 10000.f;
			if(bSizeOver) break;

			_ray temp = (*iter);
			temp.Pos += temp.Dir*1.f;
			bCol = false;

			Iter	= pColList->begin();

			_int iCol = 0;
			_bool bCol = false;
			CBasicCube* PrevColTemp;

			while(Iter != EndIter)
			{
				if(((*Iter)->GetGameObject()) == PrevCol)
				{
					Iter++;
					continue;
				}
				if(((CBasicCube*)(*Iter)->GetGameObject())->GetNormalAndDest((*iter), vNormal, fDest))
				{
					fsqrDest = SQR(fDest);

					if(fsqrDest < 10000.f)
					{
						iCol = RAY_MIRROR;
						PrevColTemp = ((CBasicCube*)((*Iter)->GetGameObject()));
					}

				}
				Iter++;
			}

			TileColInfo = Engine::RayCast->Picking((*iter), "Tile");
			if(TileColInfo.pObj)
			{
				_float tempDest = sqrMagnitude(iter->Pos - TileColInfo.vColPos);
				if(IsDynamicCast(CColorCube, TileColInfo.pObj))
				{
					if(tempDest <= fsqrDest)
					{
						iCol = RAY_COLOR;
						bCol = true;
					}
				}
				else
				{		
					if(tempDest <= fsqrDest)
					{
						iCol = RAY_TILE;
						bCol = true;
					}
				}
			}
			if(!bCol)
			{
				TileColInfo = Engine::RayCast->Picking((*iter), "Wall");
				if(TileColInfo.pObj)
				{
					_float tempDest = sqrMagnitude(iter->Pos - TileColInfo.vColPos);
					if(tempDest <= fsqrDest)
					{
						iCol = RAY_TILE;
					}
				}
			}



			switch(iCol)
			{
			case RAY_NOCOL:
				m_vEndPos = iter->Pos + iter->Dir * 300;
				break;
			case RAY_MIRROR:
				{
					_vec3 reflect = (2.f * Dot(-iter->Dir, vNormal) * vNormal) + iter->Dir;
					Normalize(reflect);
					_vec3 NextPos = iter->Pos +  iter->Dir * fDest;
					m_vEndPos = NextPos;
					_ray NextRay(&NextPos, &reflect);


					m_RayList.push_back(NextRay);
					m_vEndPos = NextRay.Pos + NextRay.Dir * 300;

					PrevCol = PrevColTemp;
				}
				break;
			case RAY_COLOR:
				((CColorCube*)TileColInfo.pObj)->SetState(m_vColor);
				m_vEndPos = TileColInfo.vColPos;
				break;
			case RAY_TILE:
				m_vEndPos = TileColInfo.vColPos;
				break;
			}
		}
	}
}

void CRay::Render()
{
 	m_pDraw->SetColor(m_vColor);

	list<_ray>::iterator iter = m_RayList.begin();
	list<_ray>::iterator iterend = m_RayList.end();

	m_pDraw->SetRay(m_StartRay);
	if(iter == iterend)
		m_pDraw->SetRayEnd(m_vEndPos);
	else
		m_pDraw->SetRayEnd(iter->Pos);
	m_pDraw->Render();


	for(iter; iter!=iterend;)
	{
		m_pDraw->SetRay((*iter));
		++iter;
		if(iter == iterend)
			m_pDraw->SetRayEnd(m_vEndPos);
		else
			m_pDraw->SetRayEnd(iter->Pos);
		m_pDraw->Render();
	}
}