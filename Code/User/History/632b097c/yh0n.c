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

#define JSON_FORMAT(FILENAME, RESULT_PTR)                                      \
  do {                                                                         \
    FILE *file = fopen(FILENAME, "r");                                         \
    if (file == NULL) {                                                        \
      *(RESULT_PTR) = NULL;                                                    \
    } else {                                                                   \
      printf("%s does exist \n", FILENAME);                                    \
      fseek(file, 0, SEEK_END);                                                \
      long fsize = ftell(file);                                                \
      fseek(file, 0, SEEK_SET);                                                \
                                                                               \
      char *temp = malloc(sizeof(char) * (fsize + 1));                         \
      if (temp == NULL) {                                                      \
        fclose(file);                                                          \
        *(RESULT_PTR) = NULL;                                                  \
      } else {                                                                 \
        char ch;                                                               \
        int i = 0;                                                             \
        while ((ch = fgetc(file)) != EOF) {                                    \
          temp[i] = ch;                                                        \
          i++;                                                                 \
        }                                                                      \
        temp[i] = '\0';                                                        \
        fclose(file);                                                          \
        *(RESULT_PTR) = temp;                                                  \
      }                                                                        \
    }                                                                          \
  } while (0)

struct BinTree create_router() {
  struct BinTree tree;

  struct Route *route = &(struct Route){
      .html_path = "/resources/index.html",
      .css_path = "/resources/index.css",
  };
  struct Node *root = &(struct Node){
      .route = route,
      .key = "/index",
  };
  root = bin_t_insert(root, "/about",
                      &(struct Route){
                          .html_path = "/resources/about/about.html",
                          .css_path = "/resources/about/about.css",
                      });
  root = bin_t_insert(root, "/projects",
                      &(struct Route){
                          .html_path = "/resources/projects/projects.html",
                          .css_path = "/resources/projects/projects.css",
                      });
  return tree;
}

void handle_client(void *network_nodes) {
  // get the client struct ( encapsulation of the client socket ) from the void*
  // args given from pthread
  struct Client client = *((struct Client *)(((void **)network_nodes)[0]));
  okay("client ip : %s connected to the server",
       get_client_addr_str(((void **)network_nodes)[0]));
  // handle client runs whenever a client connects to the server socket
  struct BinTree router_tree = create_router();
  char *html_file;
  JSON_FORMAT(router_tree.root->route->html_path, &html_file);
  char buffer[2048], response[strlen(html_file + 4)];
  bzero(&buffer, 2048);
  read(client.sockfd, buffer, 2048);
  sprintf(response, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n");
  strcat(response, html_file);
  strcat(response, "\r\n");
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
