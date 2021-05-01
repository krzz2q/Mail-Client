// RecvMail.cpp: 实现文件
//

#include "pch.h"
#include "sendmail.h"
#include "RecvMail.h"
#include "afxdialogex.h"

// RecvMail 对话框
void initial_socket_pop() {
	/* 初始化套接字库 */
	// 定义参数 
	WORD wVersionRequested;
	WSADATA wsadata;
	int err;
	wVersionRequested = MAKEWORD(2, 0); //宏MAKEWORD(high, low)
	//创建成功函数返回0
	err = WSAStartup(wVersionRequested, &wsadata);
	//if (err != 0) { std::cout << "初始化错误"; }
}

IMPLEMENT_DYNAMIC(RecvMail, CDialog)

RecvMail::RecvMail(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_RecvMail, pParent)
	, m_address(_T(""))
	, m_username(_T(""))
	, m_password(_T(""))
	, m_from(_T(""))
	, m_subject(_T(""))
	, m_context(_T(""))
{

}

RecvMail::~RecvMail()
{
}

void RecvMail::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ADDRESS, m_address);
	DDX_Text(pDX, IDC_USERNAME, m_username);
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDX_Text(pDX, IDC_FROM, m_from);
	DDX_Text(pDX, IDC_SUBJECT, m_subject);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_CONTEXT, m_context);
}


BEGIN_MESSAGE_MAP(RecvMail, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST, &RecvMail::OnLbnSelchangeList)
	ON_EN_CHANGE(IDC_ADDRESS, &RecvMail::OnEnChangeAddress)
	ON_EN_CHANGE(IDC_USERNAME, &RecvMail::OnEnChangeUsername)
	ON_EN_CHANGE(IDC_PASSWORD, &RecvMail::OnEnChangePassword)
	ON_BN_CLICKED(IDCANCEL, &RecvMail::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_FROM, &RecvMail::OnEnChangeFrom)
	ON_EN_CHANGE(IDC_SUBJECT, &RecvMail::OnEnChangeSubject)
	ON_BN_CLICKED(IDC_GET, &RecvMail::OnBnClickedGet)
	ON_BN_CLICKED(IDC_CONNECT, &RecvMail::OnBnClickedConnect)
	ON_EN_CHANGE(IDC_CONTEXT, &RecvMail::OnEnChangeContext)
END_MESSAGE_MAP()


// RecvMail 消息处理程序


void RecvMail::OnLbnSelchangeList()
{
	// TODO: 在此添加控件通知处理程序代码

	//(1)清空文本框以便重新接收数据。
	m_from = "";		//清空"发件人信息"
	m_subject = "";		//清空"邮件主题信息"
	m_context = "";		//清空"邮件内容“信息
	UpdateData(false);
	
	//(2)获取用户双击列表框时选择项的文本。
	/*代码中先后调用了ClistBox类的成员函数GetCursel()获取用户选择项的索引、
	  GetTextLen()获取索引所指文本的长度，GetText()获取索引所指的文本并保存
	  在动态分配的内存buff中。
	*/
	int index = m_list.GetCurSel();
	int length = m_list.GetTextLen(index);
	char* buff = new char[length + 1];
	m_list.GetText(index, buff);

	//(3)发送RETR命令，并接收返回信息。
	CString send_message;		//专用于发送消息
	send_message = "RETR ";		//发送RETR命令
	send_message += buff[0];
	delete buff;				//释放动态分配的内存
	send_message += "\r\n";
	if (SOCKET_ERROR == send(socket_client, send_message,
		send_message.GetLength(), NULL))
	{
		MessageBox("发送消息出错");
		return;
	}
	char recv_message[3000] = "";
	if (SOCKET_ERROR == recv(socket_client, recv_message, 256, NULL))
	{
		MessageBox("接收信息出错");
		return;
	}
	MessageBox(recv_message);

	//标识发件人、主题信息和邮件内容是否已经获取
	BOOL flag_from = false, flag_subject = false, flag_context = false;
	memset(recv_message, 0, 3000);
	int count = recv(socket_client, recv_message, 2999, NULL);
	int check = 0; //遍历接收内容

	while (count != SOCKET_ERROR)
	{
		//筛选需要的信息
		while (recv_message[check])
		{
			if (flag_from == false)
			{
				//遍历查找"From:"
				if (recv_message[check] == 'F' &&
					recv_message[check + 4] == ':')
				{
					char recv_from[50] = "";
					int recv_i = 0;

					int flag = 0; // 用于处理126与QQ的异常情况，只有收件人中有@时才接受该信息

					while (recv_message[check + 5 + recv_i] != '\r' &&
						recv_message[check + 6 + recv_i] != '\n')
					{
						if (recv_message[check + 5 + recv_i] == '@') flag = 1;
						recv_from[recv_i] = recv_message[check + 5 + recv_i];
						recv_i++;
					}
					if (flag)
					{
						m_from = recv_from;
						UpdateData(false);
						flag_from = true; //修改标识信息
					}

				}

			}

			if (flag_subject == false)
			{
				//遍历查找“SUBJECT：”
				if (recv_message[check] == 'S' && recv_message[check + 7] == ':')
				{
					char recv_subject[50] = "";
					int recv_i = 0;
					while (recv_message[check + 8 + recv_i] != '\r' &&
						recv_message[check + 9 + recv_i] != '\n')
					{
						recv_subject[recv_i] = recv_message[check + 8 + recv_i];
						recv_i++;
					}
					m_subject = recv_subject;
					UpdateData(false);
					flag_subject = true;   //修改标识信息
				}
			}

			//筛选邮件正文信息
			if (flag_context == true ||
				(recv_message[check] == '\r' &&
					recv_message[check + 1] == '\n' &&
					recv_message[check + 2] == '\r' &&
					recv_message[check + 3] == '\n'
					))
			{
				if (flag_context == false)
				{
					char recv_context[256] = "";
					int recv_i = 0;
					while (recv_message[check + 4 + recv_i] != NULL &&
						recv_message[check + 4 + recv_i] != '.')
					{
						recv_context[recv_i] = recv_message[check + 4 + recv_i];
						recv_i++;
					}
					if (recv_message[check + 4 + recv_i] == NULL)
					{
						m_context += recv_context;
						if (!flag_context) 
							flag_context = true;
						break;					//退出内层循环
					}
					if (recv_message[check + 4 + recv_i] == '.')
					{
						m_context += recv_context;
						UpdateData(false);
						break;					//退出内层循环
					}
					
				}
				if (flag_context == true)
				{
					char recv_context[256] = "";
					int recv_i = 0;
					while (recv_message[check + recv_i] != '.')
					{
						recv_context[recv_i] = recv_message[check + recv_i];
						recv_i++;
					}
					m_context += recv_context;
					if (recv_message[check + recv_i] == '.')
					{
						UpdateData(false);
						break;			//跳出循环
					}
				}
			}
			check++;
		}//while
		check = 0;				//保证上面的循环再回到起始位置

		//判断邮件是否结束
		int length = strlen(recv_message);
		if (recv_message[length - 3] == '.')
			break;

		//再次循环前的准备工作
		memset(recv_message, 0, 3000);
		count = recv(socket_client, recv_message, 2999, NULL);
	}//while
	//MessageBox(m_context);
	MessageBox("文件读取完毕");
}	


void RecvMail::OnEnChangeAddress()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void RecvMail::OnEnChangeUsername()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void RecvMail::OnEnChangePassword()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void RecvMail::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}


void RecvMail::OnEnChangeFrom()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void RecvMail::OnEnChangeSubject()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}




void RecvMail::OnBnClickedGet()
{
	//客户端登录成功后，获取邮箱中列表信息
	// TODO: 在此添加控件通知处理程序代码
	// 发送LIST命令 
	CString send_message;		//专用于发送消息
	send_message = "LIST\r\n";		//发送USER用户名
	if (SOCKET_ERROR == send(socket_client, send_message,
		send_message.GetLength(), NULL))
	{
		MessageBox("发送消息出错");
		return;
	}
	char recv_message[256] = "";
	if (SOCKET_ERROR == recv(socket_client, recv_message, 256, NULL))
	{
		MessageBox("接收信息出错");
		return;
	}

	char list_message[20] = "";
	int j = 0;
	//解析获取到的信息
	for (int i = 0; recv_message[i] != NULL; i++) {
		if (recv_message[i] == '\r' && recv_message[i + 1] == '\n') {
			if (list_message[0] == '+' || list_message[0] == '.') {
				memset(list_message, 0, 20);
				j = 0;
				i++;
				continue;
			}
			m_list.AddString(list_message);//将提取到的信息加到列表框中
			memset(list_message, 0, 20);
			j = 0;
			i++;
			continue;
		}
		list_message[j++] = recv_message[i];
	}

}


void RecvMail::OnBnClickedConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true);
	/* 初始化套接字库 */
	initial_socket_pop();

	//2.创建套接字并获取POP3服务器的IP地址
	socket_client = socket(AF_INET, SOCK_STREAM, NULL);
	if (socket_client == INVALID_SOCKET) {
		MessageBox("套接字创建失败");
	}

	//获取服务器IP地址 
	/*m_address 是由用户输入的POP3服务器的网址，通过调用函数
	gethostbyname()返回一个执行hostent结构的指针。hostent结构的成员h_addr_list
	包含了SMTP服务器网络字节序的IP地址，通过拷贝函数，将h_addr_list[0]所
	指向的内存的四个字节拷贝到了in_addt类型的变量中。
	*/
	hostent* host = gethostbyname(m_address);
	in_addr addr;
	memmove(&addr, host->h_addr_list[0], 4);

	//3.连接POP3服务器
	//需要用默认的端口号110和之前获得的IP地址填充sockaddr_in结构
	sockaddr_in socketaddr;
	memset(&socketaddr, 0, sizeof(sockaddr_in));
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(110);
	socketaddr.sin_addr = addr;

	//代码通过调用connect()函数来连接POP3服务器，调用recv()来接受来自服务器的响应信息，返回+OK表示连接成功。
	if (connect(socket_client, (sockaddr*)&socketaddr,
		sizeof(sockaddr)) )
	{
		MessageBox("与服务器连接失败");
		return;
	}
	char recv_message[256] = "";
	if (SOCKET_ERROR == recv(socket_client, recv_message, 256, NULL))
	{
		MessageBox("接受信息出错");
		return;
	}
	MessageBox(recv_message);

	//用户名验证
	CString send_message;		//专用于发送消息
	send_message = "USER ";		//发送USER用户名
	send_message += m_username;
	send_message += "\r\n";
	if (SOCKET_ERROR == send(socket_client, send_message, 
		send_message.GetLength(), NULL))
	{
		MessageBox("发送消息出错");
		return;
	}
	memset(recv_message, 0, 256);
	if (SOCKET_ERROR == recv(socket_client, recv_message, 256, NULL))
	{
		MessageBox("接收信息出错");
		return;
	}
	MessageBox(recv_message);

	//密码验证
	send_message = "PASS ";		//发送PASS密码
	send_message += m_password;
	send_message += "\r\n";
	if (SOCKET_ERROR == send(socket_client, send_message,
		send_message.GetLength(), NULL) )
	{
		MessageBox("发送消息出错");
		return;
	}
	memset(recv_message, 0, 256);
	if (SOCKET_ERROR == recv(socket_client, recv_message, 256, NULL) )
	{
		MessageBox("接收信息出错");
		return;
	}
	MessageBox(recv_message);

	if (recv_message[0] != '+') {
		MessageBox("登录失败，请确认用户名与密码！");
	}

	//POP3服务器接收明文的用户名和密码，代码先后向服务器发送了用户名和密码，
	//如果返回信息+OK，表示登录成功

	MessageBox("连接并登录成功！");

	GetDlgItem(IDC_ADDRESS)->EnableWindow(false);
	GetDlgItem(IDC_USERNAME)->EnableWindow(false);
	GetDlgItem(IDC_PASSWORD)->EnableWindow(false);

}

void RecvMail::OnCancel()
{
	closesocket(socket_client);
	WSACleanup();

	CDialog::OnCancel();
}

void RecvMail::OnEnChangeContext()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
