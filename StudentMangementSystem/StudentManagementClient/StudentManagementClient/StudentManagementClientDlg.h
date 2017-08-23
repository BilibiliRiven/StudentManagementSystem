
// StudentManagementClientDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "sqlite3.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/lexical_cast.hpp"

#include <string>
#include <vector>

#define DB_NAME "..\\..\\ClientLocalDatabase\\db_LoacalStudentManagement.db"

typedef struct _NetPackge{
	int m_iLen;
	char m_pData[1];
}NET_PACKGE, *PNET_PACKGE ;


// CStudentManagementClientDlg �Ի���
class CStudentManagementClientDlg : public CDialogEx
{
// ����
	enum MyEnum
	{
		NOSQL,
		ADD,
		DEL,
		SCH,
		CHG,
		BACK,
	};
public:
	CStudentManagementClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_STUDENTMANAGEMENTCLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DWORD m_ConnectedIP;
	short m_ConnectedPort;
	CString m_StuId;
	CString m_StuName;
	CString m_StuPhone;
//	BOOL m_StuMan;
	afx_msg void OnBnClickedRadio2();
//	BOOL m_StuSexNone;
//	BOOL m_StuWoman;
	CProgressCtrl m_ctrProgress;
	CListCtrl m_ListCtrl;
	HRESULT IniteListCtrl();
	HRESULT ConnectServer();
	SOCKET m_Socket;
	SOCKADDR_IN m_addin;
	HRESULT IniteDataMember();
	afx_msg void OnBnClickedBtnLastpage();
	afx_msg void OnBnClickedBtnNextpage();
	afx_msg void OnBnClickedBtnAdd();
	int m_StuSex;
	static HRESULT RecvThreadProc(LPVOID lParam);
	HRESULT ParserJson(std::wstring & rwstr);
	HANDLE m_hRecvThread;
	HRESULT UpdateLoacalDB(std::wstring & rwstr_id, std::wstring & rwstr_name, std::wstring & stu_tel, int & ri_sex, int & ri_del, std::wstring & rwstr_tag);
private:
	sqlite3 *m_pDB;
public:
	HRESULT ConnecteDatabase();
	afx_msg void OnBnClickedNetSch();
	afx_msg void OnBnClickedNetChg();
	afx_msg void OnBnClickedDel();
	int EditStuJson(std::wstring & rwstrStuJson, int  iType);
	int SendJsonStr(std::wstring & rwstrJson);
	afx_msg void OnBnClickedBtnSearch();
	static int CStudentManagementClientDlg::ListStuInfocallback(void *Wnd, int argc, char **argv, char **azColName);
	afx_msg void OnBnClickedBtnClear();
	int ConnectServer2();
};
