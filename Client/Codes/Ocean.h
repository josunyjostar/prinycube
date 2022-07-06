#ifndef Ocean_h__
#define Ocean_h__

#include "fft.h"
#include "Include.h"

class COcean : public Engine::CGameObject
{
private:
	bool geometry;		 

	float g;			
	int N, Nplus1;		 
	float A;			 
	_vec2 w;			
	float length;		
	complex *h_tilde,	
		*h_tilde_slopex, *h_tilde_slopez,
		*h_tilde_dx, *h_tilde_dz;
	cFFT *fft;				

	float m_fDeltaTime;
	_vec3*	m_pVertexPos;
	_vec3*	m_pVertexNormal;

	RCP_TIMER			m_pTimer;
	RCP_DEVICE			m_pGraphicDev;
	RCP_TRANSFORM		m_pTransform;
	RCP_RENDERER		m_pRenderer;
	RCP_SHADER			m_pShader;

	unsigned long						m_dwVtxSize;
	unsigned long						m_dwVtxCnt;
	unsigned long						m_dwVtxFVF;
	unsigned long						m_dwIdxSize;
	unsigned long						m_dwTriCnt;
	D3DFORMAT							m_IndexFmt;
	LPDIRECT3DVERTEXBUFFER9				m_pVB;
	LPDIRECT3DINDEXBUFFER9				m_pIB;
	vertex_ocean *pVertex;			
	unsigned int *pIndex;			
	unsigned int iIndexCount;		

	int vertex, normal, texture, light_position, projection, view, model;	

protected:
public:
	DeclRTTI(COcean, Engine::CGameObject);

	explicit COcean();
	virtual ~COcean();
	void release();

	virtual HRESULT		Init();
	virtual void		Update();
	virtual void		Render();

	void SetParamater(const int N, const float A, const _vec2 w, const float length);
	float dispersion(int j, int i);		
	float phillips(int j, int i);	
	complex hTilde_0(int j, int i);
	complex hTilde(float t, int j, int i);
	void evaluateWavesFFT(float t);

	bool GetHeightAndNormal(_float& _fHeight, _vec3& _vNormal, _vec3 _vPos);
	bool GetFace(_int& _iIndex, _vec3 _vPos);
	bool GetPosAndNormal(_vec3& _vPos, _vec3& _vNormal, _int _iIndex);


	float uniformRandomVariable();
	complex gaussianRandomVariable();

public:
	CREATE(COcean);
};

TYPEDEF_RCP(COcean, OCEAN);

#endif