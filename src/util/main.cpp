#include "util/ObjectPool.h"
#include <cstdio>

int main() {
  ObjectPool<int32_t, 128, false> pool;

  int32_t* a = pool.fetch();
  int32_t* b = pool.fetch();
  int32_t* c = pool.fetch();
  int32_t* d = pool.fetch();

  pool.release(b);
  int32_t* e = pool.fetch();

  fprintf(stdout, "fetch() %p\n", a);
  fprintf(stdout, "fetch() %p\n", b);
  fprintf(stdout, "fetch() %p\n", c);
  fprintf(stdout, "fetch() %p\n", d);
  fprintf(stdout, "fetch() %p\n", e);
  return 0;
}
