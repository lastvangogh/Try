#ifndef CHATROOM_COMMON_H
#define CHATROOM_COMMON_H

#include <iostream>
#include <list>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 默认服务器端IP地址
#define SERVER_IP "127.0.0.1"

//服务器端口号
#define SERVER_PORT 8888

//int epool_create(int size)中的size 为epoll支持的最大句柄数
#define EPOLL_SIZE 5000

//缓冲区大小65535
#define BUF_SIZE 0xFFFF

//新用户登录后的欢迎信息
#define SERVER_WELCOME "Welcome you join to the chat room! Your chat ID is: Client #%d"

//其他用户收到消息的前缀
#define SERVER_MESSAGE "ClientID %d say >> %s"
#define SERVER_PRIVATE_MESSAGE "Client %d say to you privately >> %s"
#define SERVER_PRIVATE_ERROR_MESSAGE "Client %d is not in the chat room yet~"

//退出系统
#define EXIT "EXIT"

//提醒你是聊天室中唯一的客户
#define CAUTION "There is only one in the char room!"

//注册新的fd到epollfd中
//参数enable——et表示是否启用ET模式，如果为True则启用，否则使用LT模式
static void addfd( int epollfd, int fd, bool enable_et)
{
	struct epoll_event ev;
	ev.data.fd = fd;
	ev.events = EPOLLIN;
	if( enable_et )
	{
		ev.events = EPOLLIN | EPOLLET;
	}
	epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &cv);
	//设置socket为非阻塞模式
	//套接字立刻返回，不管I/O是否完成，该函数所在的线程会继续进行
	//eg.在revc(fd...)时，该函数立刻返回，在返回时，内核数据还没准备好会返回WSAEWOULDBLOCK错误代码
	fcntl(fd, F_SETFL, fcntl(fd, F_GETFD, 0)| 0_NONBLOCK);
	std::cout<<"fd added to epoll!"<<endl<<endl;
}

//定义信息结构， 在服务端和客户端之间传送
struct Msg
{
	int type;
	int fromID;
	int toID;
	char content[BUF_SIZE];
};

#endif
