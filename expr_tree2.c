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
    struct _expr_tree_node *child[2]; // For binary operators
    double value;                      // For value nodes
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
        case VALUE: return '0'; // Placeholder; values are printed as numbers
        default: assert(0); // Invalid type
    }
}

// Documented in .h file
ExprTree ET_value(double value) {
    ExprTree new = (ExprTree)malloc(sizeof(struct _expr_tree_node));
    assert(new);
    
    new->type = VALUE;
    new->n.value = value; // Set the value for the new node

    return new;
}

// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
    ExprTree new = (ExprTree)malloc(sizeof(struct _expr_tree_node));
    assert(new);

    new->type = op; // Set the operator type
    new->n.child[LEFT] = left; // Assign left child
    new->n.child[RIGHT] = right; // Assign right child

    return new;
}

// Documented in .h file
void ET_free(ExprTree tree) {
    if (tree == NULL)
        return;

    // Recursively free children nodes
    if (tree->type != VALUE) {
        ET_free(tree->n.child[LEFT]);
        ET_free(tree->n.child[RIGHT]);
    }

    free(tree); // Free the current node
}

// Documented in .h file
int ET_count(ExprTree tree) {
    if (tree == NULL)
        return 0;

    if (tree->type == VALUE)
        return 1;

    return 1 + ET_count(tree->n.child[LEFT]) + ET_count(tree->n.child[RIGHT]);
}

// Documented in .h file
int ET_depth(ExprTree tree) {
    if (tree == NULL)
        return 0;

    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

// Documented in .h file
double ET_evaluate(ExprTree tree) {
    assert(tree);

    switch (tree->type) {
        case VALUE:
            return tree->n.value; // Return the value directly
        case UNARY_NEGATE:
            return -ET_evaluate(tree->n.child[LEFT]); // Negate the left child
        case OP_ADD:
            return ET_evaluate(tree->n.child[LEFT]) + ET_evaluate(tree->n.child[RIGHT]);
        case OP_SUB:
            return ET_evaluate(tree->n.child[LEFT]) - ET_evaluate(tree->n.child[RIGHT]);
        case OP_MUL:
            return ET_evaluate(tree->n.child[LEFT]) * ET_evaluate(tree->n.child[RIGHT]);
        case OP_DIV:
            return ET_evaluate(tree->n.child[LEFT]) / ET_evaluate(tree->n.child[RIGHT]);
        case OP_POWER: {
            double base = ET_evaluate(tree->n.child[LEFT]);
            double exponent = ET_evaluate(tree->n.child[RIGHT]);
            return pow(base, exponent);
        }
        default:
            assert(0); // This should never happen
    }
}

// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz) {
    assert(tree);
    
    size_t pos = 0;
    int left_depth, right_depth;

    // Handle the depth of the left and right children
    if (tree->type != VALUE) {
        left_depth = ET_depth(tree->n.child[LEFT]);
        right_depth = ET_depth(tree->n.child[RIGHT]);

        // Add opening parenthesis for non-leaf nodes
        if (left_depth > 0 || right_depth > 0) {
            if (pos < buf_sz - 1) {
                buf[pos++] = '(';
            }
        }
    }

    // Recursively convert left child to string
    if (tree->n.child[LEFT] != NULL) {
        pos += ET_tree2string(tree->n.child[LEFT], buf + pos, buf_sz - pos);
    }

    // Add operator
    if (tree->type != VALUE) {
        if (pos < buf_sz - 1) {
            buf[pos++] = ExprNodeType_to_char(tree->type);
            buf[pos++] = ' ';
        }
    }

    // Recursively convert right child to string
    if (tree->n.child[RIGHT] != NULL) {
        pos += ET_tree2string(tree->n.child[RIGHT], buf + pos, buf_sz - pos);
    }

    // Add closing parenthesis for non-leaf nodes
    if (tree->type != VALUE && (left_depth > 0 || right_depth > 0)) {
        if (pos < buf_sz - 1) {
            buf[pos++] = ')';
        }
    }

    // Null-terminate the string
    if (pos < buf_sz) {
        buf[pos] = '\0';
    } else {
        buf[buf_sz - 1] = '\0'; // Ensure null termination if buffer is too small
    }

    return pos; // Return the length of the string created
}
