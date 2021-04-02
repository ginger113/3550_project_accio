#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

using namespace std;

int
main(int argc, char* argv[])
{
    int port = atoi(argv[2]);
    char* fileName = argv[3];
    
    if (argc != 4) {
        std::cerr << "ERROR: You  need 3 arguments.\n";
        return 0;
    }

    if (port < 1024) {
        std::cerr << "ERROR: incorrect port number\n";
        exit(1);
    }

  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in serverAddr;
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(port);     // short, network byte order
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

  // connect to the server
  if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
      std::cerr << "ERROR: connect\n";
    return 2;
  }

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  if (getsockname(sockfd, (struct sockaddr *)&clientAddr, &clientAddrLen) == -1) {
      std::cerr << "ERROR: getsockname\n";
    return 3;
  }

  char ipstr[INET_ADDRSTRLEN] = {'\0'};
  inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));


  // send/receive data to/from connection
  fstream file;
  file.open(fileName, ios::in | ios::binary);
  if (!file.is_open()) {
      std::cerr << "ERROR: can't load file\n";
      exit(EXIT_FAILURE);
  }
  std::string contents( (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

  int total = 0;
  int bytesleft = contents.length();
  int bytes_sent = 0;
  while (total < contents.length()) {
      bytes_sent = send(sockfd, contents.c_str() + total, bytesleft, 0);
      total += bytes_sent;
      bytesleft -= bytes_sent;
  }

  close(sockfd);

  return 0;
}