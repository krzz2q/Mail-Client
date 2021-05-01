#pragma once


// Main_option 对话框

class Main_option : public CDialog
{
	DECLARE_DYNAMIC(Main_option)

public:
	Main_option(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~Main_option();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Main_option };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
};
