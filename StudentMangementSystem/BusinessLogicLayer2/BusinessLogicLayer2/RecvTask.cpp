#include "stdafx.h"
#include "RecvTask.h"
#include "SendTask.h"

CRecvTask::CRecvTask(int iLen, PSOCKET_CONTEXT pContext, LPVOID pVoid)
{
	m_pBuff = NULL;
	m_pBuff = new char[8192];
	if (m_pBuff == NULL){
		assert(FALSE);
	}
	int iRet = recv(pContext->m_Socket, (char *)&m_Len, sizeof(m_Len), MSG_PEEK);
	m_pSocketContext = pContext;
	m_pSocketContext->m_iRecvClock = 1;
	m_pPool = (CTheadPool*)pVoid;

}


CRecvTask::~CRecvTask()
{
	if (m_pBuff){
		delete[] m_pBuff;
		m_pBuff = NULL;
	}
}

int CRecvTask::Execute(LPVOID lParam){
	int iResult = 0;
	do {

		iResult = recv(m_pSocketContext->m_Socket, m_pBuff, 8192, 0);
		if (iResult > 0){
			printf("Bytes received: %d\n", iResult);
		}
		else if (iResult == 0){
			printf("Connection closed\n");
			return -1;
		}
		else{
			printf("recv failed: %d\n", WSAGetLastError());
			return -1;
		}
		m_Buff.Write((PBYTE)m_pBuff, iResult);
		m_Len = m_Len - iResult;
	} while (m_Len > 0);

	// 这样数据就接收完毕了。
	int iBuffLen = m_Buff.GetBufferLen();
	BYTE * pBytes = new BYTE[iBuffLen];
	m_Buff.Read(pBytes, iBuffLen);

	std::wstring wstr;
	wstr = (WCHAR *)(pBytes+4);


	// 解析收到的字符串
	// 处理以后转发
	boost::property_tree::wptree pt;
	switch (ParserJson(wstr, pt))
	{
	case ADD:
		ProAddJson(pt);
		OutputDebugString(TEXT("处理add包\r\n"));
		break;
	case DEL:
		ProDelJson(pt);
		OutputDebugString(TEXT("处理del包\r\n"));
		break;
	case SCH:
		ProSchJson(pt);
		OutputDebugString(TEXT("处理sch包\r\n"));
		break;
	case CHG:
		ProChgJson(pt);
		OutputDebugString(TEXT("处理chg包\r\n"));
		break;
	case BACK:
		ProBackJson(pt, pBytes);
		OutputDebugString(TEXT("处理chg包\r\n"));
		break;
	default:
		OutputDebugString(TEXT("处理未定义的包\r\n"));
		break;
	}

	if (pBytes){
		delete[] pBytes;
		pBytes = NULL;
	}

	// 完成任务以后
	m_pSocketContext->m_iRecvClock = 0;

	return 0;
}

int CRecvTask::ParserJson(std::wstring & rwstr, boost::property_tree::wptree & root)
{
	std::wstringstream is;
	is << rwstr;

	boost::property_tree::read_json(is, root);

	std::wstring wstr = root.get<std::wstring>(L"sql_type");
	if (wstr == L"add"){
		return ADD;
	}
	if (wstr == L"del"){
		return DEL;
	}
	if (wstr == L"sch"){
		return SCH;
	}
	if (wstr == L"chg"){
		return CHG;
	}
	if (wstr == L"back"){
		return BACK;
	}
	return -1;
}


int CRecvTask::ProAddJson(boost::property_tree::wptree & pt)
{
	CString cstrSql;
	boost::property_tree::wptree pt_stu;

	pt_stu = pt.get_child(L"student_info");
	if (pt_stu.empty()){
		return -1;
	}

	std::wstring	wstrName	= pt_stu.get<std::wstring>(L"stu_name");
	std::wstring	wstrTel		= pt_stu.get<std::wstring>(L"stu_tel");
	int				iSex		= pt_stu.get<int>(L"stu_sex");


	cstrSql.Format(TEXT("INSERT INTO db_StudentManagement.dbo.tb_student_info (stu_name,stu_tel,stu_sex, stu_del, stu_tag)\
		VALUES(\'%s\', \'%s\', %d, 0, GetDate())")
						, wstrName.c_str()
						, wstrTel.c_str()
						, iSex
						);

	SendSql(cstrSql, ADD);

	return 0;
}


int CRecvTask::ProDelJson(boost::property_tree::wptree & pt)
{
	CString cstrSql;
	std::wstring	wstrId;
	std::wstring	wstrName;
	std::wstring	wstrTel;
	int				iSex;

	int iRet = GetStuInfoFromTree(pt, wstrId, wstrName, wstrTel, iSex);
	if (iRet < 0){
		OutputDebugString(TEXT("解析Json出错"));
	}

	cstrSql.Format(L"UPDATE db_StudentManagement.dbo.tb_student_info \
					SET stu_del=1  WHERE stu_id IN \
					(SELECT stu_id FROM db_StudentManagement.dbo.tb_student_info \
					WHERE stu_id like \'%s\' \
					and stu_name like \'%s%%\' \
					and stu_tel like\'%s%%\' \
					and stu_sex like\'%s\')"
					, wstrId.empty() ? L"%%":wstrId.c_str()
					, wstrName.empty() ? L"" : wstrName.c_str()
					, wstrTel.empty() ? L"" : wstrTel.c_str()
					, iSex>1?L"%%":(iSex==0?L"0":L"1"));

	SendSql(cstrSql, DEL);

	return 0;
}

int CRecvTask::ProSchJson(boost::property_tree::wptree & pt)
{
	// assert(FALSE);
	CString cstrSql;
	std::wstring	wstrId;
	std::wstring	wstrName;
	std::wstring	wstrTel;
	int				iSex;

	int iRet = GetStuInfoFromTree(pt, wstrId, wstrName, wstrTel, iSex);
	if (iRet < 0){
		OutputDebugString(TEXT("解析Json出错"));
	}

	cstrSql.Format(L"SELECT * FROM tb_student_info WHERE stu_id like \'%%%s%%\'\
					and stu_name like \'%%%s%%\'\
					and stu_tel like \'%%%s%%\'\
					and stu_sex like \'%s\'"
					, wstrId.c_str()
					, wstrName.c_str()
					, wstrTel.c_str()
					, iSex>1 ? L"%%" : (iSex == 0 ? L"0" : L"1"));
	//cstrSql.Format(L"SELECT * FROM db_StudentManagement.dbo.tb_student_info WHERE stu_id like '%%'");
	SendSql(cstrSql, SCH);

	return 0;
}

int CRecvTask::ProChgJson(boost::property_tree::wptree & pt)
{
	CString cstrSql;
	std::wstring	wstrId;
	std::wstring	wstrName;
	std::wstring	wstrTel;
	int				iSex;
	int				iCount = 0;
	int iRet = GetStuInfoFromTree(pt, wstrId, wstrName, wstrTel, iSex);
	if (iRet < 0){
		OutputDebugString(TEXT("解析Json出错"));
	}

	cstrSql = L"update db_StudentManagement.dbo.tb_student_info set ";
	if (!wstrName.empty()){
		CString cstrTemp;
		cstrTemp.Format(L"stu_name=\'%s\'", wstrName.c_str());
		cstrSql += cstrTemp;
		iCount++;
	}

	if (!wstrTel.empty()){
		if (iCount > 0){
			cstrSql += ',';
		}
		CString cstrTemp;
		cstrTemp.Format(L"stu_tel=\'%s\'", wstrTel.c_str());
		cstrSql += cstrTemp;
		iCount++;
	}

	if (iSex>=0){
		if (iCount > 0){
			cstrSql += ',';
		}
		CString cstrTemp;
		cstrTemp.Format(L"stu_sex=%d", iSex);
		cstrSql += cstrTemp;
		iCount++;
	}

	if (iCount == 0){
		OutputDebugString(L"没有修改内容，放弃修改");
		return 0;
	}

	CString cstrTemp;
	cstrTemp.Format(L" WHERE stu_id=\'%s\'", wstrId.c_str());
	cstrSql += cstrTemp;

	SendSql(cstrSql, CHG);
	return 0;
}

int CRecvTask::ProBackJson(boost::property_tree::wptree & pt, BYTE * pBuff){
	std::wstringstream is;
	boost::property_tree::write_json(is, pt);
	std::wstring wstr = is.str();

	SOCKET ClientSocket = pt.get<unsigned long>(L"socket");
	NET_PACKGE * pPackge = (PNET_PACKGE)pBuff;

	SOCKET_CONTEXT * pSocketContext = NULL;
	g_ClientSocketManager.GetSocketContext(ClientSocket, pSocketContext);
	if (pSocketContext != NULL){
		ITask * pTask = new CSendTask(pSocketContext->m_Socket, pBuff, pPackge->m_iLen);
		m_pPool->Handle(pTask);
	}
	return 0;
}

int CRecvTask::GetStuInfoFromTree(boost::property_tree::wptree & pt, std::wstring & rwstrId, std::wstring & rwstrName, std::wstring & rwstrTel, int & riSex)
{
	rwstrId = L"";
	rwstrName = L"";
	rwstrTel = L"";
	riSex = 2;

	boost::property_tree::wptree pt_stu;

	pt_stu = pt.get_child(L"student_info");
	if (pt_stu.empty()){
		return -1;
	}

	rwstrId = pt_stu.get<std::wstring>(L"stu_id");
	rwstrName = pt_stu.get<std::wstring>(L"stu_name");
	rwstrTel = pt_stu.get<std::wstring>(L"stu_tel");
	riSex = pt_stu.get<int>(L"stu_sex");

	return 0;
}


int CRecvTask::SendSql(CString & rcstrSql, int iType)
{

	boost::property_tree::wptree pt_sql;
	pt_sql.put(L"socket", m_pSocketContext->m_Socket);
	pt_sql.put(L"sql", rcstrSql.GetBuffer());

	switch (iType)
	{
	case ADD:
		pt_sql.put(L"sql_type", L"add");
		break;
	case SCH:
		pt_sql.put(L"sql_type", L"sch");
		break;
	case DEL:
		pt_sql.put(L"sql_type", L"del");
		break;
	case CHG:
		pt_sql.put(L"sql_type", L"chg");
		break;
	case BACK:
		pt_sql.put(L"sql_type", L"back");
		break;
	default:
		OutputDebugString(L"SendSql(), 遇到无法解析的包，发送失败,放弃发送\r\n");
		return -1;
	}

	std::wstringstream is;
	boost::property_tree::write_json(is, pt_sql);
	std::wstring wsttest;
	wsttest = is.str();

	int iLen = 0;

	iLen = 4 + (wsttest.length() + 1)*sizeof(WCHAR);
	char * pBuff = new char[iLen];

	PNET_PACKGE pPackge = (PNET_PACKGE)pBuff;
	pPackge->m_iLen = iLen;
	memcpy(pPackge->m_pData, wsttest.c_str(), iLen - 4);


	SOCKET_CONTEXT * pContext;
	g_ServerSocketManager.GetNextSocketContext(pContext);
	if (pContext != NULL){
		ITask * pTask = new CSendTask(pContext->m_Socket, pPackge, iLen); // 服务端存在向服务端发送数据
		m_pPool->Handle(pTask);
	}
	else{
		//ITask * pTask = new CSendTask(m_pSocketContext->m_Socket, -1, 4); // 服务端存在向服务端发送数据
		// 暂不处理
		OutputDebugString(L"ProSchJson(),GetNextSocketContext函数异常\r\n");
	}

	// 根据发送的结果完成相应的任务
	if (pBuff){
		delete pBuff;
		pBuff = NULL;
	}
	return 0;
}
