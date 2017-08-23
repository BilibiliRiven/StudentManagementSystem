#include "stdafx.h"
#include "TaskManager.h"


CTaskManager::CTaskManager()
{
}


CTaskManager::~CTaskManager()
{
}


HRESULT CTaskManager::AddTask(ITask * pTask)
{
	m_cs.Lock();
	m_Lst.AddTail(pTask);
	m_cs.Unlock();

	return NOERROR;
}


HRESULT CTaskManager::RemoveTask(ITask* pAimTask)
{
	if (pAimTask == NULL){
		return E_INVALIDARG;
	}
	POSITION pos = m_Lst.GetHeadPosition();
	POSITION posLast = pos;
	while (NULL != pos ){
		posLast = pos;
		ITask * pTask = m_Lst.GetNext(pos);
		if (pTask == pAimTask){
			m_Lst.RemoveAt(posLast);
			break;
		}
	}
	return NOERROR;
}


HRESULT CTaskManager::GetTask(ITask *& rpTask)
{
	rpTask = NULL;
	if (m_Lst.GetCount() <= 0)
	{
		rpTask = NULL;
		return E_INVALIDARG;
	}

	m_cs.Lock();

	rpTask = m_Lst.RemoveHead();

	m_cs.Unlock();

	return rpTask>0 ? 1 : 0;
}
