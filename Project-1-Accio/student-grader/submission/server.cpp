#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
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
  // create a socket using TCP IP
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // allow others to reuse the address
  int yes = 1;
  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      std::cerr << "ERROR: setsockopt\n";
    return 0;
  }

  // bind address to socket
  int port = atoi(argv[1]);

  if (port < 1024) {
      std::cerr << "ERROR: incorrect port number\n";
      exit(1);
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);     // short, network byte order
  addr.sin_addr.s_addr = INADDR_ANY;
  memset(addr.sin_zero, '\0', sizeof(addr.sin_zero));

  if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
      std::cerr << "ERROR: bind";
    return 2;
  }
  int count = 0;
  for (;;) {
      // set socket to listen status
      if (listen(sockfd, 10) == -1) {
          std::cerr << "ERROR: listen\n";
          return 3;
      }
      count++;

      // accept a new connection
      struct sockaddr_in clientAddr;
      socklen_t clientAddrSize = sizeof(clientAddr);
      int clientSockfd = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrSize);

      if (clientSockfd == -1) {
          std::cerr << "ERROR: accept\n";
          return 4;
      }
      char ipstr[INET_ADDRSTRLEN] = { '\0' };
      inet_ntop(clientAddr.sin_family, &clientAddr.sin_addr, ipstr, sizeof(ipstr));

      // read/write data from/into the connection
      fstream file;
      std::string fileDir = std::string(argv[2]) + std::to_string(count) + ".file";
      file.open(fileDir, ios::out | ios::trunc | ios::binary);
      if (!file.is_open()) {
          exit(EXIT_FAILURE);
      }

      char buffer[1024];
      std::string myString;
      int valread;
      while ((valread = recv(clientSockfd, buffer, 1024, 0)) > 0) {
          myString.append(buffer, valread);
      }
      if (valread != -1) {
      file << myString;
      }
      else {
          file << "ERROR";
      }
      close(clientSockfd);
  }
  return 0;
}