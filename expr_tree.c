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
  switch (tree->type) {
        case VALUE:
            return tree->n.value; // Return the value of the leaf node

        case UNARY_NEGATE:
            return -ET_evaluate(tree->n.child[LEFT]); // Negate the value of the left child

        case OP_ADD:
            return ET_evaluate(tree->n.child[LEFT]) + ET_evaluate(tree->n.child[RIGHT]); // Addition

        case OP_SUB:
            return ET_evaluate(tree->n.child[LEFT]) - ET_evaluate(tree->n.child[RIGHT]); // Subtraction

        case OP_MUL:
            return ET_evaluate(tree->n.child[LEFT]) * ET_evaluate(tree->n.child[RIGHT]); // Multiplication

        case OP_DIV:
            return ET_evaluate(tree->n.child[LEFT]) / ET_evaluate(tree->n.child[RIGHT]); // Division

        case OP_POWER:
            return pow(ET_evaluate(tree->n.child[LEFT]), ET_evaluate(tree->n.child[RIGHT])); // Power

        default:
            return 0.0; // Unknown type
    }
  //
  //return 0.0;
}


// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz)
{
  assert(tree);

  size_t written = 0;

    if (tree->type == VALUE) {
        written = snprintf(buf, buf_sz, "%.2f", tree->n.value); // Format the value
    } else {
        // If we have space, write the operator first
        char op_char = ExprNodeType_to_char(tree->type);
        written = snprintf(buf, buf_sz, "%c ", op_char);

        // Check for remaining space and write the left and right children
        size_t left_written = ET_tree2string(tree->n.child[LEFT], buf + written, buf_sz - written);
        if (left_written < buf_sz - written) {
            written += left_written;
            written += snprintf(buf + written, buf_sz - written, " "); // Add space
        }

        size_t right_written = ET_tree2string(tree->n.child[RIGHT], buf + written, buf_sz - written);
        if (right_written < buf_sz - written) {
            written += right_written;
        }
    }

    return written; // Return the total number of characters written


  //
  // TODO: Add your code here
  //
}


