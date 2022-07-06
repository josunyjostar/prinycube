#ifndef Function_h__
#define Function_h__

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

#endif // Function_h__
