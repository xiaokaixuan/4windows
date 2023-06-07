// ThreadPool.h: interface for the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREADPOOL_H__E24F5439_D623_4511_A874_40CC9417DDEE__INCLUDED_)
#define AFX_THREADPOOL_H__E24F5439_D623_4511_A874_40CC9417DDEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786)

#include <queue>
#include <vector>
#include <utility>

class CThread;
class CThreadPool  
{
public:
	CThreadPool(size_t nMaxCount);
	virtual ~CThreadPool();

public:
	BOOL IsBusy();
	BOOL Wait(DWORD dwMilliseconds);
	void Run(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);

private:
	void __Proc();
	static DWORD WINAPI __ThreadProc(LPVOID lpParameter);

protected:
	HANDLE m_hThread;
	HANDLE m_hEventExit;

	CRITICAL_SECTION m_csFunc;

	std::vector<CThread*> m_vThreads;
	std::queue< std::pair<LPTHREAD_START_ROUTINE, LPVOID> > m_qFuncs;
};

#endif // !defined(AFX_THREADPOOL_H__E24F5439_D623_4511_A874_40CC9417DDEE__INCLUDED_)
