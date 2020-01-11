#include "server.h"

Server::Server() {
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData) < 0) {
        exit(0);
    }
    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(990);  //端口
    if(::bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
        return;
    if(::listen(servSock, 20) == SOCKET_ERROR)
        return;
    cout << "The server has been started" << endl;

    while(1) {
        int i;
        for(i = 0; i < 4; i++) {
            if(soc[i].empt == 0) {
                soc[i].empt = 1;
                break;
            }
        }
        SOCKADDR clntAddr;
        int nSize = sizeof(SOCKADDR);
        soc[i].sock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);


        if(soc[i].sock == INVALID_SOCKET)
            return;

        std::thread t(talk, i);
        t.detach();

    }

    closesocket(servSock);
}

Server::~Server() {
    ::WSACleanup();
}

void Server::talk(int id) {
    char hello[] = "Welcome";
    ::send(soc[id].sock, hello, 7, 0);

    char buff[255];
    while(true) {
        memset(buff, 0, sizeof(buff));
        int reclen = recv(soc[id].sock, buff, 255, 0);
        buff[reclen]='\0';
        if( reclen > 0) {
            char buf[300];
            memset(buf,0,sizeof(buf));
            sprintf(buf, "%d", id);
            strcat(buf, " : ");
            strcat(buf, buff);
            for(int i = 0; i < 4; i++) {
                if(soc[i].empt) {
                    ::send(soc[i].sock, buf, strlen(buf), 0);
                }
            }

        } else {
            closesocket(soc[id].sock);
            soc[id].empt = 0;
            return;
        }
    }
}


/*

int main() {
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //绑定套接字
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;  //使用IPv4地址
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  //具体的IP地址
    sockAddr.sin_port = htons(990);  //端口
    bind(servSock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //进入监听状态
    listen(servSock, 20);
    //接收客户端请求
    SOCKADDR clntAddr;
    int nSize = sizeof(SOCKADDR);
    char bufSend[BUF_SIZE] = {0};
    char bufRecv[BUF_SIZE] = {0};

    while(1) {
        SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);

        printf("replying...");
        recv(clntSock, bufRecv, BUF_SIZE, 0);
        printf("\rMessage from client:%s\n", bufRecv);
        printf("Input:");
        gets(bufSend);
        send(clntSock, bufSend, strlen(bufSend), 0);
        memset(bufSend, 0, BUF_SIZE);
        memset(bufRecv, 0, BUF_SIZE);
        closesocket(clntSock);

    }
    closesocket(servSock);
    WSACleanup();
    return 0;


    SOCKET clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &nSize);
    //向客户端发送数据
    char *str = "Hello World!";
    send(clntSock, str, strlen(str) + sizeof(char), NULL);
    //关闭套接字
    closesocket(clntSock);
    closesocket(servSock);
    //终止 DLL 的使用
    WSACleanup();
    return 0;
}



*/



