#include "stdafx.h"
#include "SendTask.h"


CSendTask::CSendTask(SOCKET s, LPVOID pBuff, int iLen)
{
	m_socket = s;
	m_pBuff = new char[iLen];
	m_iLen = iLen;
	if (m_pBuff == NULL){
		MessageBox(NULL, NULL, L"CSendTask()发送任务初始化异常，分配缓冲区失败\r\n", MB_OK);
	}
	memcpy(m_pBuff, pBuff, iLen);
	OutputDebugString(TEXT("CSendTask()发送任务初始化完成"));
}


CSendTask::~CSendTask()
{
	if (m_pBuff){
		delete m_pBuff;
		m_pBuff = NULL;
	}
}



int CSendTask::Execute(LPVOID lParam){
	//----------------------
	// Send an initial buffer
	int iResult = send(m_socket, m_pBuff, m_iLen, 0);
	if (iResult == SOCKET_ERROR) {
		wprintf(L"send failed with error: %d\n", WSAGetLastError());
		OutputDebugString(TEXT(":Execute(LPVOID lParam)发送任务执行失败\r\n"));
		WSACleanup();
		return 1;
	}

	wprintf(L":Execute(LPVOID lParam)发送 %d to %d\r\n", m_socket, m_iLen);
	return 0;
}