#ifndef Macro_h__
#define Macro_h__

#define SAFE_DELETE(p)			if(p){delete p;p=NULL;}
#define SAFE_DELETE_ARRAY(p)	if(p){delete[] p;p=NULL;}
#define SAFE_RELEASE(p)			if(p){p->Release();p=NULL;}

#define ERR_MSG(Message)		MessageBox(NULL, TEXT(Message), TEXT("Error"), MB_OK)

#endif // Macro_h__
