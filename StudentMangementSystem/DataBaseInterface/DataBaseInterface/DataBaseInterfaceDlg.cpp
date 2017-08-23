
// DataBaseInterfaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DataBaseInterface.h"
#include "DataBaseInterfaceDlg.h"
#include "afxdialogex.h"



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


// CDataBaseInterfaceDlg 对话框



CDataBaseInterfaceDlg::CDataBaseInterfaceDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataBaseInterfaceDlg::IDD, pParent)
	, m_ListenIP(0)
	, m_ListenPort(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataBaseInterfaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS1, m_ListenIP);
	DDX_Text(pDX, IDC_EDIT1, m_ListenPort);
}

BEGIN_MESSAGE_MAP(CDataBaseInterfaceDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDataBaseInterfaceDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_NETSERVER, &CDataBaseInterfaceDlg::OnBnClickedBtnNetserver)
	ON_BN_CLICKED(IDCANCEL, &CDataBaseInterfaceDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDataBaseInterfaceDlg 消息处理程序

BOOL CDataBaseInterfaceDlg::OnInitDialog()
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
	InitDataMenber();
	InitButtonStatus();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDataBaseInterfaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDataBaseInterfaceDlg::OnPaint()
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
HCURSOR CDataBaseInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDataBaseInterfaceDlg::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	// 连接数据库
	if (ConnectDatabase2() == NOERROR){
		MessageBox(TEXT("连接数据库成功\r\n"));
		OutputDebugString(TEXT("连接数据库成功\r\n"));
		CWnd *pWnd = GetDlgItem(IDC_BTN_NETSERVER);
		pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDOK);
		pWnd->EnableWindow(FALSE);
		return;
	}
	MessageBox(TEXT("连接数据库失败\r\n"));
	return;
}


void CDataBaseInterfaceDlg::OnBnClickedBtnNetserver()
{
	// TODO:  在此添加控件通知处理程序代码

	// 开启网络服务
	if (StarListening()){
		if (AcceptConnected()){
			MessageBox(TEXT("网络连接成功\r\n"));
			OutputDebugString(TEXT("网络连接成功\r\n"));
			CWnd *pWnd = GetDlgItem(IDC_BTN_NETSERVER);
			pWnd->EnableWindow(FALSE);

			pWnd = GetDlgItem(IDOK);
			pWnd->EnableWindow(FALSE);
			return;
		}
	}
	MessageBox(TEXT("网络连接失败\r\n"));
	return;
}



int CDataBaseInterfaceDlg::InitButtonStatus()
{
	CWnd * pWnd = GetDlgItem(IDC_BTN_NETSERVER);
	pWnd->EnableWindow(FALSE);
	return 0;
}


int CDataBaseInterfaceDlg::ConnectDataBase()
{
	CoInitialize(NULL);
	//"adodb.Connection"
	//_uuidof(Connection)
	HRESULT hr = m_conn.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)){
		return FALSE;
	}
	OutputDebugString(TEXT("数据库对象实例初始化成功\r\n"));

	try{
		hr = m_conn->Open(DB_STR2, "", "", adConnectUnspecified);
		if (FAILED(hr))
		{
			printf("conn err");
		}
	}
	catch (_com_error e){
		//
		_bstr_t bstr = e.Description();
		OutputDebugString(bstr+TEXT("\r\n"));
	}
	return TRUE;
}


void CDataBaseInterfaceDlg::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码

	// 断开网络连接
	closesocket(m_socket);
	OutputDebugString(TEXT("断开网络连接\r\n"));
	// 断开数据库连接
	//m_conn->Release();
	OutputDebugString(TEXT("断开数据库连接\r\n"));
	CoUninitialize();
	OutputDebugString(TEXT("CoUninitialize\r\n"));
	CDialogEx::OnCancel();
}


DWORD CDataBaseInterfaceDlg::WaitConnectionThreadProc(LPVOID lpParameter){
	CDataBaseInterfaceDlg * pDlg = (CDataBaseInterfaceDlg *)lpParameter;


	while (TRUE){
		pDlg->m_socket = accept(pDlg->m_listen_socket, NULL, NULL);
		if (pDlg->m_socket == INVALID_SOCKET) {
			WSAGetLastError();
			CWnd * pWnd = pDlg->GetDlgItem(IDC_BTN_NETSERVER);
			pWnd->EnableWindow(TRUE);
			return -1;
		}
		else
			OutputDebugString(L"Client connected.\n");


		_RecordsetPtr rs,rs2;
		rs.CreateInstance(__uuidof(Recordset));
		std::wstring wstrJson;
		char * pBuff = NULL;
		while (TRUE)
		{
			// 接收数据
			//int nRet = recv(pDlg->m_socket, pbuff, nBuffLen, 0);
			int nRet = pDlg->RecvMessage();
			if (nRet ==  0){
				//////////////////////////////////////////////////////////////////////////
				int iLen = pDlg->m_Buff.GetBufferLen();
				pBuff = new char[iLen];
				if (pBuff)
					pDlg->m_Buff.Read((PBYTE)pBuff, iLen);
				else{
					pDlg->m_Buff.ClearBuffer();
					break;
				}
			}
			else if (nRet <= 0){
				OutputDebugString(TEXT("Connection closed\n"));
				break;
			}

			// 执行
			_variant_t t; // VARIANT or _variant_t
			std::wstring wstrJson = (WCHAR *)(pBuff + 4); // Debug
			std::wstring wstsql;
			pDlg->GetSql(wstsql,wstrJson);
			_bstr_t bstr = wstsql.c_str();
			try{
				// 判断数据类型对其进行相应的处理
				switch (pDlg->GetSqlType(wstrJson))
				{
				case ADD:
					rs = pDlg->m_conn->Execute(bstr, &t, adCmdText);
					rs2 = pDlg->m_conn->Execute(L"SELECT * FROM db_StudentManagement.dbo.tb_student_info WHERE @@IDENTITY= db_StudentManagement.dbo.tb_student_info.stu_id;", &t, adCmdText);
					break;
				case SCH:
					rs2 = pDlg->m_conn->Execute(bstr, &t, adCmdText);
					break;
				case DEL:
					rs2 = pDlg->m_conn->Execute(bstr, &t, adCmdText);
					break;
				case CHG:
					rs2 = pDlg->m_conn->Execute(bstr, &t, adCmdText);
					break;
				default:
					break;
				}
			}
			catch (_com_error e){
				OutputDebugString(e.ErrorMessage());
				OutputDebugString(L"\r\n");
			}


			// 发送数据
			//pDlg->DebugFunc(rs2); // Debug
			std::wstring wstrSocket;
			pDlg->GetSocket(wstrJson, wstrSocket);
			pDlg->MakeJsonStr(rs2, wstrJson, wstrSocket);
			if (pDlg->SendMessageToMid(wstrJson) < 0){
				break;
			}
			

			if (pBuff){
				delete[] pBuff;
				pBuff = NULL;
			}
		}
		closesocket(pDlg->m_socket);
		pDlg->m_socket = NULL;
	}
}


int CDataBaseInterfaceDlg::AcceptConnected(){
	DWORD dwThread = NULL;
	HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CDataBaseInterfaceDlg::WaitConnectionThreadProc, this, 0, &dwThread);

	if (!hThread){
		MessageBox(TEXT("网络线程创建失败\r\n"));
		OutputDebugString(TEXT("网络线程创建失败\r\n"));
		GetLastError();
		return FALSE;
	}
	OutputDebugString(TEXT("网络线程创建成功\r\n"));
	return TRUE;
}

/*
*	开启一个端口坚挺网络连接
*/
int CDataBaseInterfaceDlg::StarListening(){
	m_listen_socket = INVALID_SOCKET;

	m_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == m_listen_socket){
		OutputDebugString(TEXT("StarListening()服务器监听发生错误，无法INVALID_SOCKET == m_sAccept"));
		return 0;
	}

	m_listen_addrin = {0};


	m_listen_addrin.sin_family = AF_INET;
	m_listen_addrin.sin_port = htons(m_ListenPort);
	m_listen_addrin.sin_addr.S_un.S_addr = m_ListenIP; // 这里我写不好

	int nRet = bind(m_listen_socket,
		(sockaddr*)&m_listen_addrin,
		sizeof(m_listen_addrin));

	if (nRet == SOCKET_ERROR)
	{
		OutputDebugString(TEXT("socket绑定监听端口发生失败\r\n"));
		return TRUE;
	}

	//3. listen 监听

	nRet = listen(m_listen_socket, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		OutputDebugString(TEXT("socket监听失败\r\n"));
		return TRUE;
	}
	OutputDebugString(TEXT("socket开始监听监听\r\n"));
	return TRUE;
}

int CDataBaseInterfaceDlg::InitDataMenber()
{
	m_ListenIP = inet_addr("127.0.0.1");
	m_ListenPort = LISTENING_PORT;

	UpdateData(FALSE);
	return 0;
}

/*
* 调试函数这个函数h
*/
int CDataBaseInterfaceDlg::DebugFunc(_RecordsetPtr & rRs)
{
	rRs->MoveFirst();
	int nNum = rRs->Fields->GetCount();
	for (int i = 0; i < nNum; i++)
	{
		_bstr_t bstr = (char*)rRs->Fields->Item[(long)i]->Name;
		OutputDebugString(bstr + TEXT("\t"));
	}

	OutputDebugString(TEXT("\r\n"));
	//打印数据
	while (!rRs->MyEOF)
	{
		for (int i = 0; i < nNum; i++)
		{
			//printf("%s ", (char*)(_bstr_t)rRs->Fields->Item[(long)i]->Value);
			_bstr_t bstr = rRs->Fields->Item[(long)i]->Value;
			OutputDebugString(bstr + TEXT("\t"));
		}
		printf("\r\n");

		rRs->MoveNext();
	}
	return 0;
}


HRESULT CDataBaseInterfaceDlg::MakeJsonStr(_RecordsetPtr & rRs, std::wstring & wstrJson, std::wstring wstringsocket){
	boost::property_tree::wptree stuList;
	try{
		rRs->MoveFirst();
		int nNum = rRs->Fields->GetCount();
		if (nNum <= 0){
			return -1;
		}
		while (!rRs->MyEOF){
			boost::property_tree::wptree pt;
			for (int i = 0; i < nNum; ++i){
				std::wstring wstrKey, wstrValu;
				_bstr_t bstrKey = (char*)rRs->Fields->Item[(long)i]->Name;
				_bstr_t bstrValue = (char*)(_bstr_t)rRs->Fields->Item[(long)i]->Value;

				wstrValu = bstrValue;
				wstrKey = bstrKey;
				pt.put(wstrKey, wstrValu);
			}
			stuList.push_back(make_pair(L"", pt));
			rRs->MoveNext();
		}
	}
	catch (boost::property_tree::ptree_error e){
		OutputDebugStringA(e.what());
	}
	catch (_com_error e1){
		OutputDebugStringA(e1.Description());
	}

	boost::property_tree::wptree root;
	root.put_child(L"stu_list", stuList);
	root.put(L"socket", wstringsocket);
	root.put(L"sql_type", L"back");

	std::wstringstream os;
	boost::property_tree::write_json(os, root);
	wstrJson = os.str();
}

int CDataBaseInterfaceDlg::RecvMessage()
{

	const int nBuffLen = 0x7ffffff;
	static char * pbuff = new char[nBuffLen];
	int iLen;
	int iRet = recv(m_socket, (char *)&iLen, sizeof(iLen), MSG_PEEK);
	if (iRet > 0){
	}
	else if (iRet == 0){
		printf("Connection closed\n");
		return -1;
	}
	else{
		printf("recv failed: %d\n", WSAGetLastError());
		return -1;
	}

	int iResult = 0;
	do {

		iResult = recv(m_socket, pbuff, nBuffLen, 0);
		if (iResult > 0){
			TRACE("Bytes received: %d\n", iResult);
			m_Buff.Write((PBYTE)pbuff, iResult);
			iLen = iLen - iResult;
		}
		else if (iResult == 0){
			printf("Connection closed\n");
			return -1;
		}
		else{
			printf("recv failed: %d\n", WSAGetLastError());
			return -1;
		}
	} while (iLen > 0);

	return 0;
}


HRESULT CDataBaseInterfaceDlg::GetSql(std::wstring & rwstr, std::wstring & rwstrJson)
{
	std::wstringstream is;
	boost::property_tree::wptree pt;
	is << rwstrJson;
	boost::property_tree::read_json(is, pt);

	rwstr = pt.get<std::wstring>(L"sql");
	return E_NOTIMPL;
}


int CDataBaseInterfaceDlg::GetSqlType(std::wstring & rwstrJson)
{
	std::wstringstream is;
	boost::property_tree::wptree pt;
	is << rwstrJson;
	boost::property_tree::read_json(is, pt);

	std::wstring wstr = pt.get<std::wstring>(L"sql_type");

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
	return NOSQL;
}

int CDataBaseInterfaceDlg::GetSocket(std::wstring & rwstrJson, std::wstring & rwstrSocket){
	std::wstringstream is;
	boost::property_tree::wptree pt;
	is << rwstrJson;
	boost::property_tree::read_json(is, pt);

	rwstrSocket = pt.get<std::wstring>(L"socket");
	return 0;
}

int CDataBaseInterfaceDlg::SendMessageToMid(std::wstring & rwstrJson)
{
	int iLen = (rwstrJson.length() + 1)*(sizeof(WCHAR))+sizeof(int);
	// 申请缓冲区，发送数据
	char * pBuff = new char[iLen];
	if (pBuff == NULL){
		return 0;
	}
	PNET_PACKGE pPackge = (PNET_PACKGE)pBuff;
	pPackge->m_iLen = iLen;
	memcpy_s(pPackge->m_pData, iLen - sizeof(int), rwstrJson.c_str(), iLen - sizeof(int));
	int iRet = send(m_socket, pBuff, iLen, 0);
	if (iRet == SOCKET_ERROR) {
		OutputDebugString(L"发送数据出现错误\r\n");
		return -1;
	}
	if (pBuff){
		delete[] pBuff;
		pBuff = NULL;
	}

	return iRet;
}


HRESULT CDataBaseInterfaceDlg::ConnectDatabase2()
{
	CoInitialize(NULL);

	HRESULT hr = m_conn.CreateInstance(_uuidof(Connection));
	if (FAILED(hr)){
		return FALSE;
	}

	char szPath[MAXBYTE] = "File Name=";
	char * pszPath = NULL;
	CString strConn = _T("File Name=connect.udl");
	try{
		strcat_s(szPath, MAXBYTE, "connect.udl");
		HRESULT hr = m_conn->Open(strConn.GetBuffer(), "", "", adConnectUnspecified);
		if (FAILED(hr)){
			OutputDebugString(L"数据库连接失败\r\n");
			return E_UNEXPECTED;
		}
	}
	catch (_com_error e){
		_bstr_t bstr = e.Description();
		OutputDebugString(bstr);
		return S_FALSE;
	}
	OutputDebugString(L"数据库连接成功\r\n");
	return NOERROR;
}
