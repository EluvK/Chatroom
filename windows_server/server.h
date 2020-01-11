#ifndef server_h
#define server_h

#include <thread>
#include <iostream>
#include <string>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>

using namespace std;


static struct mystruct{
    SOCKET sock;
    int empt;
}soc[4];

class Server{
public:
    Server();
    ~Server();
    static void talk(int id);

private:
    SOCKET s;
};


#endif // server
