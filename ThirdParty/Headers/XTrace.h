#ifndef XTRACE_H__
#define XTRACE_H__

#define XTRACE_BUF_SIZE		256 
#define XTRACE				_DbgPrintf 

inline void __cdecl _DbgPrintf(const char *str, ...)
{
	char    buff[XTRACE_BUF_SIZE];
	char    rbuff[XTRACE_BUF_SIZE];
	DWORD   dwError;
	va_list    ap;

	dwError = GetLastError();

	va_start(ap, str);
	if (_vsnprintf_s(buff, sizeof buff, str, ap) < 0)
		buff[sizeof buff - 1] = 0;
	va_end(ap);

	if (_snprintf_s(rbuff, sizeof rbuff, "%s\n", buff) < 0)
		rbuff[sizeof rbuff - 1] = 0;

	OutputDebugStringA(rbuff);
	SetLastError(dwError);
}

#endif //XTRACE_H__