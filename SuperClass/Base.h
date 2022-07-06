#ifndef BASE_H__
#define BASE_H__

#include "Engine_Macro.h"
#include "SmartPointer.h"
#include "RTTI.h"

class ENGINE_DLL CBase abstract
{
protected:
	explicit CBase():m_RefCnt(0),m_bActive(false),m_Tag(NULL) {}
	explicit CBase(const CBase& rhs):m_RefCnt(0),m_bActive(rhs.m_bActive),m_Tag(rhs.m_Tag) {}
	virtual ~CBase() {}

public:
	template<typename> friend class RCPtrToConst;
	DeclRootRTTI(CBase);

	CBase& operator=(const CBase& rhs)	{return *this;}

public:
	void			SetTag(const char* _tag)			{m_Tag = _tag;}
	void			SetTag(const CBase& _Obj)			{m_Tag = _Obj.m_Tag;}
	void			SetActive(bool _bActive)			{m_bActive = _bActive;(_bActive)?this->Enable():this->Disable();}

	const char*		GetTag() const						{return m_Tag;}
	bool			GetActive() const					{return m_bActive;}

	bool			CompareTag(const CBase& _Obj) const	{return m_Tag == _Obj.m_Tag;}
	bool			CompareTag(const char* _Tag) const	{return m_Tag == _Tag;}

public:
	virtual void	Enable()	{}
	virtual void	Disable()	{}

	virtual HRESULT	Init()		{ return S_OK; }
	virtual void	Update()	{}
	virtual void	Render()	{}
	virtual void	Clear()		{}

public:
	unsigned long	AddRef()	{ return ++m_RefCnt; }
	unsigned long	Release()	{ if(!--m_RefCnt) {delete this; return 0;} return m_RefCnt; }

private:
	unsigned long	m_RefCnt;
	bool			m_bActive;

protected:
	const char*		m_Tag;

public:
	template<typename T>
	static RCPtr<T> Create(const char* _Tag = NULL, bool _bActive = false) {
		T* pInstance = new T;
		pInstance->SetTag(_Tag);
		if(FAILED(pInstance->Init()))
		{
			char errMessage[256] = {};
			strcpy_s(errMessage, 256, T::GetRTTIInst()->GetClassName());
			strcat_s(errMessage, " Created Failed");
			MessageBoxA(NULL, errMessage, "Error", MB_OK);
			delete pInstance;
			return NULL;
		}
		if(_bActive) pInstance->SetActive(true);

		return pInstance;
	}
};

#define CREATE(className)	\
	friend CBase;			\
	static RCPtr<className> Create(const char* _Tag = NULL, bool _bActive = false) { return CBase::Create<className>(_Tag,_bActive); }

TYPEDEF_RCP(CBase, BASE);

#endif //BASE_H__