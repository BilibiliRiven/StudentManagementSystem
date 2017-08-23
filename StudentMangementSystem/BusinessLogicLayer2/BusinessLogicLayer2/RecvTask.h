#pragma once
#include "Buffer.h"
#include "SocketManager.h"
#include "TheadPool.h"
typedef struct _NetPackge{
	int m_iLen;
	char m_pData[1];
}NET_PACKGE, *PNET_PACKGE;

class CRecvTask :
	public ITask
{
	PSOCKET_CONTEXT m_pSocketContext;
	char * m_pBuff;
	CBuffer m_Buff;
	int m_Len;
	CTheadPool * m_pPool;
	enum MyEnum
	{
		ADD,
		DEL,
		SCH,
		CHG,
		BACK
	};
public:
	CRecvTask(int iLen, PSOCKET_CONTEXT pContext, LPVOID pVoid);
	virtual ~CRecvTask();
	virtual int Execute(LPVOID lParam);
	int SetLen(int iLen){ m_Len = iLen; return 0; }
	int SetSocket(SOCKET_CONTEXT * pContext){ m_pSocketContext = pContext; return 0; }
	int ParserJson(std::wstring & rwstr, boost::property_tree::wptree &);
	int ProAddJson(boost::property_tree::wptree & pt);
	int ProDelJson(boost::property_tree::wptree & pt);
	int ProSchJson(boost::property_tree::wptree & pt);
	int ProChgJson(boost::property_tree::wptree & pt);
	int ProBackJson(boost::property_tree::wptree & pt, BYTE * pBuff);
	int GetStuInfoFromTree(boost::property_tree::wptree & pt, std::wstring & rwstrId, std::wstring & rwstrName, std::wstring & rwstrTel, int & iSex);
	int SendSql(CString & rcstrSql, int iType);
};

