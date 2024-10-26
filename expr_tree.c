/*
 * expr_tree.c
 * 
 * A dynamically allocated tree to handle arbitrary arithmetic
 * expressions
 *
 * Author: Uwase Pauline
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>  // for snprintf

#include "expr_tree.h"

#define LEFT 0
#define RIGHT 1

struct _expr_tree_node {
  ExprNodeType type;
  union {
    struct _expr_tree_node *child[2];  // Children for interior nodes
    double value;                      // Value for leaf nodes
  } n;
};

/*
 * Convert an ExprNodeType into a printable character
 */
static char ExprNodeType_to_char(ExprNodeType ent) {
  switch (ent) {
    case OP_ADD: return '+';
    case OP_SUB: return '-';
    case OP_MUL: return '*';
    case OP_DIV: return '/';
    case OP_POWER: return '^';
    case UNARY_NEGATE: return '-';
    default: return '?';  // Unknown type
  }
}

// Create a value node
ExprTree ET_value(double value) {
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);  // Ensure memory allocation succeeded

  new->type = VALUE;
  new->n.value = value;
  return new;
}

// Create an interior node
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);  // Ensure memory allocation succeeded

  new->type = op;
  new->n.child[LEFT] = left;
  new->n.child[RIGHT] = right;
  return new;
}

// Free the tree by recursively freeing all nodes
void ET_free(ExprTree tree) {
  if (tree == NULL) return;

  if (tree->type != VALUE) {
    ET_free(tree->n.child[LEFT]);
    ET_free(tree->n.child[RIGHT]);
  }
  free(tree);
}

// Calculate the depth of the tree
int ET_depth(ExprTree tree) {
  if (tree == NULL) return 0;

  if (tree->type == VALUE) return 1;

  int left_depth = ET_depth(tree->n.child[LEFT]);
  int right_depth = ET_depth(tree->n.child[RIGHT]);

  return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

// Evaluate the expression represented by the tree
double ET_evaluate(ExprTree tree) {
  assert(tree);

  if (tree->type == VALUE) {
    return tree->n.value;
  }

  double left_value = ET_evaluate(tree->n.child[LEFT]);
  double right_value = ET_evaluate(tree->n.child[RIGHT]);

  switch (tree->type) {
    case OP_ADD: return left_value + right_value;
    case OP_SUB: return left_value - right_value;
    case OP_MUL: return left_value * right_value;
    case OP_DIV: 
      assert(right_value != 0);  // Avoid division by zero
      return left_value / right_value;
    case OP_POWER: return pow(left_value, right_value);
    default: assert(0);  // Invalid operator type
  }
  return 0.0;  // Should never reach here
}

// Convert tree to a string representation
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz) {
  assert(tree);

  if (tree->type == VALUE) {
    return snprintf(buf, buf_sz, "%.2f", tree->n.value);
  }

  char left_buf[256], right_buf[256];
  ET_tree2string(tree->n.child[LEFT], left_buf, sizeof(left_buf));
  ET_tree2string(tree->n.child[RIGHT], right_buf, sizeof(right_buf));

  size_t len = snprintf(buf, buf_sz, "(%s %c %s)", 
                        left_buf, ExprNodeType_to_char(tree->type), right_buf);

  if (len >= buf_sz) {
    buf[buf_sz - 2] = '$';  // Indicate truncation
    buf[buf_sz - 1] = '\0';
    return buf_sz - 1;
  }
  return len;
}
