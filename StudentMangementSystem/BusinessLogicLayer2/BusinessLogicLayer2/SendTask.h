#pragma once
#include "ITask.h"
class CSendTask :
	public ITask
{
	SOCKET m_socket;
	char * m_pBuff;
	int m_iLen;
public:
	CSendTask(SOCKET s, LPVOID pBuff, int iLen);
	virtual ~CSendTask();
	virtual int Execute(LPVOID lParam);
};

