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
  if (buf_sz == 0) return 0;  // No space to write anything
    assert(tree);

    size_t len = 0;

    if (tree->type == VALUE) {
        len = snprintf(buf, buf_sz, "%.2f", tree->n.value);
    } else {
        char left_buf[buf_sz], right_buf[buf_sz];
        size_t left_len = ET_tree2string(tree->n.child[LEFT], left_buf, buf_sz);
        size_t right_len = ET_tree2string(tree->n.child[RIGHT], right_buf, buf_sz);

        len = snprintf(buf, buf_sz, "(%s %c %s)",
                       left_buf, ExprNodeType_to_char(tree->type), right_buf);
    }

    if (len >= buf_sz) {
        buf[buf_sz - 2] = '$';  // Indicate truncation
        buf[buf_sz - 1] = '\0';
        return buf_sz - 1;
    }

    return len;
}


