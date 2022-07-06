#include "stdafx.h"
#include "ocean.h"

COcean::COcean() : A(0.006f), w(_vec2(-2.f, 2.f)), length(15),
geometry(false), g(9.81f), pVertex(0), pIndex(0), h_tilde(0), h_tilde_slopex(0), h_tilde_slopez(0), h_tilde_dx(0), h_tilde_dz(0), fft(0), m_fDeltaTime(0)
,m_pVertexPos(NULL), m_pVertexNormal(NULL)
{
#if defined(_DEBUG) || defined(DEBUG)
	N = 32;
	Nplus1 = N+1;
#else
	N = 64;
	Nplus1 = N+1;
#endif
}

COcean::~COcean() {
	if (h_tilde)		delete[] h_tilde;
	if (h_tilde_slopex)	delete[] h_tilde_slopex;
	if (h_tilde_slopez)	delete[] h_tilde_slopez;
	if (h_tilde_dx)		delete[] h_tilde_dx;
	if (h_tilde_dz)		delete[] h_tilde_dz;
	if (fft)			delete fft;
	m_pVB->Release();
	m_pIB->Release();
	this->release();
}

void COcean::release() {
	//glDeleteBuffers(1, &vbo_indices);
	//glDeleteBuffers(1, &vbo_vertices);
	//releaseProgram(glProgram, glShaderV, glShaderF);
	SAFE_DELETE_ARRAY(m_pVertexPos);
	SAFE_DELETE_ARRAY(m_pVertexNormal);

}
void COcean::SetParamater(const int N, const float A, const _vec2 w, const float length)
{
	this->N = N;
	Nplus1 = (this->N + 1);
	this->A = A;
	this->w = w;
	this->length = length;
}

HRESULT COcean::Init()
{

	m_pTimer = Engine::CTimerManager::Inst().FindTimer("MainApp_Frame");
	m_pGraphicDev = Engine::CGraphicDev::Inst().GetDev();
	m_pTransform = this->AddComponent<Engine::CTransform>();
	m_pRenderer = this->AddComponent<Engine::CRenderer>();
	m_pRenderer->SetRenderType(Engine::RENDER_ALPHA_TEST);
	m_pRenderer->SetZOrder(1);

	m_pShader = Engine::CShaderManager::Inst().FindShader("SHADER_OCEAN");

	h_tilde = new complex[N*N];
	h_tilde_slopex = new complex[N*N];
	h_tilde_slopez = new complex[N*N];
	h_tilde_dx = new complex[N*N];
	h_tilde_dz = new complex[N*N];
	fft = new cFFT(N);

	m_dwVtxCnt = Nplus1 * Nplus1;
	m_dwVtxSize = sizeof(vertex_ocean);
	m_dwVtxFVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
	m_IndexFmt = D3DFMT_INDEX32;
	m_dwTriCnt = N*N * 7;
	m_dwIdxSize = sizeof(unsigned int) * m_dwTriCnt;

	m_pGraphicDev->CreateVertexBuffer(m_dwVtxSize * m_dwVtxCnt, 0, m_dwVtxFVF, D3DPOOL_MANAGED, &m_pVB, NULL);
	m_pGraphicDev->CreateIndexBuffer(m_dwIdxSize, 0, m_IndexFmt, D3DPOOL_MANAGED, &m_pIB, NULL);

	Safe_Delete(m_pVertexPos);
	m_pVertexPos = new _vec3[m_dwVtxCnt];
	Safe_Delete(m_pVertexNormal);
	m_pVertexNormal = new _vec3[m_dwVtxCnt];

	this->AddComponent<Engine::CCubeCollider>()->SetSize(_vec3((_float)length, 0.1f, (_float)length));


	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	int index;

	complex htilde0, htilde0mk_conj;
	for (int i = 0; i < Nplus1; i++) {
		for (int j = 0; j < Nplus1; j++) {
			index = i * Nplus1 + j;

			htilde0 = hTilde_0(j, i);
			htilde0mk_conj = hTilde_0(-j, -i).conj();
			pVertex[index].color = D3DXCOLOR(0.5f, 0.5f, 1.f, 1.f);

			pVertex[index].a = htilde0.a;
			pVertex[index].b = htilde0.b;
			pVertex[index]._a = htilde0mk_conj.a;
			pVertex[index]._b = htilde0mk_conj.b;
			pVertex[index].ox = pVertex[index].x = (j - N / 2.0f) * length / N;
			if(i == 0 || j == N)
				pVertex[index].oy = pVertex[index].y = -0.7f;
			else
				pVertex[index].oy = pVertex[index].y = 0.0f;
			pVertex[index].oz = pVertex[index].z = (i - N / 2.0f) * length / N;

			pVertex[index].nx = 0.0f;
			pVertex[index].ny = 1.0f;
			pVertex[index].nz = 0.0f;

			memcpy(m_pVertexPos[index], ((_vec3*)&pVertex[index].x), sizeof(_vec3));
			memcpy(m_pVertexNormal[index], ((_vec3*)&pVertex[index].nx), sizeof(_vec3));

		}
	}

	iIndexCount = 0;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			index = i * Nplus1 + j;

			if (geometry) {
				pIndex[iIndexCount++] = index;				// lines
				pIndex[iIndexCount++] = index + 1;
				pIndex[iIndexCount++] = index;
				pIndex[iIndexCount++] = index + Nplus1;
				pIndex[iIndexCount++] = index;
				pIndex[iIndexCount++] = index + Nplus1 + 1;
				if (j == N - 1) {
					pIndex[iIndexCount++] = index + 1;
					pIndex[iIndexCount++] = index + Nplus1 + 1;
				}
				if (i == N - 1) {
					pIndex[iIndexCount++] = index + Nplus1;
					pIndex[iIndexCount++] = index + Nplus1 + 1;
				}
			}
			else {
				pIndex[iIndexCount++] = index;				// two triangles
				pIndex[iIndexCount++] = index + Nplus1;
				pIndex[iIndexCount++] = index + Nplus1 + 1;
				pIndex[iIndexCount++] = index;
				pIndex[iIndexCount++] = index + Nplus1 + 1;
				pIndex[iIndexCount++] = index + 1;
			}
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

float COcean::dispersion(int j, int i) {
	float w_0 = 2.0f * M_PI / 200.0f;
	float kx = M_PI * (2 * j - N) / length;
	float kz = M_PI * (2 * i - N) / length;
	return floor(sqrt(g * sqrt(kx * kx + kz * kz)) / w_0) * w_0;
}

float COcean::phillips(int j, int i) {
	_vec2 k(M_PI * (2 * j - N) / length,
		M_PI * (2 * i - N) / length);
	_vec2 temp;
	float k_length = D3DXVec2Length(&k);
	if (k_length < 0.000001) return 0.0;

	float k_length2 = k_length  * k_length;
	float k_length4 = k_length2 * k_length2;
	_vec2 nk, nw;

	D3DXVec2Normalize(&nk, &k); D3DXVec2Normalize(&nw, &w); float k_dot_w = D3DXVec2Dot(&nk, &nw);

	//float k_dot_w = D3DXVec2Dot(D3DXVec2Normalize(&temp, &k), D3DXVec2Normalize(&temp, &w));
	float k_dot_w2 = k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w * k_dot_w;

	float w_length = D3DXVec2Length(&w);
	float L = w_length * w_length / g;
	float L2 = L * L;

	float damping = 0.003f;
	float l2 = L2 * damping * damping;

	return A * exp(-1.0f / (k_length2 * L2)) / k_length4 * k_dot_w2 * exp(-k_length2 * l2);
}

complex COcean::hTilde_0(int j, int i) {
	complex r = gaussianRandomVariable();
	return r * sqrt(phillips(j, i) / 2.0f);
}

complex COcean::hTilde(float t, int j, int i) {
	int index = i * Nplus1 + j;

	complex htilde0(pVertex[index].a, pVertex[index].b);
	complex htilde0mkconj(pVertex[index]._a, pVertex[index]._b);

	float omegat = dispersion(j, i) * t;

	float cos_ = cos(omegat);
	float sin_ = sin(omegat);

	complex c0(cos_, sin_);
	complex c1(cos_, -sin_);

	complex res = htilde0 * c0 + htilde0mkconj * c1;

	return htilde0 * c0 + htilde0mkconj*c1;
}

void COcean::evaluateWavesFFT(float t) {
	m_pVB->Lock(0, 0, (void**)&pVertex, 0);
	m_pIB->Lock(0, 0, (void**)&pIndex, 0);
	float kx, kz, len, lambda = -1.0f;
	int index, index1;

	for (int i = 0; i < N; i++) {
		kz = M_PI * (2.0f * i - N) / length;
		for (int j = 0; j < N; j++) {
			kx = M_PI*(2 * j - N) / length;
			len = sqrt(kx * kx + kz * kz);
			index = i * N + j;

			h_tilde[index] = hTilde(t, j, i);
			h_tilde_slopex[index] = h_tilde[index] * complex(0, kx);
			h_tilde_slopez[index] = h_tilde[index] * complex(0, kz);
			if (len < 0.000001f) {
				h_tilde_dx[index] = complex(0.0f, 0.0f);
				h_tilde_dz[index] = complex(0.0f, 0.0f);
			}
			else {
				h_tilde_dx[index] = h_tilde[index] * complex(0, -kx / len);
				h_tilde_dz[index] = h_tilde[index] * complex(0, -kz / len);
			}
		}
	}

	for (int i = 0; i < N; i++) {
		fft->fft(h_tilde, h_tilde, 1, i * N);
		fft->fft(h_tilde_slopex, h_tilde_slopex, 1, i * N);
		fft->fft(h_tilde_slopez, h_tilde_slopez, 1, i * N);
		fft->fft(h_tilde_dx, h_tilde_dx, 1, i * N);
		fft->fft(h_tilde_dz, h_tilde_dz, 1, i * N);
	}
	for (int j = 0; j < N; j++) {
		fft->fft(h_tilde, h_tilde, N, j);
		fft->fft(h_tilde_slopex, h_tilde_slopex, N, j);
		fft->fft(h_tilde_slopez, h_tilde_slopez, N, j);
		fft->fft(h_tilde_dx, h_tilde_dx, N, j);
		fft->fft(h_tilde_dz, h_tilde_dz, N, j);
	}

	float sign;
	float signs[] = { 1.f, -1.f };
	_vec3 n;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {

			index = i * N + j;		// index into h_tilde..
			index1 = i * Nplus1 + j;	// index into pVertex

			if(i != 0 && j != N)
			{
				sign = signs[(j + i) & 1];

				h_tilde[index] = h_tilde[index] * sign;

				// height
				pVertex[index1].y = h_tilde[index].a;

				// displacement
				h_tilde_dx[index] = h_tilde_dx[index] * sign;
				h_tilde_dz[index] = h_tilde_dz[index] * sign;
				pVertex[index1].x = pVertex[index1].ox + h_tilde_dx[index].a * lambda;
				pVertex[index1].z = pVertex[index1].oz + h_tilde_dz[index].a * lambda;

				// normal
				h_tilde_slopex[index] = h_tilde_slopex[index] * sign;
				h_tilde_slopez[index] = h_tilde_slopez[index] * sign;
				n = _vec3(0.0f - h_tilde_slopex[index].a, 1.0f, 0.0f - h_tilde_slopez[index].a);
				D3DXVec3Normalize(&n, &n);
				pVertex[index1].nx = n.x;
				pVertex[index1].ny = n.y;
				pVertex[index1].nz = n.z;

				memcpy(m_pVertexPos[index1], ((_vec3*)&pVertex[index1].x), sizeof(_vec3));
				memcpy(m_pVertexNormal[index1], ((_vec3*)&pVertex[index1].nx), sizeof(_vec3));

			}
		}
	}
	m_pVB->Unlock();
	m_pIB->Unlock();
}

bool COcean::GetHeightAndNormal(_float& _fHeight, _vec3& _vNormal, _vec3 _vPos)
{
	_matrix World = this->GetComponent<Engine::CTransform>()->GetWorldMatrix();
	_matrix WorldInv;
	D3DXMatrixInverse(&WorldInv, 0, &World);
	D3DXVec3TransformCoord(&_vPos, &_vPos, &WorldInv);

	_float blocklength = length / N;
	_int indexx = _int(_vPos.x / blocklength);
	_int indexz = _int(_vPos.z / blocklength);

	_int index = indexz * Nplus1 + indexx + m_dwVtxCnt/2;
	while(true)
	{
		if(_vPos.x > m_pVertexPos[index].x)
		{
			if(_vPos.x > m_pVertexPos[index+1].x)
			{
				index += 1;
			}
			else
			{
				break;
			}
		}
		else if(_vPos.x < m_pVertexPos[index].x)
		{
			index -= 1;
		}
	}
	while(true)
	{
		if(_vPos.z > m_pVertexPos[index].z)
		{
			if(_vPos.z > m_pVertexPos[index+Nplus1].z)
			{
				index += Nplus1;
			}
			else
			{
				break;
			}
		}
		else if(_vPos.z < m_pVertexPos[index].z)
		{
			index -= Nplus1;
		}
	}
	float zG = m_pVertexPos[index+Nplus1].z - m_pVertexPos[index+1].z;
	float xG = m_pVertexPos[index+Nplus1].x - m_pVertexPos[index+1].x;
	float Gradient = zG / xG;
	if(_vPos.x + _vPos.z / Gradient > 1.f)
	{
		D3DXVec3Cross(&_vNormal, &(m_pVertexNormal[index+Nplus1+1] - m_pVertexNormal[index+Nplus1]), &(m_pVertexNormal[index+1] - m_pVertexNormal[index+Nplus1+1]));
		D3DXVec3TransformNormal(&_vNormal, &_vNormal, &World);
		_fHeight = (m_pVertexPos[index+Nplus1].y + m_pVertexPos[index+Nplus1+1].y + m_pVertexPos[index+1].y)/3.f;
		_fHeight += World._42;
		if(_fHeight < -5.5f || _fHeight > -3.5f)
			_fHeight = _fHeight;
		D3DXVec3Normalize(&_vNormal, &_vNormal);
	}
	else
	{
		D3DXVec3Cross(&_vNormal, &(m_pVertexNormal[index] - m_pVertexNormal[index+Nplus1]), &(m_pVertexNormal[index+1] - m_pVertexNormal[index]));
		D3DXVec3TransformNormal(&_vNormal, &_vNormal, &World);
		_fHeight = (m_pVertexPos[index+Nplus1].y + m_pVertexPos[index].y + m_pVertexPos[index+1].y)/3.f;
		_fHeight += World._42;

		D3DXVec3Normalize(&_vNormal, &_vNormal);
	}
	return true;
}

bool COcean::GetPosAndNormal(_vec3& _vPos, _vec3& _vNormal, _int _iIndex)
{
	_matrix World = this->GetComponent<Engine::CTransform>()->GetWorldMatrix();
	_matrix WorldInv;
	D3DXMatrixInverse(&WorldInv, 0, &World);

	_float blocklength = length / N;

	_int index = _iIndex;

	D3DXVec3Cross(&_vNormal, &(m_pVertexNormal[index+Nplus1+1] - m_pVertexNormal[index+Nplus1]), &(m_pVertexNormal[index+1] - m_pVertexNormal[index+Nplus1+1]));
	D3DXVec3TransformNormal(&_vNormal, &_vNormal, &World);

	_vPos.x = (m_pVertexPos[index+Nplus1].x + m_pVertexPos[index+Nplus1+1].x + m_pVertexPos[index+1].x)/3.f;
	_vPos.y = (m_pVertexPos[index+Nplus1].y + m_pVertexPos[index+Nplus1+1].y + m_pVertexPos[index+1].y)/3.f;
	_vPos.z = (m_pVertexPos[index+Nplus1].z + m_pVertexPos[index+Nplus1+1].z + m_pVertexPos[index+1].z)/3.f;
	D3DXVec3TransformCoord(&_vPos, &_vPos, &World);

	D3DXVec3Normalize(&_vNormal, &_vNormal);

	return true;
}

bool COcean::GetFace(_int& _iIndex, _vec3 _vPos)
{
	_matrix World = this->GetComponent<Engine::CTransform>()->GetWorldMatrix();
	_matrix WorldInv;
	D3DXMatrixInverse(&WorldInv, 0, &World);
	D3DXVec3TransformCoord(&_vPos, &_vPos, &WorldInv);

	_float blocklength = length / N;
	_int indexx = _int(_vPos.x / blocklength);
	_int indexz = _int(_vPos.z / blocklength);

	_int index = indexz * Nplus1 + indexx + m_dwVtxCnt/2;
	while(true)
	{
		if(_vPos.x > m_pVertexPos[index].x)
		{
			if(_vPos.x > m_pVertexPos[index+1].x)
			{
				index += 1;
			}
			else
			{
				break;
			}
		}
		else if(_vPos.x < m_pVertexPos[index].x)
		{
			index -= 1;
		}
	}
	while(true)
	{
		if(_vPos.z > m_pVertexPos[index].z)
		{
			if(_vPos.z > m_pVertexPos[index+Nplus1].z)
			{
				index += Nplus1;
			}
			else
			{
				break;
			}
		}
		else if(_vPos.z < m_pVertexPos[index].z)
		{
			index -= Nplus1;
		}
	}
	_iIndex = index;
	return true;
}

void COcean::Update() {
	m_fDeltaTime += m_pTimer->GetFrameTime();
	evaluateWavesFFT(m_fDeltaTime);
}
void COcean::Render()
{
	_matrix matOceanWorld, matView, matProjection;
	matOceanWorld = m_pTransform->GetWorldMatrix();
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProjection);

	m_pShader->SetFloat("fAlpha", 0.5f);
	m_pShader->SetMatrix("gWorldMatrix", &matOceanWorld);
	m_pShader->SetMatrix("gViewMatrix", &matView);
	m_pShader->SetMatrix("gProjectionMatrix", &matProjection);

	m_pShader->SetVector("gWorldLightPosition", &_vec4(0.f, 10.f, -10.f, 1.f));
	m_pShader->SetVector("gWorldCameraPosition", &_vec4(5.f, 25.f, 55.f, 1.f));


	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	UINT numPasses = 0;
	m_pShader->Begin(&numPasses, NULL);
	{
		for (UINT i = 0; i < numPasses; ++i)
		{
			m_pShader->BeginPass(i);
			{
				unsigned int res = 0;
				res = m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
				res = m_pGraphicDev->SetFVF(m_dwVtxFVF);
				res = m_pGraphicDev->SetIndices(m_pIB);
				res = m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt/3);
			}
			m_pShader->EndPass();
		}
	}
	m_pShader->End();

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, true);
}


float COcean::uniformRandomVariable() {
	return (float)rand() / RAND_MAX;
}

complex COcean::gaussianRandomVariable() {
	float x1, x2, w;
	do {
		x1 = 2.f * uniformRandomVariable() - 1.f;
		x2 = 2.f * uniformRandomVariable() - 1.f;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.f);
	w = sqrt((-2.f * log(w)) / w);
	return complex(x1 * w, x2 * w);
}