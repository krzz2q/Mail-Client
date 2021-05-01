#pragma once


#include "pch.h"
#include "Mysocket.h"
#include <iostream>


void initial_socket() {
	/* ��ʼ���׽��ֿ� */
	// ������� 
	WORD wVersionRequested;
	WSADATA wsadata = { 0 };
	int err;
	wVersionRequested = MAKEWORD(2, 2); //��MAKEWORD(high, low)
	//�����ɹ���������0
	err = WSAStartup(wVersionRequested, &wsadata);
	if (err != 0) { std::cout << "��ʼ������"; }
}
/*sockaddr_in create_addr(int port, const char* myaddr ) {
	/*������ַ��Ϣ
	//����������׽��ֵ�ַ�ṹ����
	sockaddr_in addr;
	//ָ����ַ����ΪTCP/IP
	addr.sin_family = AF_INET;
	//ָ���˿ں�
	addr.sin_port = htonl(port);
	//�������ܹ������κμ��������������
	addr.sin_addr.s_addr = inet_addr(myaddr);
	return addr;
}
*/
sockaddr_in create_addr(int port, in_addr myaddr) {
	/*������ַ��Ϣ*/
	//����������׽��ֵ�ַ�ṹ����
	sockaddr_in addr;
	//ָ����ַ����ΪTCP/IP
	addr.sin_family = AF_INET;
	//ָ���˿ں�
	addr.sin_port = htonl(port);
	//�������ܹ������κμ��������������
	addr.sin_addr = myaddr;
	return addr;
}