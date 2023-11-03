#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  return 0;
}

void rbtree_print_preorder__(FILE *stream, const rbtree *t, node_t *n, size_t indent, size_t tab_size) {
  if (n == t->nil) {
    return;
  }
  
  for (size_t i = 0; i < indent; ++i) {
    fprintf(stream, " ");
  }

  fprintf(stream, "%d(%s)\n", n->key, (n->color == RBTREE_BLACK) ? "B" : "R");
  rbtree_print_preorder__(stream, t, n->left, indent + tab_size, tab_size);
  rbtree_print_preorder__(stream, t, n->right, indent + tab_size, tab_size);
}

int rbtree_print(FILE *stream, const rbtree *t) {
  if (stream == NULL) {
    return -1;
  }

  rbtree_print_preorder__(stream, t, t->root, 0, 4);
  return 0;
}
