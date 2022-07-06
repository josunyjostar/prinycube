void System_Release()
{
	CFontManager::Inst().Release();
	CTimerManager::Inst().Release();
	CGraphicDev::Inst().Release();
}
