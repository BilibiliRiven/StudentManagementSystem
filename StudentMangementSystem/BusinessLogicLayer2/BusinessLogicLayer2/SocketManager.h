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
	
	// 完成 _IO_CONTEXT 初始化
	_IO_CONTEXT(){
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
		m_OnType = NOTING_TO_DO;
	}

	// 重置缓冲区内容
	void ResetBuffer(){
		ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
	}
}IO_CONTEXT, *PIO_CONTEXT;


typedef struct _SOCKET_CONTEXT{

	SOCKADDR_IN			m_Addrin;
	SOCKET				m_Socket;
	int					m_iRecvClock;			// 当SOCKET_CONTEXT 在接收队列中的时候加锁，防止在投递一次接收
	CArray<IO_CONTEXT *> m_arrayIoContext; // 这个暂时没用到


	// 初始化
	_SOCKET_CONTEXT()
	{
		m_Socket = INVALID_SOCKET;
		m_iRecvClock = 0;
		memset(&m_Addrin, 0, sizeof(m_Addrin));
	}

	// 释放资源
	~_SOCKET_CONTEXT()
	{
		if (m_Socket != INVALID_SOCKET)
		{
			closesocket(m_Socket);
			m_Socket = INVALID_SOCKET;
		}
		// 释放掉所有的IO上下文数据
		for (int i = 0; i < m_arrayIoContext.GetCount(); i++)
		{
			delete m_arrayIoContext.GetAt(i); // 这里不知道什么意思
		}
		m_arrayIoContext.RemoveAll();
	}

	// 获取一个新的IoContext
	IO_CONTEXT* GetNewIoContext()
	{
		IO_CONTEXT* p = new IO_CONTEXT;

		m_arrayIoContext.Add(p);

		return p;
	}

	// 从数组中移除一个指定的IoContext
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
* 建立监听端口，并且循环监听客户端连接。
*/
LRESULT ListenClientProc(LPVOID lParam);

/*
* 线程，循环检车g_，是否有数据包过来，有测产生一个任务，
* 将任务投放到任务对列中
*/
LRESULT CheckRecvProc(LPVOID lParam);

/************************************************************************/
/*	                      网络SocketManager管理类                        */
/*                 任务：将客户端，服务端的Socket，整合到一起方便管理    */
/*					注意：服务端Socket，和客户端Socket分别在不同的对象中 */
/************************************************************************/

class CSocketManager{
	CCriticalSection m_cs;
	CList<SOCKET_CONTEXT *>		m_SOCKETList;
	POSITION m_pos;
public:
	CSocketManager();
	/*
	* 循环释放所有添加进来的SocketContext，
	* 并且会优雅的关闭Socket
	*/
	~CSocketManager();

	/*
	* 将连接进来的Socket添加到队列中去
	*/
	LRESULT AddSocket(SOCKET socket, SOCKADDR_IN * pAddr );	// 添加SOCKET

	/*
	* 从List中删除SocketContext
	* 删除成功返回NOERRO
	* 删除失败返回E_UNEXPECTED
	*/
	LRESULT DeletSocket(SOCKET socket);

	/*
	* 函数根据Socket检索出对应，SocketContext，通过参数返回
	* 如果找不到则返回的pSocketContext指针为NULL
	* 如果m_SOCKETList没有元素，返回E_UNEXPECTED
	* 如果正常执行返回NERROR
	*/
	LRESULT GetSocketContext(SOCKET socket, SOCKET_CONTEXT * &);
	/*
	* 函数用于循环遍历SocketContext
	* 如果m_SOCKETList没有元素，返回E_UNEXPECTED
	* 如果正常执行返回NERROR
	*/
	LRESULT GetNextSocketContext(SOCKET_CONTEXT * &);

	/*
	* 释放所有的连接
	*/
	LRESULT ReleaseAllSocketContext();
};

/************************************************************************/
/*	                      程序启动时候需要的全局变量                     */
/************************************************************************/


extern CSocketManager g_ClientSocketManager;

extern CSocketManager g_ServerSocketManager;

extern unsigned short g_ServerPort;

extern DWORD g_ServerIP;

extern DWORD g_ListenIP;

extern unsigned short g_ListenPort;

extern char g_szServerIP[20];