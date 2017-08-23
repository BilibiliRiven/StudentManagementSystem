#pragma once
#include "TaskManager.h"

class CTheadPool
{
public:
	CTheadPool();
	~CTheadPool();
	static DWORD ThreadFunc(LPVOID lpParam);
	BOOL Handle(ITask* pTask);
	BOOL Destroy();
	BOOL Create(int nCount);
private:
  int m_nCount;
  HANDLE m_hSemaphore;
  HANDLE* m_hThreadAry;
  HANDLE m_hExitEvent;
  CTaskManager m_mgr;
  HRESULT IniteDateMenber();
};

