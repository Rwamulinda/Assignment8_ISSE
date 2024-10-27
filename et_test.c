/*
 * et_test.c
 * 
 * Test cases for ExprTree
 *
 * Author: <your name here>
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>  // strlen
#include <ctype.h>   // isblank
#include <math.h>    // fabs

#include "expr_tree.h"

// Checks that value is true; if not, prints a failure message and
// returns 0 from this function
#define test_assert(value) {                                            \
    if (!(value)) {                                                     \
      printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value);      \
      goto test_error;                                                  \
    }                                                                   \
  }

/*
 * Tests the ET_value, ET_node, and ET_free functions.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_node_free() {
    ExprTree tree = NULL;

    tree = ET_value(23400000);
    test_assert( ET_depth(tree) == 1 );
    ET_free(tree);

    tree = ET_value(-1000);
    test_assert( ET_depth(tree) == 1 );
    ET_free(tree);

    tree = ET_node(OP_ADD, ET_value(1), ET_value(3));
    test_assert( ET_depth(tree) == 2 );
    ET_free(tree);

    return 1;

 test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_count function
 */
int test_count() {
    ExprTree tree = NULL;
    int result = 1;

    // Test single node
    tree = ET_value(5.0);
    test_assert(ET_count(tree) == 1);
    ET_free(tree);

    // Test binary operation
    tree = ET_node(OP_ADD, ET_value(1.0), ET_value(2.0));
    test_assert(ET_count(tree) == 3);  // root + 2 leaves
    ET_free(tree);

    // Test unary operation
    tree = ET_node(UNARY_NEGATE, ET_value(1.0), NULL);
    test_assert(ET_count(tree) == 2);  // root + 1 leaf
    ET_free(tree);

    // Test complex expression
    tree = ET_node(OP_MUL,
                   ET_node(OP_ADD, ET_value(1.0), ET_value(2.0)),
                   ET_node(OP_SUB, ET_value(3.0), ET_value(4.0)));
    test_assert(ET_count(tree) == 7);  // 3 operators + 4 leaves
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_depth function
 */
int test_depth() {
    ExprTree tree = NULL;

    // Test single node
    tree = ET_value(5.0);
    test_assert(ET_depth(tree) == 1);
    ET_free(tree);

    // Test binary operation
    tree = ET_node(OP_ADD, ET_value(1.0), ET_value(2.0));
    test_assert(ET_depth(tree) == 2);
    ET_free(tree);

    // Test unary operation
    tree = ET_node(UNARY_NEGATE, ET_value(1.0), NULL);
    test_assert(ET_depth(tree) == 2);
    ET_free(tree);

    // Test more complex depth
    tree = ET_node(OP_MUL,
                   ET_node(OP_ADD, ET_value(1.0), ET_value(2.0)),
                   ET_node(OP_SUB, ET_value(3.0), ET_value(4.0)));
    test_assert(ET_depth(tree) == 3);  // 2 + 1 + root
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_evaluate function
 */
int test_evaluate() {
    ExprTree tree = NULL;

    // Test value
    tree = ET_value(5.0);
    test_assert(fabs(ET_evaluate(tree) - 5.0) < 1e-6);
    ET_free(tree);

    // Test addition
    tree = ET_node(OP_ADD, ET_value(3.0), ET_value(4.0));
    test_assert(fabs(ET_evaluate(tree) - 7.0) < 1e-6);
    ET_free(tree);

    // Test subtraction
    tree = ET_node(OP_SUB, ET_value(10.0), ET_value(3.0));
    test_assert(fabs(ET_evaluate(tree) - 7.0) < 1e-6);
    ET_free(tree);

    // Test multiplication
    tree = ET_node(OP_MUL, ET_value(2.0), ET_value(3.0));
    test_assert(fabs(ET_evaluate(tree) - 6.0) < 1e-6);
    ET_free(tree);

    // Test division
    tree = ET_node(OP_DIV, ET_value(6.0), ET_value(2.0));
    test_assert(fabs(ET_evaluate(tree) - 3.0) < 1e-6);
    ET_free(tree);

    // Test unary negation
    tree = ET_node(UNARY_NEGATE, ET_value(5.0), NULL);
    test_assert(fabs(ET_evaluate(tree) + 5.0) < 1e-6);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_tree2string function
 */
int test_tree2string() {
    ExprTree tree = NULL;
    char buf[256];

    // Test single value
    tree = ET_value(5.0);
    test_assert(ET_tree2string(tree, buf, sizeof(buf)) == 4); // "5.0"
    test_assert(strcmp(buf, "5") == 0);
    ET_free(tree);

    // Test binary operation
    tree = ET_node(OP_ADD, ET_value(3.0), ET_value(4.0));
    test_assert(ET_tree2string(tree, buf, sizeof(buf)) == 9); // "(3 + 4)"
    test_assert(strcmp(buf, "(3 + 4)") == 0);
    ET_free(tree);

    // Test unary operation
    tree = ET_node(UNARY_NEGATE, ET_value(3.0), NULL);
    test_assert(ET_tree2string(tree, buf, sizeof(buf)) == 7); // "-3"
    test_assert(strcmp(buf, "-3") == 0);
    ET_free(tree);

    // Test complex expression
    tree = ET_node(OP_MUL,
                   ET_node(OP_ADD, ET_value(1.0), ET_value(2.0)),
                   ET_node(OP_SUB, ET_value(3.0), ET_value(4.0)));
    test_assert(ET_tree2string(tree, buf, sizeof(buf)) == 25); // "((1 + 2) * (3 - 4))"
    test_assert(strcmp(buf, "((1 + 2) * (3 - 4))") == 0);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Main function to run tests
 */
int main() {
    printf("Running tests...\n");

    test_assert(test_node_free());
    test_assert(test_count());
    test_assert(test_depth());
    test_assert(test_evaluate());
    test_assert(test_tree2string());

    printf("All tests passed!\n");
    return 0;

test_error:
    return 1;
}
