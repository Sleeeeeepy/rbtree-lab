#include "rbtree.h"

int main(int argc, char *argv[]) {
  const key_t arr[] = {10, 5, 8, 34, 67, 23, 156, 24, 2, 12, 24, 36, 990, 25};
  const size_t n = sizeof(arr) / sizeof(arr[0]);
  rbtree *t = new_rbtree();

  for (size_t i = 0; i < n; ++i) {
    rbtree_insert(t, arr[i]);
  }

  rbtree_print(stdout, t);
  delete_rbtree(t);
}