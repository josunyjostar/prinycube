void Resource_Release()
{
	CShaderManager::Inst().Release();
	CResourceManager::Inst().Release();
}