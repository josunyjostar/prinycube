#ifndef SoundManager_h__
#define SoundManager_h__

#include "Engine_Include.h"
#include "TemplateSingleton.h"

#include "fmod.hpp"
#include "fmod_errors.h"

#ifdef _DEBUG
#pragma comment(lib, "fmodexL_vc")
#else
#pragma comment(lib, "fmodex_vc") 
#endif

using namespace FMOD;

#define MAX_SOUND		1024
#define MAX_TYPE		64

BEGIN(Engine)

class ENGINE_DLL CSoundManager : public TSingleton <CSoundManager>
{
private:
	explicit CSoundManager();
	virtual ~CSoundManager();

public:
	friend TSingleton;

	void		SetVolume(float _fMasterVolume);
	float		GetVolume() const { return m_fMasterVolume; }
	bool		IsPlay(int _type);

public:
	Channel*	Play(int _type);
	Channel*	Play(int _type, const _vec3& _vSoundPos, const _vec3& _vListenerPos);
	void		Stop(int _type);
	void		AllStop();
	void		AddSound(const char* _szPath, int _mode, int _type, int _cnt = 1, int _startNum = 0);

private:
	bool		m_bSoundHardwareError;
	System*		m_pFmod;
	Channel*	m_pCh[MAX_SOUND][MAX_TYPE];
	Sound*		m_pSound[MAX_SOUND][MAX_TYPE];
	int			m_iCount[MAX_SOUND];
	FMOD_RESULT m_Result;
	float		m_fMasterVolume;

private:
	bool		ErrorCheck(FMOD_RESULT _result);

public:
	void		Release();
};

extern CSoundManager* const SoundMgr;

END

#endif // SoundManager_h__
