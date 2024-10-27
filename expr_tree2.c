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
    if (tree == NULL)
        return;

    if (tree->type != VALUE) {
        ET_free(tree->n.child[LEFT]);
        if (tree->type != UNARY_NEGATE) {  // Only free right child if not unary
            ET_free(tree->n.child[RIGHT]);
        }
    }

    free(tree);
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
        return 0; // Handle NULL case

    if (tree->type == VALUE)
        return 1;

    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = (tree->type == UNARY_NEGATE) ? 0 : ET_depth(tree->n.child[RIGHT]);

    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

// Documented in .h file
double ET_evaluate(ExprTree tree) {
    assert(tree);

    if (tree->type == VALUE)
        return tree->n.value;

    double left_val = ET_evaluate(tree->n.child[LEFT]);

    switch (tree->type) {
        case UNARY_NEGATE:
            return -left_val;
        case OP_ADD:
            return left_val + ET_evaluate(tree->n.child[RIGHT]);
        case OP_SUB:
            return left_val - ET_evaluate(tree->n.child[RIGHT]);
        case OP_MUL:
            return left_val * ET_evaluate(tree->n.child[RIGHT]);
        case OP_DIV:
            return left_val / ET_evaluate(tree->n.child[RIGHT]);
        case OP_POWER:
            return pow(left_val, ET_evaluate(tree->n.child[RIGHT]));
        default:
            assert(0); // Should never get here
            return 0.0;
    }
}

// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz) {
    assert(tree);
    assert(buf);
    assert(buf_sz > 0);

    if (buf_sz == 1) {
        buf[0] = '\0';
        return 0;
    }

    size_t chars_written = 0;

    if (tree->type == VALUE) {
        chars_written = snprintf(buf, buf_sz, "%g", tree->n.value);
        if (chars_written >= buf_sz) {
            buf[buf_sz - 2] = '$';
            buf[buf_sz - 1] = '\0';
            return buf_sz - 1;
        }
        return chars_written;
    }

    // For non-leaf nodes, we need parentheses except for UNARY_NEGATE
    if (tree->type != UNARY_NEGATE) {
        if (buf_sz > 1) {
            buf[chars_written++] = '(';
            buf[chars_written] = '\0';
        }
    }

    // Handle left child
    size_t remaining = buf_sz - chars_written;
    if (remaining > 0) {
        size_t left_chars = ET_tree2string(tree->n.child[LEFT],
                                            buf + chars_written,
                                            remaining);
        chars_written += left_chars;
    }

    // Handle operator
    if (chars_written < buf_sz - 1) {
        if (tree->type != UNARY_NEGATE) {
            buf[chars_written++] = ' ';
            buf[chars_written++] = ExprNodeType_to_char(tree->type);
            buf[chars_written++] = ' ';
        }
        buf[chars_written] = '\0';
    }

    // Handle right child if not UNARY_NEGATE
    if (tree->type != UNARY_NEGATE) {
        remaining = buf_sz - chars_written;
        if (remaining > 0) {
            size_t right_chars = ET_tree2string(tree->n.child[RIGHT],
                                                 buf + chars_written,
                                                 remaining);
            chars_written += right_chars;
        }

        // Close parenthesis
        if (chars_written < buf_sz - 1) {
            buf[chars_written++] = ')';
            buf[chars_written] = '\0';
        }
    }

    // Check if we need to truncate
    if (chars_written >= buf_sz - 1) {
        buf[buf_sz - 2] = '$';
        buf[buf_sz - 1] = '\0';
        return buf_sz - 1;
    }

    return chars_written;
}
