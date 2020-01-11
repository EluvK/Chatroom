// WindowsClient.c

#include<iostream>
#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>
#include <string>
#include <string.h>
#include <thread>
#include <atomic>
#include <mutex>
#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll


#define BUF_SIZE 100
#define ip_adress "127.0.0.1"
#define ip_adress2 "129.204.179.241"

#define msgn 5
using namespace std;
#define MAX 1000
#define wx 30
#define wy 80
SOCKET s;
mutex mu;
struct pos {
    int x;
    int y;
};
string msg[msgn];
int msgline = 0;

void SetPosition(int x, int y) {
    HANDLE winHandle;//句柄
    COORD pos = {x, y};
    winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    //设置光标位置
    SetConsoleCursorPosition(winHandle, pos);
}
pos GetPosition() {
    HANDLE gh_std_out;
    gh_std_out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO bInfo;
    GetConsoleScreenBufferInfo(gh_std_out, &bInfo);
    pos p;
    p.x = bInfo.dwCursorPosition.X;
    p.y = bInfo.dwCursorPosition.Y;
    return p;

}

void showMessage() {
    while(1) {
        mu.lock();
        for(int i = 0; i < msgn; i++) {
            if(!msg[i].empty()) {
//                printf("\b");
                pos p = GetPosition();
                if(msgline>=wx-3){
                    SetPosition(0,MAX);
                    printf("\n\n");
                }
                SetPosition(0, msgline);
                if(msgline < wx-3)
                    msgline++;
                cout << msg[i] << endl;

                SetPosition(p.x, p.y);
                msg[i].clear();
            }
        }
        mu.unlock();
    }

}

void recMessage() {
    while(1) {
        char buff[256];
        memset(buff, 0, sizeof(buff));
        int rec =::recv(s, buff, 255, 0);
        if(rec > 0) {
            int i;
            mu.lock();
            for(i = 0; i < msgn; i++) {
                if(msg[i].empty()) {
                    msg[i] = buff;
                    break;
                }
            }
            if(i == msgn)
                cout << "msg lost" << endl;
            mu.unlock();
        }
    }
}


int main() {
    system("mode con cols=80 lines=30");
    for(int i = 0; i < wx-2; i++)
        cout << endl;
    //初始化DLL
    WSADATA wsaData;

    if(::WSAStartup(MAKEWORD(2, 2), &wsaData) < 0)
        return 0;

    //创建套接字
    s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


    //向服务器发起请求
    sockaddr_in sockAddr;
    memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
    sockAddr.sin_family = PF_INET;
    sockAddr.sin_addr.s_addr = inet_addr(ip_adress2);
    sockAddr.sin_port = htons(990);

    //连接服务器
    if(connect(s, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR)) == SOCKET_ERROR)
        return 0;

    string str;
    char buff[255];
    memset(buff, 0, sizeof(buff));

    for(int i = 0; i < msgn; i++) {
        msg[i].clear();
    }

    thread tr(recMessage);
    thread ts(showMessage);
    tr.detach();
    ts.detach();

    while(1) {

        getline(cin, str);
        pos p = GetPosition();
        SetPosition(p.x, p.y - 1);
        for(int j = 0; j < str.size(); j++)
            cout << " ";

        SetPosition(p.x, p.y - 1);

        int i;
        for(i = 0; i < str.size(); i++)
            buff[i] = str[i];
        buff[i] = '\0';

        ::send(s, buff, i, 0);
    }

    closesocket(s);
    WSACleanup();
    return 0;

}
