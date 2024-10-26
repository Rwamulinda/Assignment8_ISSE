/*
 * et_test.c
 * 
 * Test cases for ExprTree
 *
 * Author: Uwase Pauline
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>  // strlen
#include <ctype.h>   // isblank
#include <math.h>    // fabs

#include "expr_tree.h"

// Function declarations for all tests
int test_node_free();
int test_count();
int test_depth();
int test_evaluate();
int test_tree2string();

#define test_assert(value) {                                            \
    if (!(value)) {                                                     \
      printf("FAIL %s[%d]: %s\n", __FUNCTION__, __LINE__, #value);      \
      goto test_error;                                                  \
    }                                                                   \
  }

/*
 * Tests the ET_value, ET_node, and ET_free functions.
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_node_free() {
    ExprTree tree = ET_value(23400000);
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
    if (tree) ET_free(tree);  // Avoid double free
    return 0;
}

/*
 * Tests the ET_evaluate function.
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_evaluate() {
    ExprTree tree = ET_node(OP_ADD, ET_value(10), ET_value(5));
    test_assert(ET_evaluate(tree) == 15.0);  // 10 + 5
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_value(2), ET_node(OP_ADD, ET_value(3), ET_value(4)));
    test_assert(ET_evaluate(tree) == 14.0);  // 2 * (3 + 4)
    ET_free(tree);

    // Edge case: Division by zero
    tree = ET_node(OP_DIV, ET_value(1), ET_value(0));
    test_assert(isinf(ET_evaluate(tree)));  // Should return infinity or handle gracefully
    ET_free(tree);

    return 1;

test_error:
    if (tree) ET_free(tree);
    return 0;
}

/*
 * Tests the ET_tree2string function.
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_tree2string() {
    char buffer[256];
    ExprTree tree = ET_node(OP_ADD, ET_value(1.0), ET_value(2.0));
    size_t length = ET_tree2string(tree, buffer, sizeof(buffer));
    test_assert(strcmp(buffer, "1.00 + 2.00") == 0);
    test_assert(length == strlen(buffer));
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_node(OP_ADD, ET_value(2.0), ET_value(3.0)), ET_value(4.0));
    length = ET_tree2string(tree, buffer, sizeof(buffer));
    test_assert(strcmp(buffer, "(2.00 + 3.00) * 4.00") == 0);
    test_assert(length == strlen(buffer));
    ET_free(tree);

    return 1;

test_error:
    if (tree) ET_free(tree);
    return 0;
}

/*
 * Tests the ET_count function.
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_count() {
    ExprTree tree = ET_node(OP_ADD, ET_value(1), ET_value(2));
    test_assert(ET_count(tree) == 3);  // 2 values + 1 operator
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_value(2), ET_node(OP_ADD, ET_value(3), ET_value(4)));
    test_assert(ET_count(tree) == 5);  // 3 values + 2 operators
    ET_free(tree);

    return 1;

test_error:
    if (tree) ET_free(tree);
    return 0;
}

/*
 * Tests the ET_depth function.
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_depth() {
    ExprTree tree = ET_value(10);
    test_assert(ET_depth(tree) == 1);  // Depth of a single value
    ET_free(tree);

    tree = ET_node(OP_SUB, ET_value(1), ET_value(2));
    test_assert(ET_depth(tree) == 2);  // Depth of a simple operation
    ET_free(tree);

    tree = ET_node(OP_ADD, ET_node(OP_MUL, ET_value(2), ET_value(3)), ET_value(4));
    test_assert(ET_depth(tree) == 3);  // Depth with nested operations
    ET_free(tree);

    return 1;

test_error:
    if (tree) ET_free(tree);
    return 0;
}

/*
 * Main function to run all tests.
 */
int main() {
    int passed = 0;
    int num_tests = 0;

    num_tests++; passed += test_node_free();
    num_tests++; passed += test_count();
    num_tests++; passed += test_depth();
    num_tests++; passed += test_evaluate();
    num_tests++; passed += test_tree2string();

    printf("Passed %d/%d test cases\n", passed, num_tests);
    fflush(stdout);
    return (passed == num_tests) ? 0 : 1;
}
