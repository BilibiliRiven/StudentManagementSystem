#include "stdafx.h"
#include "TheadPool.h"


CTheadPool::CTheadPool()
{
	m_nCount = 0;
	m_hSemaphore = NULL;
	m_hThreadAry = NULL;
}


CTheadPool::~CTheadPool()
{
	this->Destroy();
	if (m_hExitEvent)
		CloseHandle(m_hExitEvent);
	if (m_hSemaphore)
		CloseHandle(m_hSemaphore);
}


HRESULT CTheadPool::IniteDateMenber()
{

	m_hExitEvent = CreateEvent(NULL,
		TRUE,
		FALSE,
		NULL);

	if (m_hExitEvent == NULL)
	{

		return FALSE;
	}

	m_hSemaphore = CreateSemaphore(NULL,
		0,
		0x7fffffff,
		NULL);

	if (m_hSemaphore == NULL)
	{
		return FALSE;
	}

	return NOERROR;
}

BOOL CTheadPool::Destroy()
{

	//֪ͨ�̳߳��е��߳��˳�
	SetEvent(m_hExitEvent);

	//�ȴ��̳߳������е��߳��˳�
	DWORD dwRet = WaitForMultipleObjects(m_nCount,
		m_hThreadAry,
		TRUE,
		INFINITE);

	if (dwRet != WAIT_FAILED)
	{
		for (int i = 0; i < m_nCount; i++)
		{
			CloseHandle(m_hThreadAry[i]);
		}
		delete[] m_hThreadAry;
	}
	m_hThreadAry = NULL;
	CloseHandle(m_hExitEvent);
	m_hExitEvent = NULL;
	CloseHandle(m_hSemaphore);
	m_hSemaphore = NULL;
	return TRUE;
}


BOOL CTheadPool::Create(int nCount)
{
	//�����źŵƶ���
	IniteDateMenber();
	
	m_hThreadAry = new HANDLE[nCount];
	if (m_hThreadAry == NULL)
	{
		return FALSE;
	}

	for (int i = 0; i < nCount; i++)
	{
		m_hThreadAry[i] = (HANDLE)_beginthreadex(NULL,
			0,
			(unsigned int(__stdcall *)(void *))ThreadFunc,
			this,
			0,
			NULL);
		if (m_hThreadAry[i]){
			OutputDebugString(TEXT("�����̴߳����ɹ�\r\n"));
			m_nCount++;
		}
		else{
			break;
		}
	}

	return TRUE;
}

BOOL CTheadPool::Handle(ITask * pTask){
	if (pTask == NULL){
		return FALSE;
	}
	m_mgr.AddTask(pTask);

	//�ͷ�һ���źŵ���Դ,��ʼ�ɻ�
	ReleaseSemaphore(m_hSemaphore,
		1,
		NULL);
	return TRUE;
}


DWORD CTheadPool::ThreadFunc(LPVOID lpParam){
	CTheadPool* pThis = (CTheadPool*)lpParam;
	HANDLE hAry[2];

	hAry[0] = pThis->m_hExitEvent;
	hAry[1] = pThis->m_hSemaphore;

	while (true)
	{

		DWORD dwRet = WaitForMultipleObjects(2,
			hAry,
			FALSE,
			INFINITE);
		if (dwRet == WAIT_FAILED)
		{
			//������
		}
		else if (dwRet == WAIT_TIMEOUT)
		{
			//��ʱ����
		}
		else
		{
			int nIndex = dwRet - WAIT_OBJECT_0;
			switch (nIndex)
			{
			case 0:
			{
				//��ζ�Ÿ��˳��߳���
				return 0;
			}
			break;
			case 1:
			{
				//������,ȡ������,����ʼִ��
				//pCmd->Exec();
				ITask* pTask = NULL;
				BOOL bRet = pThis->m_mgr.GetTask(pTask);
				if (bRet && pTask != NULL)
				{
					pTask->Execute(pThis);
					if (pTask != NULL)
					{
						delete pTask;
					}
				}
				continue;
			}
			break;
			default:
			{

			}
			break;
			}
		}

	}

	return 0;
}