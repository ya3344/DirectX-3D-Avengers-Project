#ifndef ENGINE_MACRO_H__
#define ENGINE_MACRO_H__

// JH_ErrorCheck_180520 ErroCehck 구문은 Release모드에서 실행안하도록 설정.
#define _MSG_BOX(MESSAGE) MessageBox(0, TEXT(MESSAGE), TEXT("System Message"), MB_OK);



#define NO_COPY(CLASSNAME)									\
	private:												\
	CLASSNAME(const CLASSNAME&);							\
	CLASSNAME& operator = (const CLASSNAME&);				

#define _DECLARE_SINGLETON(CLASSNAME)						\
	NO_COPY(CLASSNAME)										\
	private:												\
	static CLASSNAME*	m_pInstance;						\
	public:													\
	static CLASSNAME*	GetInstance( void );				\
	static DWORD DestroyInstance( void );					

#define _IMPLEMENT_SINGLETON(CLASSNAME)						\
	CLASSNAME*	CLASSNAME::m_pInstance = NULL;				\
	CLASSNAME*	CLASSNAME::GetInstance( void )	{			\
		if(m_pInstance == NULL) {							\
			m_pInstance = new CLASSNAME;					\
		}													\
		return m_pInstance;									\
	}														\
	DWORD CLASSNAME::DestroyInstance( void ) {				\
		DWORD dwRefCnt = 0;								\
		if(NULL != m_pInstance)	{							\
			dwRefCnt = m_pInstance->Release();				\
			if(0 == dwRefCnt) m_pInstance = NULL;			\
		}													\
		return dwRefCnt;									\
	}


#ifdef ENGINE_EXPORTS
#define _ENGINE_DLL _declspec(dllexport)
#else
#define _ENGINE_DLL _declspec(dllimport)
#endif

#define _BEGIN(NAMESPACE) namespace NAMESPACE {

#define _END }

#define _USING(NAMESPACE) using namespace NAMESPACE;

#endif // ENGINE_MACRO_H__

