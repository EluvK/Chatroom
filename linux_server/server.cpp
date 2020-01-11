//Linux Server

#include <thread>
#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

static struct mystruct{
    int sock;
    int empt;
}soc[4];

class Server{
public:
    Server();
    static void talk(int id);

};

Server::Server() {

    int servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.xxx.xxx");
    serv_addr.sin_port = htons(990);

    if(bind(servSock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        return;
    }

    listen(servSock, 20);

    cout << "The server has been started" << endl;

    while(1) {
        int i;
        for(i = 0; i < 4; i++) {
            if(soc[i].empt == 0) {
                soc[i].empt = 1;
                break;
            }
        }


        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        soc[i].sock = -1;
        soc[i].sock = accept(servSock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);

        if(soc[i].sock == -1)
            return;
        std::thread t(talk, i);
        t.detach();
    }

    close(servSock);
}


void Server::talk(int id) {
    char hello[] = "Welcome";
    write(soc[id].sock, hello, 7);

    char buff[255];
    while(true) {
        memset(buff, 0, sizeof(buff));
        int reclen = read(soc[id].sock, buff, 255);
        buff[reclen] = '\0';
        if( reclen > 0) {
            char buf[300];
            memset(buf, 0, sizeof(buf));
            sprintf(buf, "%d", id);
            strcat(buf, " : ");
            strcat(buf, buff);
            for(int i = 0; i < 4; i++) {
                if(soc[i].empt) {
                    write(soc[i].sock, buf, strlen(buf));
                }
            }

        } else {
            close(soc[id].sock);
            soc[id].empt = 0;
            return;
        }
    }
}

int main(){
    Server server;
    return 0;
}
