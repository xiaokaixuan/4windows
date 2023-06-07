// Thread.h: interface for the CThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_THREAD_H__9BC45342_F764_4104_B614_1CC8A1964171__INCLUDED_)
#define AFX_THREAD_H__9BC45342_F764_4104_B614_1CC8A1964171__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CThread  
{
public:
	CThread();
	virtual ~CThread();

public:
	BOOL  IsBusy();
	void  Terminate();
	DWORD GetThreadId() const;
	void  Run(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter);

protected:
	void _SetBusy(BOOL bBusy);

private:
	void __Proc();
	static DWORD WINAPI __ThreadProc(LPVOID lpParameter);

protected:
	LPTHREAD_START_ROUTINE m_pThreadProc;
	LPVOID m_pParameter;

	BOOL m_bBusy;
	CRITICAL_SECTION m_csBusy;

	HANDLE m_hEventWait;
	HANDLE m_hEventExit;

	HANDLE m_hThread;
	DWORD  m_dwThreadId;
};

#endif // !defined(AFX_THREAD_H__9BC45342_F764_4104_B614_1CC8A1964171__INCLUDED_)
