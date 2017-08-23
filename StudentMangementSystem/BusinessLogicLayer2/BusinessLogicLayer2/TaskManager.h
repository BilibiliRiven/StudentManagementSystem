#pragma once
#include "ITask.h"

class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();
private:
	CCriticalSection m_cs;
	CList<ITask*, ITask*> m_Lst;
public:
	HRESULT AddTask(ITask * pTask);
	HRESULT RemoveTask(ITask* pTask);
	HRESULT GetTask(ITask *& rpTask);
};

