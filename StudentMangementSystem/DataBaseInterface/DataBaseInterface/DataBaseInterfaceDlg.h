
// DataBaseInterfaceDlg.h : ͷ�ļ�
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
/*                   �����������йصĺ�                                 */
/*																		*/		
/************************************************************************/
#define LISTENING_PORT 12345
#define LISTENING_IP "0.0.0.0"

typedef struct _NetPackge{
	int m_iLen;
	char m_pData[1];
}NET_PACKGE, *PNET_PACKGE;

// CDataBaseInterfaceDlg �Ի���
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
// ����
public:
	CDataBaseInterfaceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_DATABASEINTERFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;
	_ConnectionPtr m_conn;
	SOCKET m_socket;
	SOCKET m_listen_socket;
	sockaddr_in m_listen_addrin;
	CBuffer m_Buff;
	// ���ɵ���Ϣӳ�亯��
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
	*	�������ݿ⣬
	*	���ӳɹ�����0������ʯ�巵��-1
	*/
	int ConnectDataBase();
	/*
	*	����һ���˿ڼ�ͦ��������
	*/
	int StarListening();
	/*
	*	����һ���̵߳ȴ���������
	*	�̳߳ɹ�����������0.
	*	�ֳ�����ʯ�巵��-1
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
