#include "stdafx.h"
#include <TlHelp32.h>
#include "extended.h"

// 提取权限
BOOL ElevatePrivileges()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	tkp.PrivilegeCount = 1;
	if(!OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))
		return FALSE;
	LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&tkp.Privileges[0].Luid);
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		return FALSE;
	}
	
	return TRUE;
}

// 拷贝句柄
HANDLE DuplicateHandleEx(DWORD pid, HANDLE h, DWORD flags)
{
	HANDLE hHandle = NULL;
	
	HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if(hProc)
	{
		if(!DuplicateHandle(hProc,
			(HANDLE)h, GetCurrentProcess(),
			&hHandle, 0, FALSE, /*DUPLICATE_SAME_ACCESS*/flags))
		{
			hHandle = NULL;
		}
	}
	
	CloseHandle(hProc);
	return hHandle;
}

// 获取进程ID
int GetProcIds(LPCTSTR pszName, DWORD* Pids)
{
	PROCESSENTRY32 pe32 = {sizeof(pe32)};
	int num = 0;
	
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hSnap)
	{
		if(Process32First(hSnap, &pe32))
		{
			do {
				if(!lstrcmpi(pszName, pe32.szExeFile))
				{
					if(Pids)
					{
						Pids[num++] = pe32.th32ProcessID;
					}
				}
			} while(Process32Next(hSnap, &pe32));
		}
		CloseHandle(hSnap);
	}
	
	return num;
}

// 是否目标进程
BOOL IsTargetPid(DWORD Pid, DWORD* Pids, int num)
{
	for(int i=0; i<num; i++)
	{
		if(Pid == Pids[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}

// 破解进程互斥
int PatchWeChat()
{
	DWORD dwSize = 0;
	POBJECT_NAME_INFORMATION pNameInfo;
	POBJECT_NAME_INFORMATION pNameType;
	PVOID pbuffer = NULL;
	NTSTATUS Status;
	int nIndex = 0;
	DWORD dwFlags = 0;
	char szType[128] = {0};
	char szName[512] = {0};
	PSYSTEM_HANDLE_INFORMATION1 pHandleInfo = NULL;
	int nReturn = -1;

	ElevatePrivileges();

	DWORD Pids[100] = {0};

	DWORD Num = GetProcIds(L"WeChat.exe", Pids);
	if(Num == 0)
	{
		return nReturn;
	}
		
	if(!ZwQuerySystemInformation)
	{
		goto Exit;
	}

	pbuffer = VirtualAlloc(NULL, 0x1000, MEM_COMMIT, PAGE_READWRITE);

	if(!pbuffer)
	{
		goto Exit;
	}

	Status = ZwQuerySystemInformation(SystemHandleInformation, pbuffer, 0x1000, &dwSize);

	if(!NT_SUCCESS(Status))
	{
		if (STATUS_INFO_LENGTH_MISMATCH != Status)
		{
			goto Exit;
		}
		else
		{
			if (NULL != pbuffer)
			{
				VirtualFree(pbuffer, 0, MEM_RELEASE);
			}

			if (dwSize*2 > 0x4000000)  // MAXSIZE
			{
				goto Exit;
			}

			pbuffer = VirtualAlloc(NULL, dwSize*2, MEM_COMMIT, PAGE_READWRITE);

			if(!pbuffer)
			{
				goto Exit;
			}

			Status = ZwQuerySystemInformation(SystemHandleInformation, pbuffer, dwSize*2, NULL);

			if(!NT_SUCCESS(Status))
			{
				goto Exit;    
			}
		}
	}

	pHandleInfo = (PSYSTEM_HANDLE_INFORMATION1)pbuffer;

	for(nIndex = 0; nIndex < (int)pHandleInfo->NumberOfHandles; nIndex++)
	{
		if(IsTargetPid(pHandleInfo->Handles[nIndex].UniqueProcessId, Pids, Num))
		{
			HANDLE hHandle = DuplicateHandleEx(pHandleInfo->Handles[nIndex].UniqueProcessId, 
										(HANDLE)pHandleInfo->Handles[nIndex].HandleValue,
										DUPLICATE_SAME_ACCESS
										);
			if(hHandle == NULL) continue;
	
			Status = NtQueryObject(hHandle, ObjectNameInformation, szName, 512, &dwFlags);

			if (!NT_SUCCESS(Status))
			{
				CloseHandle(hHandle);
				continue;
			}

			Status = NtQueryObject(hHandle, ObjectTypeInformation, szType, 128, &dwFlags);

			if (!NT_SUCCESS(Status))
			{
				CloseHandle(hHandle);
				continue;
			}

			pNameInfo = (POBJECT_NAME_INFORMATION)szName;
			pNameType = (POBJECT_NAME_INFORMATION)szType;

			WCHAR TypName[1024] = {0};
			WCHAR Name[1024] = {0};

			wcsncpy(TypName, (WCHAR*)pNameType->Name.Buffer, pNameType->Name.Length/2);
			wcsncpy(Name, (WCHAR*)pNameInfo->Name.Buffer, pNameInfo->Name.Length/2);

			if (0 == wcscmp(TypName, L"Mutant"))
			{
				if (wcsstr(Name, L"_WeChat_App_Instance_Identity_Mutex_Name"))
				{
					CloseHandle(hHandle);

					hHandle = DuplicateHandleEx(pHandleInfo->Handles[nIndex].UniqueProcessId, 
						(HANDLE)pHandleInfo->Handles[nIndex].HandleValue,
						DUPLICATE_CLOSE_SOURCE
						);

					if(hHandle)
					{
						nReturn = 0;
						CloseHandle(hHandle);
					}
					// goto Exit;
				}
			}

			CloseHandle(hHandle);
		}
		
	}

Exit:
	if (NULL != pbuffer)
	{
		VirtualFree(pbuffer, 0, MEM_RELEASE);
	}

	return nReturn;
}

