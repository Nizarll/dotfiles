#ifndef UTILS_H
#define UTILS_H

#include "ansi.h"
#include <arpa/inet.h>
#include <errno.h>
#include <libwebsockets.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdio.h>
#include <string.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define okay(msg, ...)                                                         \
  printf(ANSI_COLOR_GREEN "[✓] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)
#define err(msg, ...)                                                          \
  do {                                                                         \
    printf(ANSI_COLOR_RED "[✘] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__);  \
    exit(1);                                                                   \
  } while (0)
#define warn(msg, ...)                                                         \
  printf(ANSI_COLOR_YELLOW "[!] | " msg ANSI_COLOR_RESET "\n", ##__VA_ARGS__)

struct Route {
 char* html_path;
 char* css_path;
 char* js_path;
};

struct Node {
  struct Route* route;
  struct Node *left, *right;
  char* key;
};

struct BinTree{
  struct Node* root;
};

struct Server {
  int sockfd;
  int domain;
  int backlog;
  int service;
  int protocol;
  int port;
  struct sockaddr_in address;
  void (*handle_client)(void *);
};

struct Client {
  int sockfd;
  struct sockaddr_in addr;
  socklen_t socklen;
};

char *get_client_addr_str(struct Client *client);
struct Client client_init(struct Server *server);
struct Server server_init(int backlog, int domain, int service, int protocol,
                          int port, char *address, void (*launch)(void *));
struct Node* bin_t_create_node(char* key, struct Route* route);
struct Node* bin_t_insert(struct Node* root, char* key, struct Route* route);
struct Node* bin_t_lookup(struct Node* root, char* key);
void bin_t_free(struct Node* root);
// struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in,
// size_t len
// void websocket_init(struct lws_protocols[]);
// int websocket_callback(struct lws *, enum lws_callback_reasons, void *, void
// *,
//                      size_t);

#endif // UTILS_H
