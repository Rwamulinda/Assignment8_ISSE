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
ExprTree ET_value(double value) {
    ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
    assert(new); // Ensure memory was allocated
    new->type = VALUE;
    new->n.value = value; // Initialize the value
    return new;
}

ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
    ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
    assert(new); // Ensure memory was allocated
    new->type = op;
    new->n.child[LEFT] = left; // Set the left child
    new->n.child[RIGHT] = right; // Set the right child
    return new;
}

  //
  // TODO: Add your code here


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
//int ET_depth(ExprTree tree)
//{
  //assert(tree);

  //if (tree == NULL) return 0; // No tree has a depth of 0

    // Depth of the current node is 1 + max depth of its children
 // int left_depth = ET_depth(tree->n.child[LEFT]);
  //int right_depth = ET_depth(tree->n.child[RIGHT]);

  //return 1 + (left_depth > right_depth ? left_depth : right_depth);

int ET_depth(ExprTree tree) {
    if (tree == NULL) { 
        return 0; // Base case: depth of a NULL tree is 0
    }

    if (tree->type == VALUE) {
        return 1; // A single leaf node has depth 1
    }

    // Recursively calculate the depth of left and right subtrees
    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);

    // Return the maximum depth of the two subtrees + 1 for the current node
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}



  //
  // TODO: Add your code here
  //

  //return 0;


// Documented in .h file
double ET_evaluate(ExprTree tree) {
    assert(tree); // Ensure the tree is not NULL

    // Switch based on the type of the expression node
    switch (tree->type) {
        case VALUE:
            return tree->n.value; // Return the value of the leaf node

        case UNARY_NEGATE:
            return -ET_evaluate(tree->n.child[0]); // Negate the value of the child

        case OP_ADD:
            return ET_evaluate(tree->n.child[0]) + ET_evaluate(tree->n.child[1]); // Addition

        case OP_SUB:
            return ET_evaluate(tree->n.child[0]) - ET_evaluate(tree->n.child[1]); // Subtraction

        case OP_MUL:
            return ET_evaluate(tree->n.child[0]) * ET_evaluate(tree->n.child[1]); // Multiplication

        case OP_DIV: {
            double rightValue = ET_evaluate(tree->n.child[1]); // Evaluate right child
            if (rightValue == 0.0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE); // Exit or handle error as appropriate
            }
            return ET_evaluate(tree->n.child[0]) / rightValue; // Division
        }

        case OP_POWER:
            return pow(ET_evaluate(tree->n.child[0]), ET_evaluate(tree->n.child[1])); // Power

        default:
            fprintf(stderr, "Error: Unknown expression node type\n");
            exit(EXIT_FAILURE); // Exit or handle unknown type
    }

    // Return 0.0 by default (this line should never be reached)
    return 0.0;
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


