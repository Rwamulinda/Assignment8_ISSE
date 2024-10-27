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
#include <string.h>  // Include string.h for string operations

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

// Convert an ExprNodeType into a printable character
static char ExprNodeType_to_char(ExprNodeType ent) {
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
ExprTree ET_value(double value) {
    ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
    assert(new);
    new->type = VALUE;  // Set the node's type to VALUE
    new->n.value = value;  // Set the value
    return new;
}

// Documented in .h file
ExprTree ET_node(ExprNodeType op, ExprTree left, ExprTree right) {
    ExprTree new = (ExprTree) malloc(sizeof(struct _expr_tree_node));
    assert(new);
    new->type = op;  // Set the node's type to the provided operator type
    new->n.child[LEFT] = left;  // Set left child
    new->n.child[RIGHT] = right;  // Set right child
    return new;
}

// Documented in .h file
void ET_free(ExprTree tree) {
    if (tree == NULL) return;
    ET_free(tree->n.child[LEFT]);  // Free left child
    ET_free(tree->n.child[RIGHT]);  // Free right child
    free(tree);  // Free current node
}

// Documented in .h file
int ET_count(ExprTree tree) {
    if (tree == NULL) return 0;
    if (tree->type == VALUE) return 1;
    return 1 + ET_count(tree->n.child[LEFT]) + ET_count(tree->n.child[RIGHT]);
}

// Documented in .h file
int ET_depth(ExprTree tree) {
    assert(tree);
    if (tree->n.child[LEFT] == NULL && tree->n.child[RIGHT] == NULL) {
        return 1;  // Leaf node
    }
    int left_depth = ET_depth(tree->n.child[LEFT]);
    int right_depth = ET_depth(tree->n.child[RIGHT]);
    return 1 + (left_depth > right_depth ? left_depth : right_depth);
}

// Documented in .h file
double ET_evaluate(ExprTree tree) {
    assert(tree);
    if (tree->type == VALUE) {
        return tree->n.value;  // Return the value directly
    }
    // Recursively evaluate left and right children
    double left_value = ET_evaluate(tree->n.child[LEFT]);
    double right_value = ET_evaluate(tree->n.child[RIGHT]);

    // Apply the operator based on the type of the node
    switch (tree->type) {
        case OP_ADD:
            return left_value + right_value;  
        case OP_SUB:
            return left_value - right_value;  
        case OP_MUL:
            return left_value * right_value;  
        case OP_DIV:
            if (right_value == 0) {
                fprintf(stderr, "Error: Division by zero\n");
                exit(EXIT_FAILURE);
            }
            return left_value / right_value;  
        case OP_POWER:
            return pow(left_value, right_value);  
        default:
            fprintf(stderr, "Error: Unknown operator\n");
            exit(EXIT_FAILURE);
    }
}

// Documented in .h file
size_t ET_tree2string(ExprTree tree, char *buf, size_t buf_sz) {
    assert(tree);
    buf[0] = '\0';  // Reset buffer
    // Recursive function to build string
    void build_string(ExprTree node) {
        if (!node) return; 
        if (node->type == VALUE) {
            char value_buf[32];  
            snprintf(value_buf, sizeof(value_buf), "%g", node->n.value);  
            strncat(buf, value_buf, buf_sz - strlen(buf) - 1);  // Append value
        } else {
            strncat(buf, "(", buf_sz - strlen(buf) - 1); 
            build_string(node->n.child[LEFT]);  
            strncat(buf, " ", buf_sz - strlen(buf) - 1);
            strncat(buf, (char[]){ExprNodeType_to_char(node->type), '\0'}, buf_sz - strlen(buf) - 1);
            strncat(buf, " ", buf_sz - strlen(buf) - 1); 
            build_string(node->n.child[RIGHT]);  
            strncat(buf, ")", buf_sz - strlen(buf) - 1);
        }
    }
    build_string(tree);
    return strlen(buf);
}
