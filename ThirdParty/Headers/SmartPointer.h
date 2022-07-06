#ifndef SmartPointer_h__
#define SmartPointer_h__

#define TYPEDEF_RCP(CLASS_NAME, DEFINE_NAME)				\
typedef RCPtrToConst<CLASS_NAME>		CRCP_##DEFINE_NAME;	\
typedef RCPtr<CLASS_NAME>				RCP_##DEFINE_NAME

#define RCP_CAST(CAST_TYPE, RCP_OBJ)	((CAST_TYPE)(void*)(RCP_OBJ))

//RCObject
///////////////////////////////////////////////////////////////////////////////////////

class RCObject
{
protected:
	explicit RCObject():m_RefCnt(0) {}
	explicit RCObject(const RCObject& rhs):m_RefCnt(0) {}
	virtual ~RCObject() {}

	RCObject& operator=(const RCObject& rhs)	{return *this;}

public:
	template<typename> friend class RCPtrToConst;

	unsigned long	AddRef()					{return ++m_RefCnt;}
	unsigned long	Release()					{if(!--m_RefCnt) {delete this; return 0;} return m_RefCnt;}

private:
	unsigned long	m_RefCnt;
};

//RCPtrToConst
///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class RCPtrToConst {
public:
	RCPtrToConst(void* realPtr = NULL)		:m_constPointee((T*)realPtr)		{ this->Init(); }
	RCPtrToConst(const RCPtrToConst& rhs)	:m_constPointee(rhs.m_constPointee)	{ this->Init(); }
	virtual ~RCPtrToConst()					{ if(m_pPointee) m_pPointee->Release(); }

public:
	unsigned long	AddRef();
	unsigned long	Release();

public:
	template<typename newType>
	operator RCPtrToConst<newType>()		{ return RCPtrToConst<newType>(m_constPointee); }
	operator void*() const					{ return m_constPointee; }
	const T* operator->() const				{ return m_constPointee; }
	const T& operator*() const				{ return *m_constPointee; }

	RCPtrToConst& operator=(const RCPtrToConst& rhs);
	bool operator==(const RCPtrToConst& rhs) const
	{ return (m_pPointee==rhs.m_pPointee); }

protected:
	union {
		const T*	m_constPointee;
		T*			m_pPointee;
	};

protected:
	void Init();
};

//RCPtr
///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
class RCPtr : public RCPtrToConst<T>{
public:
	RCPtr(void* realPtr = NULL)	:RCPtrToConst<T>(realPtr)	{}
	RCPtr(const RCPtr& rhs)		:RCPtrToConst<T>(rhs)		{}
	~RCPtr() {}

public:
	template<typename newType>
	operator RCPtr<newType>()	{ return RCPtr<newType>(m_pPointee); }
	operator void*() const		{ return m_pPointee; }
	T* operator->() const		{ return m_pPointee; }
	T& operator*() const		{ return *m_pPointee; }
};

//RCPtrToConst
///////////////////////////////////////////////////////////////////////////////////////

template<typename T>
unsigned long RCPtrToConst<T>::AddRef()
{
	if(!m_pPointee) return 0;
	return m_pPointee->AddRef();
}

template<typename T>
unsigned long RCPtrToConst<T>::Release()
{
	if(!m_pPointee) return 0;
	DWORD dwRefCnt = m_pPointee->Release();
	if(dwRefCnt == 0) m_pPointee = NULL;
	return dwRefCnt;
}

template<typename T>
RCPtrToConst<T>& RCPtrToConst<T>::operator=(const RCPtrToConst& rhs)
{
	if(m_pPointee != rhs.m_pPointee) {
		T* oldPointee = m_pPointee;
		m_pPointee = rhs.m_pPointee;
		this->Init();
		if(oldPointee) oldPointee->Release();
	}
	return *this;
}


template<typename T>
void RCPtrToConst<T>::Init()
{
	if(!m_pPointee) return;
	m_pPointee->AddRef();
}

#endif // SmartPointer_h__
