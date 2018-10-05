#include <sys/types.h>//socket()
#include <sys/socket.h>//socket()
#include <unistd.h>//close()
#include <arpa/inet.h>//结构体sockaddr_in
#include <string.h>//函数strstr
#include <iostream>
#include <thread>

using namespace std;

class TcpThread{
public:
  void main(){
    cout << "in tread main !!!!!!!!!!!!!!" << endl;    
    char buf[1024] = {0};
    while(true){
      //虽然指定了接收数据的大小为sizeof(buf) - 1，
      //但实际能接收到多少，是不一定的,
      //len1是实际接到的数据的大小
      int len1 = recv(client, buf, sizeof(buf) - 1, 0);
      cout << "recv len is : " << len1 << endl;
      if(len1 <= 0) break;
      buf[len1] = '\0';
      if(strstr(buf, "quit") != NULL){
	char re[] = "quit sucess\n";
	send(client, re, strlen(re) + 1, 0);
	break;
      }
      //给客户端发送信息，如果发送的数据大，会自动被切割车成很多小块
      //分多次发出去
      //虽然指定了发送数据的大小为3，
      //但实际能发出去多少，是不一定的,
      //sendlen是实际发送出去的数据的大小
      int sendlen = send(client, "OK\n", 4, 0);
      cout << "send len is : " << sendlen << endl;
      cout << "recv is : " << buf << endl;

    }
    close(client);
    delete this;
  }
  int client = 0;
};
int main(int argc, char* argv[]){

  unsigned short port = 8080;
  int sock = socket(AF_INET,SOCK_STREAM,0);
  if(argc > 1){
    port = atoi(argv[1]);
  }
  sockaddr_in saddr;
  saddr.sin_family = AF_INET;
  //把本地字节序，转成网络字节序
  saddr.sin_port = htons(port);
  saddr.sin_addr.s_addr = htonl(0);
  if(bind(sock,(sockaddr*)&saddr, sizeof(saddr)) != 0){
    cout << "bind failed" << endl;
    return -2;
  }
  cout << "success:" << port << endl;
  listen(sock, 10);
  while(true){
    sockaddr_in caddr;
    socklen_t len = sizeof(caddr);
    //根据原来的sock，生成一个新的socket，叫clinet
    //原来的sock是专门用来建立连接的
    int client = accept(sock, (sockaddr*)&caddr, &len);
    if(client <= 0)break;
    cout << client << endl;
    char *ip = inet_ntoa(caddr.sin_addr);
    //把网络字节序，转成本地字节序
    unsigned short cport = ntohs(caddr.sin_port);
    cout << "clinet ip is " << ip << " port is " << cport << endl;

    TcpThread* th = new TcpThread();
    th->client = client;
    //开启多线程
    std::thread sth(&TcpThread::main, th);
    //让子线程脱离父线程的监管a
    sth.detach();
  }
  close(sock);
}
