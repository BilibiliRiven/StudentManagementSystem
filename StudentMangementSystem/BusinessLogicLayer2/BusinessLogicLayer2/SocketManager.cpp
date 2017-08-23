#include "stdafx.h"
#include "SocketManager.h"
#include "TheadPool.h"
#include "RecvTask.h"

CSocketManager g_ClientSocketManager;

CSocketManager g_ServerSocketManager;

unsigned short g_ServerPort = 12345;

DWORD g_ServerIP = 0;

DWORD g_ListenIP = 0;

unsigned short g_ListenPort = 10800;

char g_szServerIP[20] = "127.0.0.1";

BOOL MyWSAStartup()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		return FALSE;
	}

	/* Confirm that the WinSock DLL supports 2.2.*/
	/* Note that if the DLL supports versions greater    */
	/* than 2.2 in addition to 2.2, it will still return */
	/* 2.2 in wVersion since that is the version we      */
	/* requested.                                        */

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2) {
		/* Tell the user that we could not find a usable */
		/* WinSock DLL.                                  */
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

BOOL MyWSAEndup(){
	WSACleanup();
	return TRUE;
}

BOOL ConnectServer(){
	SOCKET s = INVALID_SOCKET;
	s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET){
		closesocket(s);
		OutputDebugString(TEXT("中间层与服务器成功建立失败，3秒钟后再次尝试\r\n"));
		Sleep(3000);
		return FALSE;
	}
	sockaddr_in addr;
	ZeroMemory(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(g_ServerPort);
	addr.sin_addr.S_un.S_addr = inet_addr(g_szServerIP);

	int nRet = connect(s, (SOCKADDR *)&addr, sizeof(addr));
	if (nRet == SOCKET_ERROR){
		closesocket(s);
		OutputDebugString(TEXT("中间层与服务器成功建立失败，3秒钟后再次尝试\r\n"));
		Sleep(3000);
		return FALSE;
	}

	g_ServerSocketManager.AddSocket(s, &addr);
	OutputDebugString(TEXT("中间层与服务器成功建立连接\r\n"));
	return TRUE;
}

LRESULT ListenClientProc(LPVOID lParam){

	SOCKET listen_socket = INVALID_SOCKET;

	listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == listen_socket){
		OutputDebugString(TEXT("中间层中间层器监听发生错误，无法INVALID_SOCKET == m_sAccept"));
		return 0;
	}

	SOCKADDR_IN listen_addrin = { 0 };


	listen_addrin.sin_family = AF_INET;
	listen_addrin.sin_port = htons(g_ListenPort);
	listen_addrin.sin_addr.S_un.S_addr = inet_addr("0.0.0.0"); // 这里我写不好

	int nRet = bind(listen_socket,
		(sockaddr*)&listen_addrin,
		sizeof(listen_addrin));

	if (nRet == SOCKET_ERROR){
		OutputDebugString(TEXT("中间层socket绑定监听端口发生失败\r\n"));
		return TRUE;
	}

	//3. listen 监听

	nRet = listen(listen_socket, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		OutputDebugString(TEXT("socket监听失败\r\n"));
		return TRUE;
	}
	OutputDebugString(TEXT("socket开始监听监听\r\n"));

	while (TRUE){
		SOCKET AcceptSocket = accept(listen_socket, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET) {
			wprintf(L"accept failed with error: %ld\n", WSAGetLastError());
			return 1;
		}
		else
			wprintf(L"Client connected.\n");


		g_ClientSocketManager.AddSocket(AcceptSocket, &listen_addrin); // 添加
	}
	return TRUE;
}


LRESULT CheckRecvProc(LPVOID lParam){

	CTheadPool *pPool = (CTheadPool *)lParam;

	while (true)
	{
		SOCKET_CONTEXT * pContext = NULL;
		g_ClientSocketManager.GetNextSocketContext(pContext);
		if (pContext && !(pContext->m_iRecvClock)){ // 拿出队列中SOCKET_CONTEXT，检查是否已经添加过接收任务，添加过任务则不再进行检查。
			//OutputDebugString(L"检查客户端socketContext\r\n");
			unsigned long nLen = 0;
			//int iRet = recv(pContext->m_Socket, (char *)&nLen, sizeof(nLen), MSG_PEEK);
			int iRet = ioctlsocket(pContext->m_Socket, FIONREAD, &nLen);
			if (iRet != 0){
				wprintf(L"Client disconnected. socket close unexpected \n");
				OutputDebugString(TEXT("客户端意外断开连接\r\n"));
				g_ClientSocketManager.DeletSocket(pContext->m_Socket);
			}
			else{
				if (nLen > 0){
					pContext->m_iRecvClock = 1;
					CRecvTask * pRecvTask = new CRecvTask(nLen, pContext, pPool);

					OutputDebugString(TEXT("接收客户数据，投递接收消息,开启接收锁\r\n"));
					pPool->Handle(pRecvTask);
				}
			}
		}


		pContext = NULL;
		g_ServerSocketManager.GetNextSocketContext(pContext);
		if (pContext && !(pContext->m_iRecvClock)){ // 拿出队列中SOCKET_CONTEXT，检查是否已经添加过接收任务，添加过任务则不再进行检查。
			//OutputDebugString(L"检查服务端socketContext\r\n");
			unsigned long nLen = 0;
			//int iRet = recv(pContext->m_Socket, (char *)&nLen, sizeof(nLen), MSG_PEEK);
			int iRet = ioctlsocket(pContext->m_Socket, FIONREAD, &nLen);
			if (iRet != 0){
				wprintf(L"Server disconnected. socket close unexpected \n");
				OutputDebugString(TEXT("Server端意外断开连接\r\n"));
				g_ServerSocketManager.DeletSocket(pContext->m_Socket);
				break;
			}
			else{
				if (nLen > 0){
					pContext->m_iRecvClock = 1;
					CRecvTask * pRecvTask = new CRecvTask(nLen, pContext, pPool);
					OutputDebugString(TEXT("Server客户数据，投递接收消息,开启接收锁\r\n"));
					pPool->Handle(pRecvTask);
				}
			}
		}
	}
	return NOERROR;
}

CSocketManager::CSocketManager(){
	m_pos = NULL;
}



CSocketManager::~CSocketManager(){
	// 循环释放掉Socket
	m_cs.Lock();
	POSITION pos = m_SOCKETList.GetHeadPosition();
	POSITION posPre;
	while (NULL != pos){
		posPre = pos;
		SOCKET_CONTEXT * pContext = m_SOCKETList.GetNext(pos);
		closesocket(pContext->m_Socket);
		delete pContext;
		m_SOCKETList.RemoveAt(posPre);
	}
	m_cs.Unlock();
}

LRESULT CSocketManager::AddSocket(SOCKET socket, SOCKADDR_IN * pAddr){
	SOCKET_CONTEXT * pContext = NULL;
	pContext = new SOCKET_CONTEXT;
	pContext->m_Socket = socket;
	pContext->m_Addrin = *pAddr;
	m_cs.Lock();
	m_SOCKETList.AddTail(pContext);
	m_cs.Unlock();
	return NOERROR;
}

LRESULT CSocketManager::DeletSocket(SOCKET socket){
	m_cs.Lock();
	POSITION pos = m_SOCKETList.GetHeadPosition();
	POSITION posPre;
	while (NULL != pos){
		posPre = pos;
		SOCKET_CONTEXT * pContext = m_SOCKETList.GetNext(pos);
		if (pContext->m_Socket == socket){
			closesocket(socket);
			m_SOCKETList.RemoveAt(posPre);
			break;
		}
	}
	m_cs.Unlock();
	return E_UNEXPECTED;
}

LRESULT CSocketManager::GetSocketContext(SOCKET socket, SOCKET_CONTEXT * & rpContext){
	rpContext = NULL;
	m_cs.Lock();
	POSITION pos = m_SOCKETList.GetHeadPosition();
	POSITION posPre;
	while (NULL != pos){
		posPre = pos;
		SOCKET_CONTEXT * pContext = m_SOCKETList.GetNext(pos);
		if (pContext->m_Socket == socket){
			rpContext = pContext;
			break;
		}
	}
	m_cs.Unlock();
	return NOERROR;
}

LRESULT CSocketManager::GetNextSocketContext(SOCKET_CONTEXT * & pSocketContext){
	pSocketContext = NULL;
	m_cs.Lock();
	do 
	{
		if (m_SOCKETList.GetCount() <= 0){
			break;
		}


		if (!m_pos){
			m_pos = m_SOCKETList.GetHeadPosition();
		}

		if (NULL == m_pos){
			break;
		}

		pSocketContext = m_SOCKETList.GetNext(m_pos);
	} while (0);
	m_cs.Unlock();
	return NOERROR;
}

/*
* 释放所有的连接
*/
LRESULT CSocketManager::ReleaseAllSocketContext(){
	m_cs.Lock();
	POSITION pos = m_SOCKETList.GetHeadPosition();
	POSITION posPre;
	while (NULL != pos){
		posPre = pos;
		SOCKET_CONTEXT * pContext = m_SOCKETList.GetNext(pos);
		closesocket(pContext->m_Socket);
		delete pContext;
		m_SOCKETList.RemoveAt(posPre);
	}
	m_cs.Unlock();
	return NOERROR;
}