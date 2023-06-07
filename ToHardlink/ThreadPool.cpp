// ThreadPool.cpp: implementation of the CThreadPool class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Thread.h"
#include <algorithm>
#include "ThreadPool.h"


CThreadPool::CThreadPool(size_t nMaxCount)
{
	for (size_t i(0); i < nMaxCount; ++i)
	{
		m_vThreads.push_back(new CThread);
	}
	InitializeCriticalSection(&m_csFunc);
	m_hEventExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hThread = CreateThread(0, 0, __ThreadProc, this, 0, 0);
}

CThreadPool::~CThreadPool()
{
	SetEvent(m_hEventExit);
	WaitForSingleObject(m_hThread, INFINITE);
	CloseHandle(m_hThread);
	for (std::vector<CThread*>::iterator i(m_vThreads.begin())
		; i != m_vThreads.end(); ++i)
	{
		delete *i;
	}
	m_vThreads.clear();
	DeleteCriticalSection(&m_csFunc);
}

DWORD WINAPI CThreadPool::__ThreadProc(LPVOID lpParameter)
{
	CThreadPool* pThis = (CThreadPool*)lpParameter;
	pThis->__Proc();

	return 0;
}

void CThreadPool::__Proc()
{
	for (;WaitForSingleObject(m_hEventExit, 100) != WAIT_OBJECT_0;)
	{
		EnterCriticalSection(&m_csFunc);
		if (!m_qFuncs.empty())
		{
			for (std::vector<CThread*>::iterator i(m_vThreads.begin())
				; i != m_vThreads.end(); ++i)
			{
				if (!(*i)->IsBusy() && !m_qFuncs.empty())
				{
					std::pair<LPTHREAD_START_ROUTINE, LPVOID>& pFunc = m_qFuncs.front();
					(*i)->Run(pFunc.first, pFunc.second);
					m_qFuncs.pop();
				}
			}
		}
		LeaveCriticalSection(&m_csFunc);
	}
}

void CThreadPool::Run(LPTHREAD_START_ROUTINE lpStartAddress, LPVOID lpParameter)
{
	EnterCriticalSection(&m_csFunc);
	std::pair<LPTHREAD_START_ROUTINE, LPVOID> pFunc(std::make_pair(lpStartAddress, lpParameter));
	m_qFuncs.push(pFunc);
	LeaveCriticalSection(&m_csFunc);
}

BOOL CThreadPool::IsBusy()
{
	BOOL bRet(FALSE);
	EnterCriticalSection(&m_csFunc);
	if (m_qFuncs.empty())
	{
		for (std::vector<CThread*>::iterator i(m_vThreads.begin())
			; i != m_vThreads.end(); ++i)
		{
			if ((*i)->IsBusy())
			{
				bRet = TRUE;
				break;
			}
		}
	}
	else bRet = TRUE;
	LeaveCriticalSection(&m_csFunc);
	return bRet;
}

BOOL CThreadPool::Wait(DWORD dwMilliseconds)
{
	DWORD dwTick = GetTickCount();
	for (;IsBusy();)
	{
		Sleep(1);
		if (GetTickCount() - dwTick > dwMilliseconds)
		{
			break;
		}
	}
	return !IsBusy();
}
