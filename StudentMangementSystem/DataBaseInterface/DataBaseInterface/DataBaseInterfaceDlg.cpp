
// DataBaseInterfaceDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DataBaseInterface.h"
#include "DataBaseInterfaceDlg.h"
#include "afxdialogex.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CDataBaseInterfaceDlg �Ի���



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


// CDataBaseInterfaceDlg ��Ϣ�������

BOOL CDataBaseInterfaceDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	InitDataMenber();
	InitButtonStatus();
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CDataBaseInterfaceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CDataBaseInterfaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDataBaseInterfaceDlg::OnBnClickedOk()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	// �������ݿ�
	if (ConnectDatabase2() == NOERROR){
		MessageBox(TEXT("�������ݿ�ɹ�\r\n"));
		OutputDebugString(TEXT("�������ݿ�ɹ�\r\n"));
		CWnd *pWnd = GetDlgItem(IDC_BTN_NETSERVER);
		pWnd->EnableWindow(TRUE);

		pWnd = GetDlgItem(IDOK);
		pWnd->EnableWindow(FALSE);
		return;
	}
	MessageBox(TEXT("�������ݿ�ʧ��\r\n"));
	return;
}


void CDataBaseInterfaceDlg::OnBnClickedBtnNetserver()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	// �����������
	if (StarListening()){
		if (AcceptConnected()){
			MessageBox(TEXT("�������ӳɹ�\r\n"));
			OutputDebugString(TEXT("�������ӳɹ�\r\n"));
			CWnd *pWnd = GetDlgItem(IDC_BTN_NETSERVER);
			pWnd->EnableWindow(FALSE);

			pWnd = GetDlgItem(IDOK);
			pWnd->EnableWindow(FALSE);
			return;
		}
	}
	MessageBox(TEXT("��������ʧ��\r\n"));
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
	OutputDebugString(TEXT("���ݿ����ʵ����ʼ���ɹ�\r\n"));

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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	// �Ͽ���������
	closesocket(m_socket);
	OutputDebugString(TEXT("�Ͽ���������\r\n"));
	// �Ͽ����ݿ�����
	//m_conn->Release();
	OutputDebugString(TEXT("�Ͽ����ݿ�����\r\n"));
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
			// ��������
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

			// ִ��
			_variant_t t; // VARIANT or _variant_t
			std::wstring wstrJson = (WCHAR *)(pBuff + 4); // Debug
			std::wstring wstsql;
			pDlg->GetSql(wstsql,wstrJson);
			_bstr_t bstr = wstsql.c_str();
			try{
				// �ж��������Ͷ��������Ӧ�Ĵ���
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


			// ��������
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
		MessageBox(TEXT("�����̴߳���ʧ��\r\n"));
		OutputDebugString(TEXT("�����̴߳���ʧ��\r\n"));
		GetLastError();
		return FALSE;
	}
	OutputDebugString(TEXT("�����̴߳����ɹ�\r\n"));
	return TRUE;
}

/*
*	����һ���˿ڼ�ͦ��������
*/
int CDataBaseInterfaceDlg::StarListening(){
	m_listen_socket = INVALID_SOCKET;

	m_listen_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (INVALID_SOCKET == m_listen_socket){
		OutputDebugString(TEXT("StarListening()�������������������޷�INVALID_SOCKET == m_sAccept"));
		return 0;
	}

	m_listen_addrin = {0};


	m_listen_addrin.sin_family = AF_INET;
	m_listen_addrin.sin_port = htons(m_ListenPort);
	m_listen_addrin.sin_addr.S_un.S_addr = m_ListenIP; // ������д����

	int nRet = bind(m_listen_socket,
		(sockaddr*)&m_listen_addrin,
		sizeof(m_listen_addrin));

	if (nRet == SOCKET_ERROR)
	{
		OutputDebugString(TEXT("socket�󶨼����˿ڷ���ʧ��\r\n"));
		return TRUE;
	}

	//3. listen ����

	nRet = listen(m_listen_socket, SOMAXCONN);

	if (nRet == SOCKET_ERROR)
	{
		OutputDebugString(TEXT("socket����ʧ��\r\n"));
		return TRUE;
	}
	OutputDebugString(TEXT("socket��ʼ��������\r\n"));
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
* ���Ժ����������h
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
	//��ӡ����
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
	// ���뻺��������������
	char * pBuff = new char[iLen];
	if (pBuff == NULL){
		return 0;
	}
	PNET_PACKGE pPackge = (PNET_PACKGE)pBuff;
	pPackge->m_iLen = iLen;
	memcpy_s(pPackge->m_pData, iLen - sizeof(int), rwstrJson.c_str(), iLen - sizeof(int));
	int iRet = send(m_socket, pBuff, iLen, 0);
	if (iRet == SOCKET_ERROR) {
		OutputDebugString(L"�������ݳ��ִ���\r\n");
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
			OutputDebugString(L"���ݿ�����ʧ��\r\n");
			return E_UNEXPECTED;
		}
	}
	catch (_com_error e){
		_bstr_t bstr = e.Description();
		OutputDebugString(bstr);
		return S_FALSE;
	}
	OutputDebugString(L"���ݿ����ӳɹ�\r\n");
	return NOERROR;
}
