
// sendmailDlg.h: 头文件
//

#pragma once


// CsendmailDlg 对话框
class CsendmailDlg : public CDialogEx
{
// 构造
public:
	CsendmailDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDMAIL_DIALOG };
#endif
	CString	m_name;
	CString	m_password;
	CString	m_address;
	CString	m_context;
	CString	m_rely;
	CString	m_subject;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON	m_hIcon;
	SOCKET	socket_client;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedConnect();
	afx_msg void OnBnClickedSendbtn();
	afx_msg void OnBnClickedCancel();
	//afx_msg void OnEnChangeChao();
	afx_msg void OnBnClickedButton1();
	// 抄送
	CString m_chao;
	// 抄送控件变量
	CEdit m_chaosong;
};
