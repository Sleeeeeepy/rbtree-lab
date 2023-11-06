#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  if (p == NULL) {
    return NULL;
  }

  p->nil = (node_t *)calloc(1, sizeof(node_t));
  if (p->nil == NULL) {
    return NULL;
  }

  p->nil->color = RBTREE_BLACK;
  p->root = p->nil;
  
  return p;
}

node_t *new_node__(rbtree *t, key_t key) {
  node_t *n = (node_t *)calloc(1, sizeof(node_t));
  if (n == NULL) {
    return NULL;
  }

  n->color = RBTREE_RED;
  n->key = key;
  n->left = t->nil;
  n->right = t->nil;
  
  return n;
}

void delete_node__(rbtree *t, node_t *n) {
  if (t == NULL || n == NULL) {
    return;
  }

  if (n->left != t->nil) {
    delete_node__(t, n->left);
  }

  if (n->right != t->nil) {
    delete_node__(t, n->right);
  }

  free(n);
}

void delete_rbtree(rbtree *t) {
  node_t *root = t->root;
  if (root != NULL) {
    delete_node__(t, root);
  }
  
  if (t->nil != root) {
    free(t->nil);
    root = NULL;
    t->nil = NULL;
  }
  
  free(t);
}

void rbtree_left_rotate__(rbtree *t, node_t *n) {
  node_t *y = n->right;
  n->right = y->left;
  if (y->left != t->nil) {
    y->left->parent = n;
  }

  y->parent = n->parent;

  if (n->parent == t->nil) {
    t->root = y;
  } else if (n == n->parent->left) {
    n->parent->left = y;
  } else {
    n->parent->right = y;
  }

  y->left = n;
  n->parent = y;
}

void rbtree_right_rotate__(rbtree *t, node_t *n) {
  node_t *y = n->left;
  n->left = y->right;
  if (y->right != t->nil) {
    y->right->parent = n;
  }

  y->parent = n->parent;
  
  if (n->parent == t->nil) {
    t->root = y;
  } else if (n == n->parent->right) {
    n->parent->right = y;
  } else {
    n->parent->left = y;
  }

  y->right = n;
  n->parent = y;
}

void rbtree_insert_fixup__(rbtree *t, node_t *n) {
  node_t *uncle; 
  while (n->parent->color == RBTREE_RED) {
    if (n->parent == n->parent->parent->left) {
      uncle = n->parent->parent->right;
      if (uncle->color == RBTREE_RED) {
        n->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      } else {
        if (n == n->parent->right) {
          n = n->parent;
          rbtree_left_rotate__(t, n);
        }

        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        rbtree_right_rotate__(t, n->parent->parent);
      }
    } else {
      uncle = n->parent->parent->left;
      if (uncle->color == RBTREE_RED) {
        n->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        n = n->parent->parent;
      } else {
        if (n == n->parent->left) {
          n = n->parent;
          rbtree_right_rotate__(t, n);
        }

        n->parent->color = RBTREE_BLACK;
        n->parent->parent->color = RBTREE_RED;
        rbtree_left_rotate__(t, n->parent->parent);
      }
    }
  }

  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *parent = t->nil;
  node_t *cursor = t->root;
  while (cursor != t->nil) {
    parent = cursor;
    if (key < cursor->key) {
      cursor = cursor->left;
      continue;
    }
    cursor = cursor->right;
  }

  node_t *node = new_node__(t, key);
  node->parent = parent;
  if (parent == t->nil) {
    t->root = node;
  } else if (node->key < parent->key) {
    parent->left = node;
  } else {
    parent->right = node;
  }

  rbtree_insert_fixup__(t, node);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *cursor = t->root;
  while (cursor != t->nil) {
    if (cursor->key == key) {
      return cursor;
    }

    if (cursor->key > key) {
      cursor = cursor->left;
      continue;
    }

    if (cursor->key < key) {
      cursor = cursor->right;
      continue;
    }
  }

  return NULL;
}

node_t *rbtree_sub_min(const rbtree *t, node_t *subroot) {
  if (subroot == NULL) {
    return NULL;
  }

  node_t *cursor = subroot;
  while (cursor->left != t->nil) {
    cursor = cursor->left;
  }

  return cursor;
}

node_t *rbtree_sub_max(const rbtree *t, node_t *subroot) {
  if (subroot == NULL) {
    return NULL;
  }

  node_t *cursor = subroot;
  while (cursor->right != t->nil) {
    cursor = cursor->right;
  }

  return cursor;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *cursor = t->root;
  while (cursor->left != t->nil) {
    cursor = cursor->left;
  }

  return cursor;
}

node_t *rbtree_max(const rbtree *t) {
  node_t *cursor = t->root;
  while (cursor->right != t->nil) {
    cursor = cursor->right;
  }
  
  return cursor;
}

void rbtree_transplant__(rbtree *t, node_t *dest, node_t *src) {
  if (dest->parent == t->nil) {
    t->root = src;
  } else if (dest == dest->parent->left) {
    dest->parent->left = src;
  } else {
    dest->parent->right = src;
  }
  src->parent = dest->parent;
}

void rbtree_erase_fixup__(rbtree *t, node_t *n) {
  node_t *uncle;
  while (n != t->root && n->color == RBTREE_BLACK) {
    if (n == n->parent->left) {
      uncle = n->parent->right;
      if (uncle->color == RBTREE_RED) {
        uncle->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        rbtree_left_rotate__(t, n->parent);
        uncle = n->parent->right;
      }

      if (uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK) {
        uncle->color = RBTREE_RED;
        n = n->parent;
      } else {
        if (uncle->right->color == RBTREE_BLACK) {
          uncle->left->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          rbtree_right_rotate__(t, uncle);
          uncle = n->parent->right;
        }
        uncle->color = n->parent->color;
        n->parent->color = RBTREE_BLACK;
        uncle->right->color = RBTREE_BLACK;
        rbtree_left_rotate__(t, n->parent);
        n = t->root;
      }
    } else {
      uncle = n->parent->left;
      if (uncle->color == RBTREE_RED) {
        uncle->color = RBTREE_BLACK;
        n->parent->color = RBTREE_RED;
        rbtree_right_rotate__(t, n->parent);
        uncle = n->parent->left;
      }

      if (uncle->right->color == RBTREE_BLACK && uncle->left->color == RBTREE_BLACK) {
        uncle->color = RBTREE_RED;
        n = n->parent;
      } else {
        if (uncle->left->color == RBTREE_BLACK) {
          uncle->right->color = RBTREE_BLACK;
          uncle->color = RBTREE_RED;
          rbtree_left_rotate__(t, uncle);
          uncle = n->parent->left;
        }
        uncle->color = n->parent->color;
        n->parent->color = RBTREE_BLACK;
        uncle->left->color = RBTREE_BLACK;
        rbtree_right_rotate__(t, n->parent);
        n = t->root;
      }
    }
  }

  n->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
  node_t *x;
  node_t *y = p;
  color_t y_color = y->color;

  if (p->left == t->nil) {
    x = p->right;
    rbtree_transplant__(t, p, p->right);
  } else if (p->right == t->nil) {
    x = p->left;
    rbtree_transplant__(t, p, p->left);
  } else {
    y = rbtree_sub_min(t, p->right);
    y_color = y->color;
    x = y->right;
    if (y->parent == p) {
      x->parent = y;
    } else {
      rbtree_transplant__(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }

    rbtree_transplant__(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  if (y_color == RBTREE_BLACK) {
    rbtree_erase_fixup__(t, x);
  }

  free(p);
  return 0;
}

int rbtree_to_array_inorder__(const rbtree *t, node_t *n, key_t *arr, size_t idx, const size_t len) {
  if (n == t->nil) {
    return idx;
  }

  if (idx >= len) {
    return -1;
  }

  idx = rbtree_to_array_inorder__(t, n->left, arr, idx, len);
  if (idx == -1) {
    return -1;
  }

  arr[idx++] = n->key;
  idx = rbtree_to_array_inorder__(t, n->right, arr, idx, len);

  return idx;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  rbtree_to_array_inorder__(t, t->root, arr, 0, n);
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
