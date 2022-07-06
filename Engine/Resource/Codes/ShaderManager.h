#ifndef ShaderManager_h__
#define ShaderManager_h__

#include "Engine_Include.h"
#include "TemplateSingleton.h"
#include "TinyHashTable.h"

BEGIN(Engine)

class ENGINE_DLL CShaderManager : public TSingleton<CShaderManager>
{
private:
	explicit CShaderManager();
	virtual ~CShaderManager();

public:
	friend TSingleton;

	void			Init(RCP_DEVICE& _pDev)					{ m_pDev = _pDev; }
	RCP_SHADER		FindShader(const _char* _pShaderTag)	{ return m_hShader[_pShaderTag]; }
	RCP_SHADER		CreateShader(const _char* _pShaderTag, const _char* _pFilePath);

private:
	typedef TinyHashTable<const _char*, RCP_SHADER>	HASH_SHADER;
	HASH_SHADER		m_hShader;

	RCP_DEVICE		m_pDev;

public:
	void			Release();
};

END

#endif // ShaderManager_h__
