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
    test_assert(ET_depth(tree) == 1);
    ET_free(tree);

    tree = ET_value(-1000);
    test_assert(ET_depth(tree) == 1);
    ET_free(tree);

    tree = ET_node(OP_ADD, ET_value(1), ET_value(3));
    test_assert(ET_depth(tree) == 2);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_depth function.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_depth() {
    ExprTree tree = NULL;

    tree = ET_value(5);
    test_assert(ET_depth(tree) == 1);
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_value(2), ET_value(3));
    test_assert(ET_depth(tree) == 2);
    ET_free(tree);

    tree = ET_node(OP_ADD, ET_node(OP_SUB, ET_value(1), ET_value(2)), ET_value(3));
    test_assert(ET_depth(tree) == 3);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_evaluate function.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_evaluate() {
    ExprTree tree = NULL;

    tree = ET_value(5);
    test_assert(fabs(ET_evaluate(tree) - 5.0) < 1e-6);
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_value(2), ET_value(3));
    test_assert(fabs(ET_evaluate(tree) - 6.0) < 1e-6);
    ET_free(tree);

    tree = ET_node(OP_ADD, ET_node(OP_SUB, ET_value(1), ET_value(2)), ET_value(3));
    test_assert(fabs(ET_evaluate(tree) - 2.0) < 1e-6);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_tree2string function.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_tree_to_string() {
    char buffer[256];

    ExprTree tree = ET_node(OP_ADD, ET_value(2), ET_value(3));
    ET_tree2string(tree, buffer, sizeof(buffer));
    test_assert(strcmp(buffer, "+ 2.00 3.00") == 0);
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_node(OP_ADD, ET_value(1), ET_value(2)), ET_value(3));
    ET_tree2string(tree, buffer, sizeof(buffer));
    test_assert(strcmp(buffer, "* + 1.00 2.00 3.00") == 0);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

int main() {
    int passed = 0;
    int num_tests = 0;

    num_tests++; passed += test_node_free();
    num_tests++; passed += test_depth();
    num_tests++; passed += test_evaluate();
    num_tests++; passed += test_tree_to_string();

    printf("Passed %d/%d test cases\n", passed, num_tests);
    fflush(stdout);
    return 0;
}
