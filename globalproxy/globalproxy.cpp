// globalproxy.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <tchar.h>
#include <windows.h>
#include "wininet.h"
#include <iostream>

#pragma comment(lib, "Wininet")

BOOL SetConnectionOptions(LPTSTR conn_name, LPTSTR proxy_full_addr);
BOOL DisableConnectionProxy(LPTSTR conn_name);

int main(int argc, TCHAR* argv[])
{
	if (argc < 2) {
		std::cout << "globalproxy Usage:" << std::endl << std::endl;
		std::cout << "Examples:" << std::endl;
		std::cout << "globalproxy.exe 127.0.0.1:1080 -- set proxy to 127.0.0.1:1080." << std::endl;
		std::cout << "globalproxy.exe \"\" -- disable proxy, set proxy to \"\"." << std::endl;
		std::cout << std::endl;
		std::cout << "Version 1.0.0" << std::endl;
		std::cout << "Author: Kaixuan <xiaokaixuan@gmail.com>." << std::endl;
		return 0;
	}

	TCHAR connection_name[1024] = _T("");
	TCHAR proxy_addr[1024] = { 0 };
	(void)lstrcpyn(proxy_addr, argv[1], _countof(proxy_addr));

	if (lstrlen(proxy_addr) <= 0) {
		if (!DisableConnectionProxy(connection_name))
		{
			std::cerr << _T("Proxy disable") << " failure." << std::endl;
			return 1;
		}
	}
	else if (!SetConnectionOptions(connection_name, proxy_addr))
	{
		std::cerr << _T("Proxy set to ") << proxy_addr << " failure." << std::endl;
		return 1;
	}
	return 0;
}

BOOL SetConnectionOptions(LPTSTR conn_name, LPTSTR proxy_full_addr)
{
	//conn_name: active connection name. 
	//proxy_full_addr : eg "210.78.22.87:8000"
	INTERNET_PER_CONN_OPTION_LIST list{};
	BOOL    bReturn;
	DWORD   dwBufSize = sizeof(list);
	// Fill out list struct.
	list.dwSize = sizeof(list);
	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = conn_name;
	// Set three options.
	list.dwOptionCount = 2;//3;
	list.pOptions = new INTERNET_PER_CONN_OPTION[2/*3*/];
	// Make sure the memory was allocated.
	if (NULL == list.pOptions)
	{
		// Return FALSE if the memory wasn't allocated.
		OutputDebugString(_T("failed to allocat memory in SetConnectionOptions()"));
		return FALSE;
	}
	// Set flags.
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT |
		PROXY_TYPE_PROXY;

	// Set proxy name.
	list.pOptions[1].dwOption = INTERNET_PER_CONN_PROXY_SERVER;
	list.pOptions[1].Value.pszValue = proxy_full_addr;//"http://proxy:80";

	/*
	// Set proxy override.
	list.pOptions[2].dwOption = INTERNET_PER_CONN_PROXY_BYPASS;
	list.pOptions[2].Value.pszValue = "local";
	*/

	// Set the options on the connection.
	bReturn = InternetSetOption(NULL,
		INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);

	// Free the allocated memory.
	delete[] list.pOptions;

	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
	return bReturn;
}

BOOL DisableConnectionProxy(LPTSTR conn_name)
{
	//conn_name: active connection name. 
	INTERNET_PER_CONN_OPTION_LIST list{};
	BOOL    bReturn;
	DWORD   dwBufSize = sizeof(list);
	// Fill out list struct.
	list.dwSize = sizeof(list);
	// NULL == LAN, otherwise connectoid name.
	list.pszConnection = conn_name;
	// Set three options.
	list.dwOptionCount = 1;
	list.pOptions = new INTERNET_PER_CONN_OPTION[list.dwOptionCount];
	// Make sure the memory was allocated.
	if (NULL == list.pOptions)
	{
		// Return FALSE if the memory wasn't allocated.
		OutputDebugString(_T("failed to allocat memory in DisableConnectionProxy()"));
		return FALSE;
	}
	// Set flags.
	list.pOptions[0].dwOption = INTERNET_PER_CONN_FLAGS;
	list.pOptions[0].Value.dwValue = PROXY_TYPE_DIRECT;
	// Set the options on the connection.
	bReturn = InternetSetOption(NULL,
		INTERNET_OPTION_PER_CONNECTION_OPTION, &list, dwBufSize);
	// Free the allocated memory.
	delete[] list.pOptions;
	InternetSetOption(NULL, INTERNET_OPTION_SETTINGS_CHANGED, NULL, 0);
	InternetSetOption(NULL, INTERNET_OPTION_REFRESH, NULL, 0);
	return bReturn;
}

