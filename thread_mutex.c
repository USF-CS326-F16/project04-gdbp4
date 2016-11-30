#include "types.h"
#include "stat.h"
#include "user.h"

#define NTHREADS 2

int x = 0;

struct lock m;


int thread_func(void *arg)
{
  int id = (int) arg;
  int i;
  for (i = 0; i < 100000000; i++) {
    thread_lock_acquire(&m);
    x = x + 1;
    thread_lock_release(&m);
  }
  thread_exit(id);
  return 0;
}

int main(int argc, char **argv)
{
  void *stack;
  struct thread t[NTHREADS];
  int rv;
  int i;

  thread_lock_init(&m);

  for (i = 0; i < NTHREADS; i++) {
    stack = malloc(4096);
    thread_create(&t[i], thread_func, stack, (void *) i);
  }

  for (i = 0; i < NTHREADS; i++) {
    rv = thread_join(&t[i]);
    printf(1, "thread_join(&t[%d]) = %d\n", i, rv);
  }

  printf(1, "x = %d\n", x);

  exit();
}
