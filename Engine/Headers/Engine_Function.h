#ifndef Engine_Function_h__
#define Engine_Function_h__

namespace Engine {

	template<typename T>
	void Safe_Delete(T& p)
	{
		if(p) {
			delete p;
			p = NULL;
		}
	}

	template<typename T>
	void Safe_Delete_Array(T& p)
	{
		if(p) {
			delete[] p;
			p = NULL;
		}
	}

	template<typename T>
	unsigned long Safe_Release(T& p)
	{
		unsigned long dwRefCnt = 0;
		if(p) {
			dwRefCnt = p->Release();
			if(dwRefCnt == 0) p = NULL;
		}
		return dwRefCnt;
	}

	extern inline void IndexBufferInitializer(WORD* _pIndies, int _iSize, ...)
	{
		va_list ap;
		va_start(ap, _iSize);

		for(int i = 0; i < _iSize; ++i)
			_pIndies[i] = va_arg(ap, int);

		va_end(ap);
	}

}

#endif // Engine_Function_h__
