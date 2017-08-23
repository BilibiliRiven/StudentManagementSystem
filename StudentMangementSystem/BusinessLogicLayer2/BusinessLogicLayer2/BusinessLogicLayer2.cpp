// BusinessLogicLayer2.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BusinessLogicLayer2.h"
#include "SocketManager.h"
#include "TheadPool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一的应用程序对象
void myMain();
CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// 初始化 MFC 并在失败时显示错误
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  更改错误代码以符合您的需要
			_tprintf(_T("错误:  MFC 初始化失败\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  在此处为应用程序的行为编写代码。
			myMain();
		}
	}
	else
	{
		// TODO:  更改错误代码以符合您的需要
		_tprintf(_T("错误:  GetModuleHandle 失败\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

void myMain(){
	MyWSAStartup();
	HANDLE	hListenThread = NULL;;
	DWORD	dwListenThreadID = NULL;

	HANDLE	hCheckRecvThread = NULL;;
	DWORD	dwCheckRecvThreadID = NULL;
	while (TRUE)
	{
		// 连接服务器
		do{
			if (!ConnectServer()){
				break;
			}

			// 初始化线程池
			CTheadPool pool;
			pool.Create(4);

			// 开启客户端监听线程
			hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenClientProc, NULL, 0, &dwListenThreadID);
			if (hListenThread == NULL){
				GetLastError();
				break;
			}

			// 开启客户端接收检查线程
			hCheckRecvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckRecvProc, &pool, 0, &dwCheckRecvThreadID);

			if (hCheckRecvThread == NULL){
				GetLastError();
				break;
			}

			WaitForSingleObject(hCheckRecvThread, INFINITE);

		} while (0);

		// 网络连接断开就释放所有资源重新开始
		OutputDebugString(TEXT("网络连接断开就释放所有资源重新开始\r\n"));
		CloseHandle(hListenThread);
		CloseHandle(hCheckRecvThread);
		g_ClientSocketManager.ReleaseAllSocketContext();
		g_ServerSocketManager.ReleaseAllSocketContext();
		hListenThread = NULL;
		hCheckRecvThread = NULL;
	}


	MyWSAEndup();
}