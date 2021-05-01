// Main_option.cpp: 实现文件
//

#include "pch.h"
#include "sendmail.h"
#include "Main_option.h"
#include "afxdialogex.h"

CsendmailDlg dlg1;
RecvMail dlg2;
// Main_option 对话框

IMPLEMENT_DYNAMIC(Main_option, CDialog)

Main_option::Main_option(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_Main_option, pParent)
{

}

Main_option::~Main_option()
{
}

void Main_option::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Main_option, CDialog)
	ON_BN_CLICKED(IDC_BUTTON2, &Main_option::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &Main_option::OnBnClickedButton1)
END_MESSAGE_MAP()


// Main_option 消息处理程序


void Main_option::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	dlg1.DoModal();
}


void Main_option::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	dlg2.DoModal();
}
