/*
 * expr_tree.c
 * 
 * A dynamically allocated tree to handle arbitrary arithmetic
 * expressions
 *
 * Author: <Uwase Pauline>
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
  // TODO: Add your code here
  switch (ent) {
        case OP_ADD:      return '+';
        case OP_SUB:      return '-';
        case OP_MUL:      return '*';
        case OP_DIV:      return '/';
        case OP_POWER:    return '^';
        case UNARY_NEGATE: return '_';  // Using '_' to represent unary negation.
        default:          return '?';  // Unknown type.
    }
  //

}


// Documented in .h file
ExprTree ET_value(double value)
{
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);


  //
  // TODO: Add your code here
  new->type = VALUE;  // Set the node type to VALUE.
  new->n.value = value;  // Store the provided value.

  return new;
  //

  return new;
}


// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right)
{
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);


  //
  // TODO: Add your code here
  new->type = op;  // Set the operator type.
  new->n.child[LEFT] = left;  // Assign the left child.
  new->n.child[RIGHT] = right;  // Assign the right child.
  //

  return new;
}


// Documented in .h file
void ET_free(ExprTree tree)
{
  if (tree == NULL)
    return;


  //
  // TODO: Add your code here
  if (tree->type != VALUE) {  // Only free children if it's not a leaf.
      ET_free(tree->n.child[LEFT]);
      ET_free(tree->n.child[RIGHT]);
    }

    free(tree); 
  //

  return;
}


// Documented in .h file
int ET_count(ExprTree tree)
{
  if (tree == NULL)
      return 0;

  int left_count = ET_count(tree->n.child[LEFT]);
  int right_count = ET_count(tree->n.child[RIGHT]);
  printf("Node Type: %d, Left Count: %d, Right Count: %d\n", tree->type, left_count, right_count);

  return 1 + left_count + right_count;
}


// Documented in .h file
int ET_depth(ExprTree tree)
{
  assert(tree);


  //
  // TODO: Add your code here
  if (tree->type == VALUE)  // Base case: A leaf node has depth 1.
      return 1;

  // Recursive depth calculation for left and right children.
  int left_depth = ET_depth(tree->n.child[LEFT]);
  int right_depth = ET_depth(tree->n.child[RIGHT]);

  // Return the maximum depth plus 1 (for the current node).
  return 1 + (left_depth > right_depth ? left_depth : right_depth);
  //
}


// Documented in .h file
double ET_evaluate(ExprTree tree)
{
  assert(tree);


  //
  // TODO: Add your code here

  if (tree->type == VALUE)  // Base case: Return the leaf node’s value.
      return tree->n.value;

  // Recursively evaluate left and right children.
  double left_value = ET_evaluate(tree->n.child[LEFT]);
  double right_value = ET_evaluate(tree->n.child[RIGHT]);

  // Perform the operation based on the node type.
  switch (tree->type) {
        case OP_ADD:      return left_value + right_value;
        case OP_SUB:      return left_value - right_value;
        case OP_MUL:      return left_value * right_value;
        case OP_DIV:      assert(right_value != 0);  // Prevent division by zero.
                         return left_value / right_value;
        case OP_POWER:    return pow(left_value, right_value);
        case UNARY_NEGATE: return -left_value;
        default:          assert(0);  // Invalid operator.
    }
  return 0.0;  // Unreachable, but keeps compiler happy.
  //
  
}


// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz)
{
  assert(tree);


  //
  // TODO: Add your code here
  assert(buf);   // Ensure the buffer is not NULL.

  if (tree == NULL) {
      snprintf(buf, buf_sz, " ");  // Use buf_sz instead of bufsize
      return 0;
  }

  char left[256], right[256];  // Temporary buffers for child expressions

    // Generate string representations of left and right children (if any)
  if (tree->type != VALUE && tree->n.child[LEFT]) {
        ET_tree2string(tree->n.child[LEFT], left, sizeof(left));
  } else {
        snprintf(left, sizeof(left), " ");
  }

  if (tree->type != VALUE && tree->n.child[RIGHT]) {
        ET_tree2string(tree->n.child[RIGHT], right, sizeof(right));
  } else {
        snprintf(right, sizeof(right), " ");
  }

    // Handle different node types
  switch (tree->type) {
        case VALUE:
            snprintf(buf, buf_sz, "%.2f", tree->n.value);  // Print value with 2 decimal precision
            break;
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
            snprintf(buf, buf_sz, "(%s %c %s)", left, ExprNodeType_to_char(tree->type), right);
            break;
        case UNARY_NEGATE:
            snprintf(buf, buf_sz, "-%s", left);
            break;
        default:
            snprintf(buf, buf_sz, "UNKNOWN");
            break;
    }

    return strlen(buf);
}


