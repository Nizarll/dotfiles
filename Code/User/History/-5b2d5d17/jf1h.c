#include "utils.h"
#include <netinet/in.h>

void handle_http_request() {}
void build_http_response(const char *file_name, const char *file_ext, char i) {}

struct Server server_init(int backlog, int domain, int service, int protocol,
                          int port, char *address,
                          void (*handle_client)(void *)) {
  struct Server server;
  memset(&server, 0, sizeof(struct Server));
  server.backlog = backlog;
  server.domain = domain;
  server.service = service;
  server.protocol = protocol;
  server.port = port;
  server.address.sin_family = domain;
  server.address.sin_port = htons(port);
  server.address.sin_addr.s_addr = inet_addr("127.0.0.1");

  server.sockfd = socket(server.domain, server.service, server.protocol);

  if (server.sockfd == 0)
    err("Failed to connect to socket %s", errno);
  okay("Created Server Successfully ! ");
  if ((bind(server.sockfd, (struct sockaddr *)&server.address,
            sizeof(server.address))) < 0)
    err("failed to bind socket %s", errno);
  if ((listen(server.sockfd, server.backlog)) < 0)
    err("failed to listen to the socket %s", errno);

  server.handle_client = handle_client;
  return server;
}

struct Client client_init(struct Server *server) {
  struct Client client;
  client.socklen = sizeof(struct sockaddr_in);
  if ((client.sockfd = accept(server->sockfd, (struct sockaddr *)&client.addr,
                              (socklen_t *)&client.socklen)) < 0) {
    warn("client failed to connect to server %s", errno);
  }
  return client;
}

char *get_client_addr_str(struct Client *client) {
  char *str = malloc(INET_ADDRSTRLEN +
                     6); // Maximum size for IPv4 + port (xxx.xxx.xxx.xxx:ppppp)

  if (str == NULL) {
    perror("malloc");
    exit(EXIT_FAILURE);
  }

  char ip[INET_ADDRSTRLEN];
  const char *ip_str = inet_ntop(AF_INET, &(client->addr), ip, INET_ADDRSTRLEN);
  if (ip_str == NULL) {
    perror("inet_ntop");
    free(str);
    exit(EXIT_FAILURE);
  }

  snprintf(str, INET_ADDRSTRLEN + 6, "%s:%d", ip, 3000);

  return str;
}

struct Node* bin_t_create_node(char* key, char* data);
struct Node* bin_t_insert(struct Node* root, char* key, char* data);
struct Node* bin_t_lookup(struct Node* root, char* key);