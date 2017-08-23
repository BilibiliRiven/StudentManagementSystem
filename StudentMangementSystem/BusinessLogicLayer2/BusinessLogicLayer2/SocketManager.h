#pragma once
#include <WinSock2.h>
#include "ITask.h"
#include <afxtempl.h>

#define MAX_BUFFER_LEN        8192  


typedef enum _OPERATION_TYPE
{
	RECV_CLIENT,
	SEND_CLIENT,
	RECV_SERVER,
	SEND_SERVER,
	NOTING_TO_DO
}OPERATION_TYPE;


typedef struct _IO_CONTEXT{

	SOCKET				m_sockAccept;
	OPERATION_TYPE		m_OnType;
	char				m_szBuffer[MAX_BUFFER_LEN];
	
	// ��� _IO_CONTEXT ��ʼ��
	_IO_CONTEXT(){
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
		m_OnType = NOTING_TO_DO;
	}

	// ���û���������
	void ResetBuffer(){
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
	}
}IO_CONTEXT, *PIO_CONTEXT;


typedef struct _SOCKET_CONTEXT{

	SOCKADDR_IN			m_Addrin;
	SOCKET				m_Socket;
	int					m_iRecvClock;			// ��SOCKET_CONTEXT �ڽ��ն����е�ʱ���������ֹ��Ͷ��һ�ν���
	CArray<IO_CONTEXT *> m_arrayIoContext; // �����ʱû�õ�


	// ��ʼ��
	_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		m_iRecvClock = 0;
		memset(&m_Addrin, 0, sizeof(m_Addrin));
	}

	// �ͷ���Դ
	~_SOCKET_CONTEXT()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
		// �ͷŵ����е�IO����������
		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			delete m_arrayIoContext.GetAt(i); // ���ﲻ֪��ʲô��˼
		}
		m_arrayIoContext.RemoveAll();
	}

	// ��ȡһ���µ�IoContext
	IO_CONTEXT* GetNewIoContext()
	{
		IO_CONTEXT* p = new IO_CONTEXT;

		m_arrayIoContext.Add(p);

		return p;
	}

	// ���������Ƴ�һ��ָ����IoContext
	void RemoveContext(IO_CONTEXT* pContext)
	{
		ASSERT(pContext != NULL);

		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			if (pContext == m_arrayIoContext.GetAt(i))
			{
				delete pContext;
				pContext = NULL;
				m_arrayIoContext.RemoveAt(i);
				break;
			}
		}
	}

}SOCKET_CONTEXT, *PSOCKET_CONTEXT;


BOOL MyWSAStartup();

BOOL MyWSAEndup();

BOOL ConnectServer();

/*
* ���������˿ڣ�����ѭ�������ͻ������ӡ�
*/
LRESULT ListenClientProc(LPVOID lParam);

/*
* �̣߳�ѭ���쳵g_���Ƿ������ݰ��������в����һ������
* ������Ͷ�ŵ����������
*/
LRESULT CheckRecvProc(LPVOID lParam);

/************************************************************************/
/*	                      ����SocketManager������                        */
/*                 ���񣺽��ͻ��ˣ�����˵�Socket�����ϵ�һ�𷽱����    */
/*					ע�⣺�����Socket���Ϳͻ���Socket�ֱ��ڲ�ͬ�Ķ����� */
/************************************************************************/

class CSocketManager{
	CCriticalSection m_cs;
	CList<SOCKET_CONTEXT *>		m_SOCKETList;
	POSITION m_pos;
public:
	CSocketManager();
	/*
	* ѭ���ͷ�������ӽ�����SocketContext��
	* ���һ����ŵĹر�Socket
	*/
	~CSocketManager();

	/*
	* �����ӽ�����Socket��ӵ�������ȥ
	*/
	LRESULT AddSocket(SOCKET socket, SOCKADDR_IN * pAddr );	// ���SOCKET

	/*
	* ��List��ɾ��SocketContext
	* ɾ���ɹ�����NOERRO
	* ɾ��ʧ�ܷ���E_UNEXPECTED
	*/
	LRESULT DeletSocket(SOCKET socket);

	/*
	* ��������Socket��������Ӧ��SocketContext��ͨ����������
	* ����Ҳ����򷵻ص�pSocketContextָ��ΪNULL
	* ���m_SOCKETListû��Ԫ�أ�����E_UNEXPECTED
	* �������ִ�з���NERROR
	*/
	LRESULT GetSocketContext(SOCKET socket, SOCKET_CONTEXT * &);
	/*
	* ��������ѭ������SocketContext
	* ���m_SOCKETListû��Ԫ�أ�����E_UNEXPECTED
	* �������ִ�з���NERROR
	*/
	LRESULT GetNextSocketContext(SOCKET_CONTEXT * &);

	/*
	* �ͷ����е�����
	*/
	LRESULT ReleaseAllSocketContext();
};

/************************************************************************/
/*	                      ��������ʱ����Ҫ��ȫ�ֱ���                     */
/************************************************************************/


extern CSocketManager g_ClientSocketManager;

extern CSocketManager g_ServerSocketManager;

extern unsigned short g_ServerPort;

extern DWORD g_ServerIP;

extern DWORD g_ListenIP;

extern unsigned short g_ListenPort;

extern char g_szServerIP[20];