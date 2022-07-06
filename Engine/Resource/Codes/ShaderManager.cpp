#include "ShaderManager.h"
using namespace Engine;

CShaderManager::CShaderManager()
:m_hShader(64)
{
}

CShaderManager::~CShaderManager()
{
	this->Release();
}

RCP_SHADER CShaderManager::CreateShader(const _char* _pShaderTag, const _char* _pFilePath)
{
	if(!_pShaderTag || !_pFilePath) return NULL;

	RCP_SHADER& pShader = m_hShader[_pShaderTag];
	if(pShader) return pShader;

	LPD3DXEFFECT ret = NULL;
	D3DXCreateEffectFromFileA(&*m_pDev, _pFilePath, NULL, NULL, NULL, NULL, &ret, NULL);

	pShader = ret;
	pShader->Release();

	return pShader;
}

void CShaderManager::Release()
{
	m_hShader.Clear(true);
	m_pDev = NULL;
}