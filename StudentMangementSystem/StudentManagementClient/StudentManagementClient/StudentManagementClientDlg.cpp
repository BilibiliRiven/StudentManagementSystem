
// StudentManagementClientDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StudentManagementClient.h"
#include "StudentManagementClientDlg.h"
#include "afxdialogex.h"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/lexical_cast.hpp"
#include "Buffer.h"
#include <string>
#include <vector>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CStudentManagementClientDlg 对话框



CStudentManagementClientDlg::CStudentManagementClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStudentManagementClientDlg::IDD, pParent)
	, m_ConnectedIP(0)
	, m_ConnectedPort(0)
	, m_StuId(_T(""))
	, m_StuName(_T(""))
	, m_StuPhone(_T(""))
	//, m_StuMan(FALSE)
	//, m_StuSexNone(FALSE)
	//, m_StuWoman(FALSE)
	, m_StuSex(0)
	, m_pDB(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStudentManagementClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ConnectedIP);
	DDX_Text(pDX, IDC_EDIT1, m_ConnectedPort);
	DDX_Text(pDX, IDC_EDIT2, m_StuId);
	DDX_Text(pDX, IDC_EDIT3, m_StuName);
	DDX_Text(pDX, IDC_EDIT4, m_StuPhone);
	//  DDX_Radio(pDX, IDC_RADIO1, m_StuMan);
	//DDX_Radio(pDX, IDC_RADIO3, m_StuSexNone);
	//DDX_Radio(pDX, IDC_RADIO2, m_StuWoman);
	DDX_Control(pDX, IDC_PROGRESS2, m_ctrProgress);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	DDX_Radio(pDX, IDC_RADIO1, m_StuSex);
}

BEGIN_MESSAGE_MAP(CStudentManagementClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CStudentManagementClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CStudentManagementClientDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO2, &CStudentManagementClientDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_BTN_LASTPAGE, &CStudentManagementClientDlg::OnBnClickedBtnLastpage)
	ON_BN_CLICKED(IDC_BTN_NEXTPAGE, &CStudentManagementClientDlg::OnBnClickedBtnNextpage)
	ON_BN_CLICKED(IDC_BTN_ADD, &CStudentManagementClientDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_NET_SCH, &CStudentManagementClientDlg::OnBnClickedNetSch)
	ON_BN_CLICKED(IDC_NET_CHG, &CStudentManagementClientDlg::OnBnClickedNetChg)
	ON_BN_CLICKED(IDC_DEL, &CStudentManagementClientDlg::OnBnClickedDel)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CStudentManagementClientDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CStudentManagementClientDlg::OnBnClickedBtnClear)
END_MESSAGE_MAP()


// CStudentManagementClientDlg 消息处理程序

BOOL CStudentManagementClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	IniteListCtrl();
	IniteDataMember();
	ConnecteDatabase();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CStudentManagementClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStudentManagementClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStudentManagementClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStudentManagementClientDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	if (ConnectServer() == NOERROR){
		OutputDebugString(L"与中间层建立连接成功，准备开启接收线程");
		DWORD ThreadId = NULL;
		m_hRecvThread = CreateThread(NULL, NULL,
			(LPTHREAD_START_ROUTINE)(CStudentManagementClientDlg::RecvThreadProc),
			this,
			NULL,
			&ThreadId);
		if (m_hRecvThread){
			OutputDebugString(L"接收线程开启成功");
		}
	}
}


void CStudentManagementClientDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码
	closesocket(m_Socket);
	CDialogEx::OnCancel();
}


void CStudentManagementClientDlg::OnBnClickedRadio2()
{
	// TODO:  在此添加控件通知处理程序代码
}


HRESULT CStudentManagementClientDlg::IniteListCtrl()
{
	int col = 0;
	m_ListCtrl.InsertColumn(col++, TEXT("ID"), 0, 50);
	m_ListCtrl.InsertColumn(col++, TEXT("姓名"), 0, 100);
	m_ListCtrl.InsertColumn(col++, TEXT("电话"), 0, 100);
	m_ListCtrl.InsertColumn(col++, TEXT("性别"), 0, 100);
	m_ListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	return NOERROR;
}


HRESULT CStudentManagementClientDlg::ConnectServer()
{
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_Socket == INVALID_SOCKET) {
		OutputDebugString(TEXT("socket() 函数失败\r\n"));
		return E_UNEXPECTED;
	}
	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	m_addin.sin_family = AF_INET;
	m_addin.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_addin.sin_port = htons(m_ConnectedPort);

	//----------------------
	// Connect to server.
	int iResult = connect(m_Socket, (SOCKADDR *)& m_addin, sizeof(m_addin));
	if (iResult == SOCKET_ERROR) {
		OutputDebugString(TEXT("connect() 函数失败\r\n"));
		iResult = closesocket(m_Socket);
		if (iResult == SOCKET_ERROR)
			OutputDebugString(TEXT("closesocket 函数失败\r\n"));
		return E_UNEXPECTED;
	}


	return NOERROR;
}


HRESULT CStudentManagementClientDlg::IniteDataMember()
{
	m_ConnectedIP = 0;
	m_ConnectedPort = 10800;
	m_StuName = "";
	m_StuId = "";
	m_StuPhone = "";
	m_addin = { 0 };
	m_Socket = INVALID_SOCKET;
	UpdateData(FALSE);
	return E_NOTIMPL;
}


void CStudentManagementClientDlg::OnBnClickedBtnLastpage()
{
	// TODO:  在此添加控件通知处理程序代码

}


void CStudentManagementClientDlg::OnBnClickedBtnNextpage()
{
	// TODO:  在此添加控件通知处理程序代码

}


void CStudentManagementClientDlg::OnBnClickedBtnAdd()
{
	// TODO:  在此添加控件通知处理程序代码
	static char Buff[8192];

	UpdateData(TRUE);
	if (m_StuName.IsEmpty()){
		MessageBox(TEXT("姓名不能为空"));
		return; 
	}

	if (m_StuPhone.IsEmpty()){
		MessageBox(TEXT("电话不能为空"));
		return;
	}

	std::wstring wstrStuJson;
	EditStuJson(wstrStuJson, ADD);
	SendJsonStr(wstrStuJson);

	//int sex = m_StuSex;

	//boost::property_tree::wptree root, pt;
	//root.put(L"sql_type", L"add");

	//pt.put(L"stu_name", m_StuName.GetBuffer());
	//pt.put(L"stu_tel", m_StuPhone.GetBuffer());
	//pt.put(L"stu_sex", m_StuSex);

	//root.push_back(make_pair(L"student_info", pt));

	//std::wstring wstr;
	//std::wstringstream os;
	//boost::property_tree::write_json(os, root);
	//wstr = os.str();

	//int nLen = wstr.length()*sizeof(WCHAR) + 4 + sizeof(WCHAR);
	//PNET_PACKGE pPackge = (PNET_PACKGE)Buff;
	//pPackge->m_iLen = nLen;
	//memcpy(pPackge->m_pData, wstr.c_str(), nLen);
	////int nRet = send(m_Socket, "123", 4, 0);
	//int nRet = send(m_Socket, Buff, nLen, 0);

	//if (nRet == SOCKET_ERROR) {
	//	MessageBox(TEXT("网络连接出现异常，尝试重新连接\r\n"));
	//	closesocket(m_Socket);
	//	return ;
	//}


	return;
}

HRESULT CStudentManagementClientDlg::RecvThreadProc(LPVOID lParam)
{
	
	static char pBuff[8192];
	CBuffer Buffer;
	CStudentManagementClientDlg * pDlg = (CStudentManagementClientDlg *)lParam;
	while (TRUE){
		int iLen = 0;
		int iRet = recv(pDlg->m_Socket, (char *)&iLen, sizeof(iLen), MSG_PEEK);
		if (iRet > 0){
			OutputDebugString(L"Connection closed, 接收到线程头部信息\n");
		}
		else if (iRet == 0){
			OutputDebugString(L"Connection closed, 接收线程结束\n");
			closesocket(pDlg->m_Socket);
			pDlg->m_Socket = NULL;
			return -1;
		}
		else{
			OutputDebugString(L"recv failed: 接收线程结束\n");
			closesocket(pDlg->m_Socket);
			pDlg->m_Socket = NULL;
			return -1;
		}

		/*
		*	前面预读取的包的长度，这个循环用来接收数据包，将数据写到
		*	Buffer中一次读取出来
		*/
		int iResult = 0;
		do {

			iResult = recv(pDlg->m_Socket, pBuff, 8192, 0);
			if (iResult > 0){
				printf("Bytes received: %d\n", iResult);
			}
			else if (iResult == 0){
				OutputDebugString(L"Connection closed, 接收线程结束\n");
				closesocket(pDlg->m_Socket);
				pDlg->m_Socket = NULL;
				return -1;
			}
			else{
				OutputDebugString(L"recv failed: %d , 接收线程结束\n");
				closesocket(pDlg->m_Socket);
				pDlg->m_Socket = NULL;
				return -1;
			}
			Buffer.Write((PBYTE)pBuff, iResult);

			iLen = iLen - iResult;
		} while (iLen > 0);

		int iBuffLen = Buffer.GetBufferLen();
		BYTE * pBytes = new BYTE[iBuffLen];
		Buffer.Read(pBytes, iBuffLen);

		std::wstring wstr;
		wstr = (WCHAR *)(pBytes + 4);
		if (pBytes){
			delete[] pBytes;
			pBytes = NULL;
		}

		// 到这里表示收数据包收完了，处理数据包
		// 主要任务，解析json字符串拼成查询语句，插入到SQLite中。
		// 这样数据就接收完毕了。
		pDlg->ParserJson(wstr);
	}

	return E_NOTIMPL;
}


HRESULT CStudentManagementClientDlg::ParserJson(std::wstring & rwstr)
{
	std::wstringstream is;
	is << rwstr;
	boost::property_tree::wptree root;
	boost::property_tree::read_json(is, root);

	boost::property_tree::wptree pt_list;
	pt_list = root.get_child(L"stu_list");

	for (boost::property_tree::wptree::iterator ita = pt_list.begin();
		ita != pt_list.end();
		++ita)
	{
		// 读取并且将其插入本地数据库
		std::wstring wstrId = ita->second.get<std::wstring>(L"stu_id");
		std::wstring wstrName = ita->second.get<std::wstring>(L"stu_name");
		std::wstring wstrTel = ita->second.get<std::wstring>(L"stu_tel");
		int iSex = ita->second.get<int>(L"stu_sex");
		int iDel = ita->second.get<int>(L"stu_del");
		std::wstring wstrTag = ita->second.get<std::wstring>(L"stu_tag");
		UpdateLoacalDB(
			wstrId,
			wstrName,
			wstrTel,
			iSex,
			iDel,
			wstrTag);
	}

	return E_NOTIMPL;
}


HRESULT CStudentManagementClientDlg::UpdateLoacalDB(std::wstring & rwstr_id, std::wstring & rwstr_name, std::wstring & rwstr_tel, int & ri_sex, int & ri_del, std::wstring & rwstr_tag)
{

	CString cstr;
	/*
	*	初始化list控件
	*	-----------------------------------------------
	*	|学号id | 姓名 | 电话 | 性别 | 删除标识|时间戳|
	*	-----------------------------------------------
	*/
	cstr.Format(L"replace INTO tb_student_info  VALUES (%s,\'%s\',\'%s\',%d,%d,\'%s\')"
				 , rwstr_id.c_str()
				 , rwstr_name.c_str()
				 , rwstr_tel.c_str()
				 , ri_sex
				 , ri_del
				 , rwstr_tag.c_str());


	CStringA cstrSql1_a;
	cstrSql1_a = cstr;
	char * error_msg;
	
	int iRet = sqlite3_exec(m_pDB, cstrSql1_a.GetBuffer(), NULL, NULL, &error_msg);
	if (SQLITE_OK == iRet){
		return NOERROR;
	}

	CString cstr_erro;
	cstr_erro = error_msg;
	OutputDebugString(cstr_erro);

	return E_UNEXPECTED;
}


HRESULT CStudentManagementClientDlg::ConnecteDatabase()
{
	int rc = sqlite3_open(DB_NAME, &m_pDB);

	// 判断是否有连接故障
	if (rc){
		MessageBox(TEXT("本地数据库连接失败\r\n"));
	}
	else{
		OutputDebugString(TEXT("数据库连接成功\r\n"));
	}
	return E_NOTIMPL;
}


void CStudentManagementClientDlg::OnBnClickedNetSch()
{
	// TODO:  在此添加控件通知处理程序代码
	//static char Buff[8192];

	//UpdateData(TRUE);

	//int sex = m_StuSex;

	//boost::property_tree::wptree root, pt;
	//root.put(L"sql_type", L"sch");
	//pt.put(L"stu_id", m_StuId.GetBuffer());
	//pt.put(L"stu_name", m_StuName.GetBuffer());
	//pt.put(L"stu_tel", m_StuPhone.GetBuffer());
	//pt.put(L"stu_sex", m_StuSex);

	//root.push_back(make_pair(L"student_info", pt));

	//std::wstring wstr;
	//std::wstringstream os;
	//boost::property_tree::write_json(os, root);
	//wstr = os.str();

	//int nLen = wstr.length()*sizeof(WCHAR) + 4 + sizeof(WCHAR);
	//PNET_PACKGE pPackge = (PNET_PACKGE)Buff;
	//pPackge->m_iLen = nLen;
	//memcpy_s(pPackge->m_pData, nLen - 4, wstr.c_str(), nLen - 4);
	//int nRet = send(m_Socket, Buff, nLen, 0);

	//if (nRet == SOCKET_ERROR) {
	//	MessageBox(TEXT("网络连接出现异常，尝试重新连接\r\n"));
	//	closesocket(m_Socket);
	//	return;
	//}

	std::wstring wstrStuJson;
	EditStuJson(wstrStuJson, SCH);
	SendJsonStr(wstrStuJson);
}


void CStudentManagementClientDlg::OnBnClickedNetChg()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_StuId.IsEmpty()){
		MessageBox(TEXT("ID不能为空"));
		return;
	}

	std::wstring wstrStuJson;
	EditStuJson(wstrStuJson, CHG);
	SendJsonStr(wstrStuJson);

	OnBnClickedNetSch();
}


void CStudentManagementClientDlg::OnBnClickedDel()
{
	// TODO:  在此添加控件通知处理程序代码
	//static char Buff[8192];

	//UpdateData(TRUE);


	//boost::property_tree::wptree root, pt;
	//root.put(L"sql_type", L"del");
	//pt.put(L"stu_id", m_StuId.GetBuffer());
	//pt.put(L"stu_name", m_StuName.GetBuffer());
	//pt.put(L"stu_tel", m_StuPhone.GetBuffer());
	//pt.put(L"stu_sex", m_StuSex);

	//root.push_back(make_pair(L"student_info", pt));

	//std::wstring wstr;
	//std::wstringstream os;
	//boost::property_tree::write_json(os, root);
	//wstr = os.str();

	std::wstring wstrStuJson;
	EditStuJson(wstrStuJson, DEL);
	SendJsonStr(wstrStuJson);

	OnBnClickedNetSch();
	//int nLen = wstr.length()*sizeof(WCHAR) + 4 + sizeof(WCHAR);
	//PNET_PACKGE pPackge = (PNET_PACKGE)Buff;
	//pPackge->m_iLen = nLen;
	//memcpy_s(pPackge->m_pData, nLen - 4, wstr.c_str(), nLen - 4);
	//int nRet = send(m_Socket, Buff, nLen, 0);

	//if (nRet == SOCKET_ERROR) {
	//	MessageBox(TEXT("网络连接出现异常，尝试重新连接\r\n"));
	//	closesocket(m_Socket);
	//	return;
	//}
}


int CStudentManagementClientDlg::EditStuJson(std::wstring & rwstrStuJson, int  iType)
{

	static char Buff[8192];

	UpdateData(TRUE);

	boost::property_tree::wptree root, pt;
	switch (iType)
	{
	case ADD:
		root.put(L"sql_type", L"add");
		break;
	case DEL:
		root.put(L"sql_type", L"del");
		break;
	case CHG:
		root.put(L"sql_type", L"chg");
		break;
	case SCH:
		root.put(L"sql_type", L"sch");
		break;
	default:
		OutputDebugString(L"EditStuJson()无法识别类型,函数放弃生成Json字符串\r\n");
		return -1;
	}
	pt.put(L"stu_id", m_StuId.GetBuffer());
	pt.put(L"stu_name", m_StuName.GetBuffer());
	pt.put(L"stu_tel", m_StuPhone.GetBuffer());
	pt.put(L"stu_sex", m_StuSex);
	root.push_back(make_pair(L"student_info", pt));

	std::wstringstream os;
	boost::property_tree::write_json(os, root);
	rwstrStuJson = os.str();
	return 0;
}


int CStudentManagementClientDlg::SendJsonStr(std::wstring & rwstrJson)
{	
	static char Buff[8192];

	int nLen = (rwstrJson.length() + 1)*sizeof(WCHAR) + sizeof(int);
	PNET_PACKGE pPackge = (PNET_PACKGE)Buff;
	pPackge->m_iLen = nLen;
	memcpy_s(pPackge->m_pData, nLen - sizeof(int), rwstrJson.c_str(), nLen - sizeof(int));
	int nRet = send(m_Socket, Buff, nLen, 0);

	if (nRet == SOCKET_ERROR) {
		MessageBox(TEXT("网络连接出现异常，尝试重新连接\r\n"));
		closesocket(m_Socket);
		return -1;
	}
	return 0;
}


void CStudentManagementClientDlg::OnBnClickedBtnSearch()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	OnBnClickedNetSch();
	CString cstrSql;

	cstrSql.Format(L"SELECT * FROM tb_student_info WHERE stu_id like \'%s%%\'\
				and stu_name like \'%s%%\'\
				and stu_tel like \'%s%%\'\
				and stu_sex like \'%s\' \
				and stu_del=0"
				, m_StuId
				, m_StuName
				, m_StuPhone
				, m_StuSex>1 ? L"%%" : (m_StuSex == 0 ? L"0" : L"1"));
	CStringA cstrSql_a;
	cstrSql_a = cstrSql;
	char * pError;
	m_ListCtrl.DeleteAllItems();
	int iRet = sqlite3_exec(m_pDB, cstrSql_a.GetBuffer(), CStudentManagementClientDlg::ListStuInfocallback, this, &pError);
	if (iRet != SQLITE_OK){
		MessageBox(L"本地查询出现异常\r\n");
	}

}

int CStudentManagementClientDlg::ListStuInfocallback(void *Wnd, int argc, char **argv, char **azColName){
	CStudentManagementClientDlg * pDlg = (CStudentManagementClientDlg *)Wnd;
	CString cstrTemp;
	cstrTemp = argv[0];
	CString cstrTempName;
	pDlg->m_ListCtrl.InsertItem(0, cstrTemp);
	for (int i = 1; i < argc; ++i){
		cstrTemp = argv[i];
		cstrTempName = azColName[i];
		if (cstrTempName == L"stu_sex"){
			pDlg->m_ListCtrl.SetItemText(0, i, cstrTemp==L"1"?L"女":L"男");
		}
		else{
			pDlg->m_ListCtrl.SetItemText(0, i, cstrTemp);
		}
	}
	return 0;
}


void CStudentManagementClientDlg::OnBnClickedBtnClear()
{
	// TODO:  在此添加控件通知处理程序代码
	m_StuId = L"";
	m_StuPhone = L"";
	m_StuSex = 2;
	m_StuName = L"";
	UpdateData(TRUE);
}


int CStudentManagementClientDlg::ConnectServer2()
{
	return 0;
}
