
// DataBaseInterfaceDlg.h : 头文件
//

#pragma once
#import "C:\Program Files (x86)\Common Files\System\ado\msado15.dll" no_namespace rename("EOF", "MyEOF")
#define DB_STR "Provider = SQLOLEDB.1; Integrated Security\
= SSPI; Persist Security Info = False; Data Source = USER - 20170804NE"
#define DB_STR2 TEXT("Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security Info=False;Initial Catalog=db_StudentManagement;Data Source=.")

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/lexical_cast.hpp"

#include <string>
#include <vector>
#include "Buffer.h"
/************************************************************************/
/*                   与网络连接有关的宏                                 */
/*																		*/		
/************************************************************************/
#define LISTENING_PORT 12345
#define LISTENING_IP "0.0.0.0"

typedef struct _NetPackge{
	int m_iLen;
	char m_pData[1];
}NET_PACKGE, *PNET_PACKGE;

// CDataBaseInterfaceDlg 对话框
class CDataBaseInterfaceDlg : public CDialogEx
{
	enum MyEnum
	{
		NOSQL,
		ADD,
		DEL,
		SCH,
		CHG,
		BACK,
	};
// 构造
public:
	CDataBaseInterfaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_DATABASEINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	_ConnectionPtr m_conn;
	SOCKET m_socket;
	SOCKET m_listen_socket;
	sockaddr_in m_listen_addrin;
	CBuffer m_Buff;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	DWORD m_ListenIP;
	short m_ListenPort;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnNetserver();
	int InitButtonStatus();
	/*
	*	连接数据库，
	*	连接成功返回0，连接石板返回-1
	*/
	int ConnectDataBase();
	/*
	*	开启一个端口坚挺网络连接
	*/
	int StarListening();
	/*
	*	开启一个线程等待网络连接
	*	线程成功启动，返回0.
	*	现场启动石板返回-1
	*/
	int AcceptConnected();
	static DWORD WaitConnectionThreadProc(LPVOID lpParameter);
	afx_msg void OnBnClickedCancel();
	int InitDataMenber();
	int DebugFunc(_RecordsetPtr & rRs);
	HRESULT MakeJsonStr(_RecordsetPtr & rRs, std::wstring & wstrJson, std::wstring wstringsocket);
	int RecvMessage();
	HRESULT GetSql(std::wstring & rwstr, std::wstring & rwstrJson);
	int GetSqlType(std::wstring & rwstrJson);
	int GetSocket(std::wstring & rwstrJson, std::wstring & rwstrSocket);
	int SendMessageToMid(std::wstring &  rwstrJson);
	HRESULT ConnectDatabase2();
};
