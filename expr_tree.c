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
    case UNARY_NEGATE: return '-';
    default: return '?';  // Unknown type
  }

}


// Documented in .h file
ExprTree ET_value(double value)
{
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);


  //
  // TODO: Add your code here
  // Set the node's type to VALUE
    switch (tree->type) {
        case VALUE:
            return tree->n.value; 
        case UNARY_NEGATE:
            return -ET_evaluate(tree->n.child[LEFT]); 
        case OP_ADD:
            return ET_evaluate(tree->n.child[LEFT]) + ET_evaluate(tree->n.child[RIGHT]);
        case OP_SUB:
            return ET_evaluate(tree->n.child[LEFT]) - ET_evaluate(tree->n.child[RIGHT]);
        case OP_MUL:
            return ET_evaluate(tree->n.child[LEFT]) * ET_evaluate(tree->n.child[RIGHT]);
        case OP_DIV:
            if (ET_evaluate(tree->n.child[RIGHT]) == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(1);
            }
            return ET_evaluate(tree->n.child[LEFT]) / ET_evaluate(tree->n.child[RIGHT]);
        case OP_POWER: {
            double base = ET_evaluate(tree->n.child[LEFT]);
            double exponent = ET_evaluate(tree->n.child[RIGHT]);
            return pow(base, exponent);
        }
        default:
            assert(0);
    }
  return new;
}


// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right)
{
  ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
  assert(new);


  //
  // TODO: Add your code here
  // Set the node's type to the provided operator type
    new->type = op;

    // Since this is an operator node, we don't set a value. We set child nodes instead.
    new->left = left;
    new->right = right;

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
  // Free the left and right subtrees first
    ET_free(tree->left);
    ET_free(tree->right);

    // Free the current node
    free(tree);
  //

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
   // Base case: if the node is a leaf (both children are NULL), depth is 1
    if (tree->left == NULL && tree->right == NULL) {
        return 1;
    }

    // Calculate the depth of the left and right subtrees
    int left_depth = (tree->left != NULL) ? ET_depth(tree->left) : 0;
    int right_depth = (tree->right != NULL) ? ET_depth(tree->right) : 0;

    // Return the maximum of the two depths, plus one for the current node
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
  //
}


// Documented in .h file
double ET_evaluate(ExprTree tree)
{
  assert(tree);


  //
  // TODO: Add your code here
  // If the node is a leaf node (value)
    if (tree->type == ET_VALUE) {
        return tree->value;  // Return the value directly
    }

    // Recursively evaluate the left and right subtrees
    double left_value = ET_evaluate(tree->left);
    double right_value = ET_evaluate(tree->right);

    // Apply the operator based on the type of the node
    switch (tree->type) {
        case ET_ADD:
            return left_value + right_value;  // Addition
        case ET_SUBTRACT:
            return left_value - right_value;  // Subtraction
        case ET_MULTIPLY:
            return left_value * right_value;  // Multiplication
        case ET_DIVIDE:
            // Handle division by zero
            if (right_value == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            return left_value / right_value;  // Division
        // Add more cases for other operators as needed
        default:
            fprintf(stderr, "Error: Unknown operator\n");
            exit(EXIT_FAILURE);
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
  // Reset the buffer in case of previous content
    buf[0] = '\0';  // Ensure the buffer is empty

    // Helper function to append strings safely
    size_t append_to_buffer(const char *str) {
        size_t len = strlen(str);
        if (len < buf_sz) {
            strncat(buf, str, buf_sz - strlen(buf) - 1); // Append with safety
        }
        return len;
    }

    // Recursive function to traverse the tree and build the string
    void build_string(ExprTree node) {
        if (!node) return; // Base case: if node is NULL, return

        // If the node is a value, append its value to the buffer
        if (node->type == ET_VALUE) {
            char value_buf[32];  // Buffer for the number
            snprintf(value_buf, sizeof(value_buf), "%g", node->value);  // Format value as a string
            append_to_buffer(value_buf);  // Append value to buffer
        } else {
            // It's an operator; start with '('
            append_to_buffer("(");
            // Process left child
            build_string(node->left);
            // Append operator
            append_to_buffer(" ");
            append_to_buffer(ExprNodeType_to_char(node->type)); // Convert operator to char
            append_to_buffer(" ");
            // Process right child
            build_string(node->right);
            // End with ')'
            append_to_buffer(")");
        }
    }

    // Start building the string from the root of the tree
    build_string(tree);

    return strlen(buf);
  //
}


