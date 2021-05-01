#pragma once


// RecvMail 对话框

class RecvMail : public CDialog
{
	DECLARE_DYNAMIC(RecvMail)

public:
	RecvMail(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~RecvMail();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RecvMail };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel();
	SOCKET socket_client;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnEnChangeAddress();
	afx_msg void OnEnChangeUsername();
	afx_msg void OnEnChangePassword();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnEnChangeFrom();
	afx_msg void OnEnChangeSubject();
	afx_msg void OnBnClickedGet();
	afx_msg void OnBnClickedConnect();
	afx_msg void OnEnChangeContext();
	CString m_address;
	CString m_username;
	CString m_password;
	CString m_from;
	CString m_subject;
	CListBox m_list;
	CString m_context;

	
};
