
// StudentManagementClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStudentManagementClientApp: 
// �йش����ʵ�֣������ StudentManagementClient.cpp
//

class CStudentManagementClientApp : public CWinApp
{
public:
	CStudentManagementClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStudentManagementClientApp theApp;