#pragma comment (lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <WinSock2.h>

using namespace std;
void err_handling(char *msg) {
  fputs(msg, stderr);
  fputc('\n', stderr);
  exit(1);
}
void main() {
  WSADATA wsaData;
  SOCKET clntSock;
  SOCKADDR_IN servAddr;
  char message[BUFSIZ];

  if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
    err_handling("wsastartup err");
  }

  clntSock = socket(PF_INET, SOCK_DGRAM, 0);
  if (clntSock == INVALID_SOCKET) {
    err_handling("clntsock err");
  }

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
  servAddr.sin_port = htons(9000);

  if (connect(clntSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
    err_handling("connect err");
  }

  while (1) {
    fgets(message, BUFSIZ, stdin);
    if (!strcmp(message, "q\n")) {
      break;
    }
    //sendto를 써도되나 connect한 상태이므로 데이터만 보내도된다
    send(clntSock, message, sizeof(message), 0);
    recv(clntSock, message, BUFSIZ, 0);
    cout << "message from : " << message << endl;
  }
  closesocket(clntSock);
  WSACleanup();
}