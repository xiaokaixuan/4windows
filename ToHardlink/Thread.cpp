// Thread.cpp: implementation of the CThread class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Thread.h"


CThread::CThread()
: m_pThreadProc(0)
, m_pParameter(0)
, m_bBusy(0)
, m_hThread(0)
{
	m_hEventWait = CreateEvent(0, TRUE, FALSE, NULL);
	m_hEventExit = CreateEvent(0, TRUE, FALSE, NULL);
	InitializeCriticalSection(&m_csBusy);
	m_hThread = CreateThread(0, 0, __ThreadProc, this, 0, &m_dwThreadId);
}

CThread::~CThread()
{
	Terminate();
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	DeleteCriticalSection(&m_csBusy);
	CloseHandle(m_hEventWait);
	CloseHandle(m_hEventExit);
}

void CThread::Terminate()
{
	SetEvent(m_hEventExit);
}

BOOL CThread::IsBusy()
{
	EnterCriticalSection(&m_csBusy);
	BOOL bRet = m_bBusy;
	LeaveCriticalSection(&m_csBusy);
	return bRet;
}

void CThread::_SetBusy(BOOL bBusy)
{
	EnterCriticalSection(&m_csBusy);
	m_bBusy = bBusy;
	LeaveCriticalSection(&m_csBusy);
}

DWORD CThread::GetThreadId() const
{
	return m_dwThreadId;
}

void CThread::Run(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
	if (!IsBusy() && lpStartAddress)
	{
		_SetBusy(TRUE);
		m_pThreadProc = lpStartAddress;
		m_pParameter = lpParameter;
		SetEvent(m_hEventWait);
	}
}

DWORD WINAPI CThread::__ThreadProc(LPVOID lpParameter)
{
	CThread* pThis = (CThread*)lpParameter;
	pThis->__Proc();

	return 0;
}

void CThread::__Proc()
{
	for (;;)
	{
		if (!m_pThreadProc)
		{
			HANDLE hEvents[2] = {m_hEventExit, m_hEventWait};
			DWORD dwRet = WaitForMultipleObjects(2, hEvents, FALSE, INFINITE);
			if (dwRet - WAIT_OBJECT_0 == 0) break;
		}
		m_pThreadProc(m_pParameter);
		m_pThreadProc = NULL;
		m_pParameter = NULL;
		ResetEvent(m_hEventWait);
		_SetBusy(FALSE);
	}
}
