#ifndef HTMC_H
#define HTMC_H

#include "utils.h"

#define MAX_ROUTE_LENGTH 255
#define HTMC_ASSERT(X, ...)                                                    \
  do {                                                                         \
    if (!X)                                                                    \
      err("assert error ! : ", ##__VA_ARGS__);                                 \
  } while (0)

#define SERIALIZE_FILE() ()

typedef enum TYPES {
  JAVASCRIPT,
  CSS,
  IMAGE,
} Type;

struct Html {
  char route[MAX_ROUTE_LENGTH];
  char dependenies;
};

#endif // HTMC_H
