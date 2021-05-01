#pragma once


#include "pch.h"
#include "Mysocket.h"
#include <iostream>


void initial_socket() {
	/* 初始化套接字库 */
	// 定义参数 
	WORD wVersionRequested;
	WSADATA wsadata = { 0 };
	int err;
	wVersionRequested = MAKEWORD(2, 2); //宏MAKEWORD(high, low)
	//创建成功函数返回0
	err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0) { std::cout << "初始化错误"; }
}
/*sockaddr_in create_addr(int port, const char* myaddr ) {
	/*创建地址信息
	//定义服务器套接字地址结构变量
	sockaddr_in addr;
	//指定地址家族为TCP/IP
	addr.sin_family = AF_INET;
	//指定端口号
	addr.sin_port = htonl(port);
	//服务器能够接收任何计算机发来的请求
	addr.sin_addr.s_addr = inet_addr(myaddr);
	return addr;
}
*/
sockaddr_in create_addr(int port, in_addr myaddr) {
	/*创建地址信息*/
	//定义服务器套接字地址结构变量
	sockaddr_in addr;
	//指定地址家族为TCP/IP
	addr.sin_family = AF_INET;
	//指定端口号
	addr.sin_port = htonl(port);
	//服务器能够接收任何计算机发来的请求
	addr.sin_addr = myaddr;
	return addr;
}