// WindowsClient.c

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll

#define BUF_SIZE 100

int main() {
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("129.204.xxx.xxx");
    sockAddr.sin_port = htons(990);

    //连接服务器
    while(connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
        printf("Connection Error! Now re-try to get connected ... \n");
    }
//    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));

    //获取用户输入的字符串并发送给服务器
    char bufSend[BUF_SIZE] = {0};
    char bufRecv[BUF_SIZE] = {0};
    while(1) {
        printf("Input your message: ");
        gets(bufSend);

        send(sock, bufSend, strlen(bufSend), 0);

        if(strcmp(bufSend, "close") == 0) {
            break;
        }
        printf("Replying...");

        //接收服务器传回的数据

        recv(sock, bufRecv, BUF_SIZE, 0);

        //输出接收到的数据
        printf("\rServer: %s\n", bufRecv);

        memset(bufSend, 0, BUF_SIZE);
        memset(bufRecv, 0, BUF_SIZE);
    }
    //关闭套接字
    closesocket(sock);

    //终止使用 DLL
    WSACleanup();

    printf("Message has been achieved, the Client now gonna close");
    system("pause");
    return 0;
}




/*

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#define BUF_SIZE 100
int main() {
    //初始化DLL
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    //创建套接字
    SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sockAddr.sin_port = htons(1234);

    char bufSend[BUF_SIZE]={0};
    char bufRecv[BUF_SIZE]={0};
    while(1){
        SOCKET sock = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);

    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    printf("Input:");
    gets(bufSend);
    send(sock,bufSend,strlen(bufSend),0);
    printf("replying...");
    recv(sock,bufRecv,BUF_SIZE,0);
    printf("\rMessage from server:%s\n",bufRecv);
    memset(bufSend,0,BUF_SIZE);
    memset(bufRecv,0,BUF_SIZE);
    closesocket(sock);
    }
    WSACleanup();
    return 0;

    connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
    //接收服务器传回的数据
    char szBuffer[MAXBYTE] = {0};
    recv(sock, szBuffer, MAXBYTE, NULL);
    //输出接收到的数据
    printf("Message form server: %s\n", szBuffer);
    //关闭套接字
    closesocket(sock);
    //终止使用 DLL
    WSACleanup();
    system("pause");

    return 0;
}
*/
