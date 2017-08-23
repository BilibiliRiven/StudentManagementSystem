// BusinessLogicLayer2.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "BusinessLogicLayer2.h"
#include "SocketManager.h"
#include "TheadPool.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ψһ��Ӧ�ó������
void myMain();
CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// ��ʼ�� MFC ����ʧ��ʱ��ʾ����
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO:  ���Ĵ�������Է���������Ҫ
			_tprintf(_T("����:  MFC ��ʼ��ʧ��\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO:  �ڴ˴�ΪӦ�ó������Ϊ��д���롣
			myMain();
		}
	}
	else
	{
		// TODO:  ���Ĵ�������Է���������Ҫ
		_tprintf(_T("����:  GetModuleHandle ʧ��\n"));
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
		// ���ӷ�����
		do{
			if (!ConnectServer()){
				break;
			}

			// ��ʼ���̳߳�
			CTheadPool pool;
			pool.Create(4);

			// �����ͻ��˼����߳�
			hListenThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ListenClientProc, NULL, 0, &dwListenThreadID);
			if (hListenThread == NULL){
				GetLastError();
				break;
			}

			// �����ͻ��˽��ռ���߳�
			hCheckRecvThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckRecvProc, &pool, 0, &dwCheckRecvThreadID);

			if (hCheckRecvThread == NULL){
				GetLastError();
				break;
			}

			WaitForSingleObject(hCheckRecvThread, INFINITE);

		} while (0);

		// �������ӶϿ����ͷ�������Դ���¿�ʼ
		OutputDebugString(TEXT("�������ӶϿ����ͷ�������Դ���¿�ʼ\r\n"));
		CloseHandle(hListenThread);
		CloseHandle(hCheckRecvThread);
		g_ClientSocketManager.ReleaseAllSocketContext();
		g_ServerSocketManager.ReleaseAllSocketContext();
		hListenThread = NULL;
		hCheckRecvThread = NULL;
	}


	MyWSAEndup();
}