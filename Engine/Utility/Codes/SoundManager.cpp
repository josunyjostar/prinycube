#include "SoundManager.h"
using namespace Engine;

CSoundManager::CSoundManager()
:m_fMasterVolume(1.f)
{
	memset(m_pCh, NULL, sizeof(m_pCh));
	memset(m_pSound, NULL, sizeof(m_pSound));
	memset(m_iCount, 0, sizeof(m_iCount));

	if(!this->ErrorCheck(System_Create(&m_pFmod))) {
		m_bSoundHardwareError = true;
		return;
	}
	if(!this->ErrorCheck(m_pFmod->init(100, FMOD_INIT_NORMAL, NULL))) {
		m_bSoundHardwareError = true;
		return;
	}
	m_bSoundHardwareError = false;
}

CSoundManager::~CSoundManager()
{
	this->Release();
}

void CSoundManager::SetVolume(float _fMasterVolume)
{ 
	m_fMasterVolume = _fMasterVolume;
	bool temp;
	for (int i = 0; i < MAX_SOUND; ++i) {
		for(int j = 0; j< m_iCount[i]; ++j) {
			m_pCh[i][j]->isPlaying(&temp);
			if (temp == true) m_pCh[i][j]->setVolume(1.f*m_fMasterVolume);
		}
	}
}

bool CSoundManager::IsPlay(int _type)
{
	bool temp = false;
	for(int i = 0; i < m_iCount[_type]; ++i) {
		m_pCh[_type][i]->isPlaying(&temp);
		if (temp == true) break;
	}
	return temp;
}

Channel* CSoundManager::Play(int _type)
{
	if(m_bSoundHardwareError) return NULL;

	if (m_pSound[_type]) {
		if(!this->ErrorCheck(m_pFmod->update())) return NULL;
		int randIndex = rand()%m_iCount[_type];
		if(!this->ErrorCheck(m_pFmod->playSound(FMOD_CHANNEL_FREE, m_pSound[_type][randIndex], false, &m_pCh[_type][randIndex]))) return NULL;
		m_pCh[_type][randIndex]->setVolume(1.f*m_fMasterVolume);
		return m_pCh[_type][randIndex];
	}
	return NULL;
}

Channel* CSoundManager::Play(int _type, const _vec3& _vSoundPos, const _vec3& _vListenerPos)
{
	if(m_bSoundHardwareError) return NULL;

	if (m_pSound[_type]) {
		if(!this->ErrorCheck(m_pFmod->update())) return NULL;
		int randIndex = rand()%m_iCount[_type];
		if(!this->ErrorCheck(m_pFmod->playSound(FMOD_CHANNEL_FREE, m_pSound[_type][randIndex], false, &m_pCh[_type][randIndex]))) return NULL;
		
		float fSD = sqrDistance(_vSoundPos, _vListenerPos);
		
		if(fSD < 100.f)			m_pCh[_type][randIndex]->setVolume(1.f*m_fMasterVolume);
		else if(fSD > 10000.f)	m_pCh[_type][randIndex]->setVolume(0.f*m_fMasterVolume);
		else					m_pCh[_type][randIndex]->setVolume((fSD-100.f)/9900.f*m_fMasterVolume);
		
		return m_pCh[_type][randIndex];
	}
	return NULL;
}

void CSoundManager::Stop(int _type)
{
	bool temp;
	for(int i = 0; i < m_iCount[_type]; ++i) {
		m_pCh[_type][i]->isPlaying(&temp);
		if (temp == true) this->ErrorCheck(m_pCh[_type][i]->stop());
	}
}

void CSoundManager::AllStop()
{
	bool temp;
	for (int i = 0; i < MAX_SOUND; ++i) {
		for(int j = 0; j< m_iCount[i]; ++j) {
			m_pCh[i][j]->isPlaying(&temp);
			if (temp == true) this->ErrorCheck(m_pCh[i][j]->stop());
		}
	}
}

bool CSoundManager::ErrorCheck(FMOD_RESULT _result)
{
	if (_result != FMOD_OK){
		TCHAR szStr[256] = { 0 };
		MultiByteToWideChar(CP_ACP, NULL, FMOD_ErrorString(_result), -1, szStr, 256);
		return false;
	}
	return true;
}

#include "stdio.h"
void CSoundManager::AddSound(const char* _szPath, int _mode, int _type, int _cnt /*= 1*/, int _startNum /*= 0*/)
{
	for(int i = 0; i < _cnt; ++i)
	{
		char szFileName[256] = "";
		sprintf_s(szFileName, _szPath, i+_startNum);
		if(this->ErrorCheck(m_pFmod->createSound(szFileName, _mode, NULL, &m_pSound[_type][m_iCount[_type]]))) ++m_iCount[_type];
	}
}

void CSoundManager::Release()
{
	if(!m_pFmod) return;
	this->AllStop();
	for (int i = 0; i < MAX_SOUND; i++) {
		for(int j = 0; j< m_iCount[i]; ++j)
			if (m_pSound[i][j]) m_pSound[i][j]->release();
		m_iCount[i] = 0;
	}
	m_pFmod->close();
	m_pFmod->release();
	m_pFmod = NULL;
}