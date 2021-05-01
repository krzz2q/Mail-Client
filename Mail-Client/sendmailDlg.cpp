
// sendmailDlg.cpp: 实现文件

#include "pch.h"
#include "framework.h"
#include "sendmail.h"
#include "sendmailDlg.h"
#include "afxdialogex.h"

#include "Base64.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
CString cc_list[100];
int num_cc = 0;

void initial_socket() {
	/* 初始化套接字库 */
	// 定义参数 
	WORD wVersionRequested;
	WSADATA wsadata ;
	int err;
	wVersionRequested = MAKEWORD(2, 0); //宏MAKEWORD(high, low)
	//创建成功函数返回0
	err = WSAStartup(wVersionRequested, &wsadata);
	//if (err != 0) { std::cout << "初始化错误"; }
}




// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CsendmailDlg 对话框



CsendmailDlg::CsendmailDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SENDMAIL_DIALOG, pParent)
	
{
	/*如何获得？*/
	m_name = _T("");
	m_chao = _T("");
	m_password = _T("");
	m_address = _T("");
	m_context = _T("");
	m_rely = _T("");
	m_subject = _T("");

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CsendmailDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_NAME, m_name);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_CONTEXT, m_context);
	DDX_Text(pDX, IDC_RELY, m_rely);
	DDX_Text(pDX, IDC_SUBJECT, m_subject);
	DDX_Text(pDX, IDC_CHAO, m_chao);
	DDX_Control(pDX, IDC_CHAO, m_chaosong);
}

BEGIN_MESSAGE_MAP(CsendmailDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, &CsendmailDlg::OnBnClickedConnect)
	ON_BN_CLICKED(IDC_SENDBTN, &CsendmailDlg::OnBnClickedSendbtn)
	ON_BN_CLICKED(IDCANCEL, &CsendmailDlg::OnBnClickedCancel)
	//ON_EN_CHANGE(IDC_CHAO, &CsendmailDlg::OnEnChangeChao)
	ON_BN_CLICKED(IDC_BUTTON1, &CsendmailDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CsendmailDlg 消息处理程序

BOOL CsendmailDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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
	SetIcon(m_hIcon, TRUE);		// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	//ShowWindow(SW_MAXIMIZE);

	ShowWindow(SW_MINIMIZE);

	// TODO: 在此添加额外的初始化代码

	//初始化时除了登录按钮以外的按钮不亮
	//GetDlgItem(IDCANCEL)->EnableWindow(false);
	GetDlgItem(IDC_SENDBTN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CsendmailDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CsendmailDlg::OnPaint()
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
HCURSOR CsendmailDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CsendmailDlg::OnBnClickedConnect()
{
	UpdateData(true);
	/* 初始化套接字库 */
	initial_socket();

	/*创建服务器套接字句柄并获取SMTP服务器的IP地址 */
	socket_client = socket(AF_INET, SOCK_STREAM, NULL);
	if (INVALID_SOCKET == socket_client)
	{
		MessageBox("创建套接字失败");
		return;
	}

	//获取smtp服务器的IP地址
	hostent* host = gethostbyname(m_address);
	//将host->h_addr_list[0]所指向内存的4个字节拷贝到in_addr类型变量中
	in_addr in_addr_string;
	memmove(&in_addr_string, host->h_addr_list[0], 4);

	/*与服务器建立连接 */
	 sockaddr_in addr;
	//定义服务器套接字地址结构变量
	memset(&addr, 0, sizeof(sockaddr_in));
	//指定地址家族为TCP/IP
	addr.sin_family = AF_INET;
	//指定端口号
	addr.sin_port = htons(25);//htonl不可
	//服务器能够接收任何计算机发来的请求
	addr.sin_addr = in_addr_string;
	
	if (connect(socket_client, (sockaddr*)&addr, sizeof(sockaddr)))
	{
		int i = WSAGetLastError();
		CString err = "";
		err.Format("连接失败，错误号：%d", i);
		MessageBox(err);
		return;
	}
	
	/*接收来自服务器的信息，确认是否连接成功*/
	char buff[100] = "";
	if (!recv(socket_client, buff, 100, 0))
	{
		MessageBox("接收服务器的数据失败");
		return;
	}
	MessageBox(buff);

	/*发送helo smtp*/
	CString helo = "HELO smtp\r\n";
	if (send(socket_client, helo.GetBuffer(1), helo.GetLength(),0) == SOCKET_ERROR)
	{
		MessageBox("Error send HELO");
		return;
	}
	memset(buff, 0, 100);//重置buff
	/*接收服务器返回信息*/
	if (!recv(socket_client, buff, 100, 0))
	{
		MessageBox("Error recv HELO");
	}
	MessageBox(buff);

	/*登录验证*/
	//选择验证方式为LOGIN
	CString login = "auth login\r\n";
	send(socket_client, login.GetBuffer(1), login.GetLength(), 0);
	memset(buff, 0, 100);
	recv(socket_client, buff, 100, NULL);
	MessageBox(buff);

	CBase64 code;		//用于将验证信息加密
	CString login_name = m_name;
	login_name.Format("%s\r\n", code.Encode(login_name, login_name.GetLength()));
	//向服务器发送加密后的账号
	send(socket_client, login_name.GetBuffer(1), login_name.GetLength(), NULL);
	memset(buff, 0, 100);
	//接受服务器返回的信息
	recv(socket_client, buff, 100, NULL);
	MessageBox(buff);
	

	CString login_pass = m_password;
	login_pass.Format("%s\r\n", code.Encode(login_pass, login_pass.GetLength()));
	//向服务器发送加密后的密码
	send(socket_client, login_pass.GetBuffer(1), login_pass.GetLength(), NULL);
	memset(buff, 0, 100);
	//接受服务器返回的信息
	recv(socket_client, buff, 100, NULL);
	MessageBox(buff);
	//MessageBox("成功登陆！");

		//GetDlgItem(IDCANCEL)->EnableWindow(false);
	//登录后图标可用
	if (!((buff[0] == '5' && buff[1] == '3' && buff[2] == '5')|| (buff[0] == '5' && buff[1] == '0' && buff[2] == '2'))) {
		GetDlgItem(IDC_SENDBTN)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	}




}


void CsendmailDlg::OnBnClickedSendbtn()
{

	UpdateData(true);
	CString  send_message = "";	//用于发送信息
	char recv_message[512] = ""; //用于接收信息
	/*
		命令在程序中被发送时必须加上换行符号“\r\n”
		用户在构造完成整个邮件内容后，需要在邮件内容后面加上“\0”，
		表示数据内容发送或者接收完毕。
	*/

	/*发送mail from:<发件人>*/
	send_message = "MAIL FROM:<" + m_name + ">\r\n";//发件人信息
	if (SOCKET_ERROR == send(socket_client, send_message, send_message.GetLength(), NULL))
	{
		//int err = WSAGetLastError();
		MessageBox("Error send");
		return;
	}
	recv(socket_client, recv_message, 512, NULL);
	MessageBox(recv_message);

	/*发送RCPT TO:<收件人>*/
	send_message = "RCPT TO:<" + m_rely + ">\r\n";
	if (SOCKET_ERROR == send(socket_client, send_message, send_message.GetLength(), NULL))
	{
		MessageBox("Error send");
		return;
	}
	memset(recv_message, 0, 512);
	recv(socket_client, recv_message, 512, NULL);
	MessageBox(recv_message);

	/*发送RCPT TO:<抄送人>*/
	for (int i = 0; i < num_cc; i++) {
		send_message = "RCPT TO:<" + cc_list[i] + ">\r\n";
		MessageBox(send_message);
	 if (SOCKET_ERROR == send(socket_client, send_message, send_message.GetLength(), NULL))
	{
		MessageBox("Error send");
		return;
	 }
	memset(recv_message, 0, 512);
	recv(socket_client, recv_message, 512, NULL);
	 MessageBox(recv_message);
	}
	num_cc = 0;



	/*发送"DATA命令",表名客户端将要发送邮件到服务器*/
	send_message = "DATA\r\n";
	if (SOCKET_ERROR == send(socket_client, send_message, send_message.GetLength(), NULL))
	{
		MessageBox("Error send");
		return;
	}
	memset(recv_message, 0, 512);
	//服务器收到命令后返回SMTP响应码到客户端，表示服务器已经准备好接受数据
	recv(socket_client, recv_message, 512, NULL);
	MessageBox(recv_message);

	/*发送邮件*/
	//HEADER部分
	send_message = "From:"; //FROM:发件人
	send_message += m_name;
	send_message += "\r\n";
	/*
	send_message = "To:"; //To:发件人
	send_message += m_rely;
	send_message += "\r\n";
	*/


	send_message += "Subject:";//Subject:主题
	send_message += m_subject;
	send_message += "\r\n\r\n";

	send_message += m_context;//邮件内容
	send_message += "\r\n\r\n";
	send_message += "\r\n.\r\n";//邮件结束标志
	//MessageBox(send_message);
	if (SOCKET_ERROR == send(socket_client, send_message, send_message.GetLength(), NULL))
	{
		MessageBox("Error send");
		return;
	}
	//接受
	memset(recv_message, 0, 512);
	recv(socket_client, recv_message, 512, NULL);
	MessageBox(recv_message);

	/*发送QUIT命令*/
	send_message = "QUIT\r\n";
	if (SOCKET_ERROR == send(socket_client, send_message, send_message.GetLength(), NULL))
	{
		MessageBox("Error send");
		return;
	}
	memset(recv_message, 0, 512);
	recv(socket_client, recv_message, 512, NULL);
	MessageBox(recv_message);
	GetDlgItem(IDC_RELY)->SetWindowText("");
	GetDlgItem(IDC_SUBJECT)->SetWindowText("");
	GetDlgItem(IDC_CONTEXT)->SetWindowText("");
	GetDlgItem(IDC_SENDBTN)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	

}


void CsendmailDlg::OnBnClickedCancel()
{
	
	//关闭套接字
	closesocket(socket_client);
	//卸载socket库
	WSACleanup();
	CDialogEx::OnCancel();
}

/*
void CsendmailDlg::OnEnChangeChao()
{ 
	cc_list[num_cc++] = m_chao;
	MessageBox("成功添加抄送");
}
*/

void CsendmailDlg::OnBnClickedButton1()
{
	//cc_list[num_cc++] = m_chao;
	CString Mchao;
	GetDlgItem(IDC_CHAO)->GetWindowText(Mchao);
	cc_list[num_cc++] = Mchao;
	//m_chao = "";

	MessageBox("成功添加抄送");
	
	m_chaosong.SetSel(0, -1);
	m_chaosong.Clear();
	m_chaosong.GetFocus();
	


	//MessageBox(cc_list[num_cc-1]);
}

