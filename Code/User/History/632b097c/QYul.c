#include "libs/utils.h"
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILE_NAME 100
#define MAX_TYPE_LEN 10
#define MAX_TEXT_LEN 1024
#define MAX_CHILDREN 100

BinTree create_router(){

}

void handle_client(void *network_nodes) {
  struct Client client = *((struct Client *)(((void **)network_nodes)[0]));
  okay("client ip : %s connected to the server",
       get_client_addr_str(((void **)network_nodes)[0]));
  char buffer[2048], response[2048];
  bzero(&buffer, 2048);
  read(client.sockfd, buffer, 2048);
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  strcat(response, "<html><body><h1>Hello World!</h1></body></html>\r\n");
  int bytes_sent = send((client.sockfd), &response, strlen(response), 0);
  if (bytes_sent < 0) {
    err("could not send data %s", strerror(errno));
  }
  close(client.sockfd);
}

int main(void) {

  struct Server server = server_init(100, AF_INET, SOCK_STREAM, 0, 6969,
                                     "127.0.0.1", handle_client);
  for (;;) {
    struct Client client = client_init(&server);
    void *network_nodes[2];
    network_nodes[0] = &client;
    network_nodes[1] = &server;
    if (client.sockfd < 0) {
      continue;
    } else {
      pthread_t thread_id;
      pthread_create(&thread_id, NULL, (void *)&handle_client,
                     (void *)network_nodes);
    }
  }

  return EXIT_SUCCESS;
}
