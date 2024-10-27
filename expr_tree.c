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
static char ExprNodeType_to_char(ExprNodeType ent)
{

  //
  switch (ent) {
     case VALUE: return 'v'; // value node
     case UNARY_NEGATE: return '-'; // unary negation
     case OP_ADD: return '+'; // addition
     case OP_SUB: return '-'; // subtraction
     case OP_MUL: return '*'; // multiplication
     case OP_DIV: return '/'; // division
     case OP_POWER: return '^'; // power
     default: return '?'; // unknown type
    }
  // TODO: Add your code here
  //

}


// Documented in .h file
ExprTree ET_value(double value)
{
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);


  //
  // TODO: Add your code her

  new->type = VALUE; // Set the type to VALUE
  new->n.value = value; // Store the value in the unio

  return new;
}


// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right)
{
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);

  new->type = op; // Set the type to the operation
  new->n.child[LEFT] = left; // Set the left child
  new->n.child[RIGHT] = right; // Set the right child

  return new;


  //
  // TODO: Add your code here
  //
}


// Documented in .h file
void ET_free(ExprTree tree)
{
  if (tree == NULL)
    return;


  if (tree->type != VALUE) { // Only free children if it's not a leaf
      ET_free(tree->n.child[LEFT]);
      ET_free(tree->n.child[RIGHT]);
  }

  free(tree); // Free the current node
  // TODO: Add your code here
  //

  //return;
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
int ET_depth(ExprTree tree)
{
  assert(tree);

  if (tree == NULL) return 0; // No tree has a depth of 0

    // Depth of the current node is 1 + max depth of its children
  int left_depth = ET_depth(tree->n.child[LEFT]);
  int right_depth = ET_depth(tree->n.child[RIGHT]);

  return 1 + (left_depth > right_depth ? left_depth : right_depth);


  //
  // TODO: Add your code here
  //

  //return 0;
}


// Documented in .h file
double ET_evaluate(ExprTree tree)
{
  assert(tree);


  //
  // TODO: Add your code here
  //
  return 0.0;
}


// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz)
{
  assert(tree);


  //
  // TODO: Add your code here
  //
}


