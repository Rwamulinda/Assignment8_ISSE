/*
 * expr_tree.c
 * 
 * A dynamically allocated tree to handle arbitrary arithmetic
 * expressions
 *
 * Author: <your name here>
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

#include "expr_tree.h"

#define LEFT 0
#define RIGHT 1

struct _expr_tree_node {
  ExprNodeType type;
  union {
    struct _expr_tree_node *child[2];
    double value;
  } n;
};


/*
 * Convert an ExprNodeType into a printable character
 *
 * Parameters:
 *   ent    The ExprNodeType to convert
 * 
 * Returns: A single character representing the ent
 */
static char ExprNodeType_to_char(ExprNodeType ent) {
    switch (ent) {
        case OP_ADD: return '+';
        case OP_SUB: return '-';
        case OP_MUL: return '*';
        case OP_DIV: return '/';
        case OP_POWER: return '^';
        case UNARY_NEGATE: return '-';
        default: return '?';
    }
}



// Documented in .h file
ExprTree ET_value(double value) {
    ExprTree new = (ExprTree)malloc(sizeof(struct _expr_tree_node));
    assert(new);
    new->type = VALUE;
    new->n.value = value;
    return new;
}



// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
    ExprTree new = (ExprTree)malloc(sizeof(struct _expr_tree_node));
    assert(new);
    new->type = op;
    new->n.child[LEFT] = left;
    new->n.child[RIGHT] = right;
    return new;
}



// Documented in .h file
void ET_free(ExprTree tree) {
    if (tree == NULL) return;

    if (tree->type != VALUE) {
        ET_free(tree->n.child[LEFT]);
        ET_free(tree->n.child[RIGHT]);
    }

    free(tree);
}



// Documented in .h file
int ET_count(ExprTree tree)
{
  if (tree == NULL)
    return 0;

  if (tree->type == VALUE)
    return 1;

  return 1 + ET_count(tree->n.child[LEFT]) + ET_count(tree->n.child[RIGHT]);
}


// Documented in .h file
int ET_depth(ExprTree tree) {
    assert(tree);
    if (tree->type == VALUE) return 1;

    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}



// Documented in .h file
double ET_evaluate(ExprTree tree) {
    assert(tree);
    if (tree->type == VALUE) return tree->n.value;

    double left_val = ET_evaluate(tree->n.child[LEFT]);
    double right_val = ET_evaluate(tree->n.child[RIGHT]);

    switch (tree->type) {
        case OP_ADD: return left_val + right_val;
        case OP_SUB: return left_val - right_val;
        case OP_MUL: return left_val * right_val;
        case OP_DIV: return left_val / right_val;
        case OP_POWER: return pow(left_val, right_val);
        case UNARY_NEGATE: return -left_val;
        default: return 0.0;
    }
}



// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz) {
    assert(tree);
    if (buf_sz == 0) return 0;

    if (tree->type == VALUE) {
        return snprintf(buf, buf_sz, "%.2f", tree->n.value);
    }

    char op = ExprNodeType_to_char(tree->type);
    size_t left_len = ET_tree2string(tree->n.child[LEFT], buf, buf_sz);
    if (left_len >= buf_sz - 1) return left_len;

    buf[left_len] = op;
    size_t right_len = ET_tree2string(tree->n.child[RIGHT], buf + left_len + 1, buf_sz - left_len - 1);

    return left_len + 1 + right_len;
}

