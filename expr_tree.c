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
        case OP_ADD: return '+';
        case OP_SUB: return '-';
        case OP_MUL: return '*';
        case OP_DIV: return '/';
        case OP_POWER: return '^';
        case UNARY_NEGATE: return '-'; // For unary negation
        default: return '?'; // For unknown types
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
 // Ensure memory allocation was successful

    new->type = VALUE; // Set the type to VALUE
    new->n.value = value; // Store the value in the node
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
 // Ensure memory allocation was successful

    new->type = op; // Set the type to the given operator
    new->n.child[LEFT] = left; // Assign the left child
    new->n.child[RIGHT] = right; // Assign the right child
  //

  return new;
}


// Documented in .h file
void ET_free(ExprTree tree)
{
   if (tree == NULL) {
        return; // If the tree is NULL, nothing to free
    }

    if (tree->type != VALUE) { // If it's not a leaf node, free children first
        ET_free(tree->n.child[LEFT]); // Free left child
        ET_free(tree->n.child[RIGHT]); // Free right child
    }

    free(tree); // Finally, free the current node

  return;
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


  //
  // TODO: Add your code here
  if (tree == NULL) {
        return 0; // If the tree is NULL, depth is 0
    }

    // If it's a leaf node (value), the depth is 1
    if (tree->type == VALUE) {
        return 1;
    }

    // Calculate depth of left and right children
    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);

    // Return the maximum depth plus one for the current node
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
  //

  return 0;
}


// Documented in .h file
double ET_evaluate(ExprTree tree)
{
  assert(tree);


  //
  // TODO: Add your code here

  switch (tree->type) {
        case VALUE:
            return tree->n.value; // Return the value of a leaf node
        case OP_ADD:
            return ET_evaluate(tree->n.child[LEFT]) + ET_evaluate(tree->n.child[RIGHT]);
        case OP_SUB:
            return ET_evaluate(tree->n.child[LEFT]) - ET_evaluate(tree->n.child[RIGHT]);
        case OP_MUL:
            return ET_evaluate(tree->n.child[LEFT]) * ET_evaluate(tree->n.child[RIGHT]);
        case OP_DIV:
            return ET_evaluate(tree->n.child[LEFT]) / ET_evaluate(tree->n.child[RIGHT]);
        case OP_POWER:
            return pow(ET_evaluate(tree->n.child[LEFT]), ET_evaluate(tree->n.child[RIGHT]));
        default:
            return 0.0; // In case of an unknown operation
    }
  //
  return 0.0;
}


// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz)
{
  assert(tree);


  //
  // TODO: Add your code here
  size_t written = 0; // To keep track of how much has been written to the buffer

    if (tree->type == VALUE) {
        // Write the value to the buffer, formatted to 2 decimal places
        written = snprintf(buf, buf_sz, "%.2f", tree->n.value);
    } else {
        // Write the left child to the buffer
        written += ET_tree2string(tree->n.child[LEFT], buf + written, buf_sz - written);

        // If there's enough space left in the buffer, write the operator
        if (written < buf_sz) {
            written += snprintf(buf + written, buf_sz - written, " %c ", ExprNodeType_to_char(tree->type));
        }

        // Write the right child to the buffer
        written += ET_tree2string(tree->n.child[RIGHT], buf + written, buf_sz - written);
    }

    // Ensure the buffer is null-terminated
    if (written < buf_sz) {
        buf[written] = '\0';
    } else if (buf_sz > 0) {
        buf[buf_sz - 1] = '$'; // Indicate truncation if buffer size is exceeded
        buf[buf_sz - 2] = '\0'; // Null-terminate
    }

    return written; // Return the number of characters written
  //
}


