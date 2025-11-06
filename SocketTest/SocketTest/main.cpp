#ifdef _WIN64
#include <windows.h>
#define socklen_t int
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#define closesocket close
#endif

#include <thread>
#include <iostream>
using namespace std;

class TCPThread
{
public:
	void Main() { //线程的入口函数
		char buf[1024] = { 0 };
		cout << "client begin"<< endl;
		for (;;) {
			int recv_len = recv(client, buf, sizeof(buf) - 1, 0);
			if (recv_len <= 0) break;
			buf[recv_len] = '\0';
			if (strstr((buf), "quit") != NULL) {
				char res[] = "quit success!\n";
				send(client, res, strlen(res) + 1, 0);
				break;
			}

			int sendlen = send(client, "ok\n", 4, 0);

			cout << buf << endl;
		}
		closesocket(client);
		delete this;
	}
	int client = 0;
};

int main(int argc, char *argv[]) {
#ifdef _WIN64
	WSADATA ws;
	WSAStartup(MAKEWORD(2, 2), &ws);
#endif // _WIN64

	int sock1 = socket(AF_INET, SOCK_STREAM, 0);
	if (sock1 == -1) {
		cout << "create socket failed!" << endl;
		return -1;
	}

	unsigned short port = 8080;
	if (argc > 1) {
		port = atoi(argv[1]);
	}

	sockaddr_in saddr;
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port); //本地字节序转化为网络字节序
	saddr.sin_addr.s_addr = htonl(0);
	if (::bind(sock1, (sockaddr*)&saddr, sizeof(saddr)) != 0) {
		cout << "bind port " << port << " faild!" << endl;
		return -2;
	}
	else {
		cout << "bind port " << port << " success!" << endl;
	}

	listen(sock1, 10);
	for (;;) {
		sockaddr_in caddr;
		socklen_t len = sizeof(caddr);
		int client = accept(sock1, (sockaddr*)&caddr, &len);
		if (client <= 0) break;
		cout << "accept client " << client << endl;
		char* ip = inet_ntoa(caddr.sin_addr);
		unsigned short cport = ntohs(caddr.sin_port);
		cout << "client ip is " << ip << " , port is " << cport << endl;

		TCPThread *th = new TCPThread();
		th->client = client;
		thread sth(&TCPThread::Main, th);
		sth.detach();

	}
	
	closesocket(sock1);
	
	getchar();
	return 0;
}