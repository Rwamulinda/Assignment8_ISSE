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
        default: return '?';  // For VALUE or unknown types
    }
}

// Documented in .h file
ExprTree ET_value(double value) {
    ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
    assert(new);

    new->type = VALUE;
    new->n.value = value;

    return new;
}

// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
    ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
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
int ET_count(ExprTree tree) {
    if (tree == NULL) return 0;
    if (tree->type == VALUE) return 1;
    
    return 1 + ET_count(tree->n.child[LEFT]) + ET_count(tree->n.child[RIGHT]);
}

// Documented in .h file
int ET_depth(ExprTree tree) {
    if (tree == NULL) return 0;
    
    if (tree->type == VALUE) return 1; // Base case for leaf node
    
    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);
    
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

// Documented in .h file
double ET_evaluate(ExprTree tree) {
    if (tree == NULL) return 0.0;
    
    switch (tree->type) {
        case VALUE:
            return tree->n.value;

        case OP_ADD:
            return ET_evaluate(tree->n.child[LEFT]) + ET_evaluate(tree->n.child[RIGHT]);

        case OP_SUB:
            return ET_evaluate(tree->n.child[LEFT]) - ET_evaluate(tree->n.child[RIGHT]);

        case OP_MUL:
            return ET_evaluate(tree->n.child[LEFT]) * ET_evaluate(tree->n.child[RIGHT]);

        case OP_DIV:
            return ET_evaluate(tree->n.child[LEFT]) / ET_evaluate(tree->n.child[RIGHT]);

        case UNARY_NEGATE:
            return -ET_evaluate(tree->n.child[LEFT]);

        default:
            return 0.0; // Shouldn't reach here
    }
}

// Documented in .h file
int ET_tree2string(ExprTree tree, char *buf, size_t bufsize) {
    if (tree == NULL) {
        snprintf(buf, bufsize, "");
        return 0;
    }

    char left[256], right[256];
    int left_size = ET_tree2string(tree->n.child[LEFT], left, sizeof(left));
    int right_size = ET_tree2string(tree->n.child[RIGHT], right, sizeof(right));

    switch (tree->type) {
        case VALUE:
            snprintf(buf, bufsize, "%f", tree->n.value);
            return strlen(buf);
        case OP_ADD:
        case OP_SUB:
        case OP_MUL:
        case OP_DIV:
            snprintf(buf, bufsize, "(%s %c %s)", left, ExprNodeType_to_char(tree->type), right);
            return strlen(buf);
        case UNARY_NEGATE:
            snprintf(buf, bufsize, "-%s", left);
            return strlen(buf);
        default:
            return 0; // Unsupported node type
    }
}
