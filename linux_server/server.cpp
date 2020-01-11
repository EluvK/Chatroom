//Linux Server

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#define BUF_SIZE 50

int main() {
    int serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);


    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.xxx.xxx.xxx");
    serv_addr.sin_port = htons(990);

    if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
        return -1;
    }


    listen(serv_sock, 20);


    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size = sizeof(clnt_addr);

    int clnt_sock = -1;

    do {
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
        if(clnt_sock < 0)
            printf("Connected Error, re-try to get connected with client ... \n");
    } while(clnt_sock < 0);




    char bufSend[BUF_SIZE] = {0};
    char bufRecv[BUF_SIZE] = {0};
    while(1) {

        printf("replying...");

        read(clnt_sock, bufRecv, BUF_SIZE - 1);
        if(strcmp(bufRecv, "close") == 0) {
            close(clnt_sock);
            close(serv_sock);
            return 0;
        }
        printf("\rMessage from client:%s\n", bufRecv);
        printf("Input:");
        scanf("%[^\n]", bufSend);
        write(clnt_sock, bufSend, strlen(bufSend));
        memset(bufSend, 0, BUF_SIZE);
        memset(bufRecv, 0, BUF_SIZE);

    }
    close(clnt_sock);
    close(serv_sock);

    return 0;
}
