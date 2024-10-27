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

    // Testing for NULL nodes and freeing empty trees
    tree = NULL;
    ET_free(tree);  // Should handle gracefully

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
 * Tests the ET_evaluate function, including edge cases.
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

    // Testing division by zero
    tree = ET_node(OP_DIV, ET_value(10), ET_value(0));
    test_assert(ET_evaluate(tree) == 0); // Should print an error or handle gracefully
    ET_free(tree);

    // Testing unary negate
    tree = ET_node(UNARY_NEGATE, ET_value(5), NULL);
    test_assert(fabs(ET_evaluate(tree) + 5.0) < 1e-6);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_tree2string function, including buffer handling.
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

    // Test for buffer overflow handling
    char small_buffer[5];
    tree = ET_node(OP_ADD, ET_value(1), ET_value(2));
    ET_tree2string(tree, small_buffer, sizeof(small_buffer));
    test_assert(small_buffer[sizeof(small_buffer) - 1] == '\0');
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the ET_count function.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_count() {
    ExprTree tree = NULL;

    tree = ET_value(5);
    test_assert(ET_count(tree) == 1);
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_value(2), ET_value(3));
    test_assert(ET_count(tree) == 3);
    ET_free(tree);

    tree = ET_node(OP_ADD, ET_node(OP_SUB, ET_value(1), ET_value(2)), ET_value(3));
    test_assert(ET_count(tree) == 5);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests additional edge cases or scenarios.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_additional_cases() {
    ExprTree tree = NULL;

    // Example additional test case
    tree = ET_node(OP_SUB, ET_value(10), ET_value(5));
    test_assert(fabs(ET_evaluate(tree) - 5.0) < 1e-6);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests operations with negative and zero values.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_negative_and_zero_operations() {
    ExprTree tree = NULL;

    // Testing unary negation with negative value
    tree = ET_node(UNARY_NEGATE, ET_value(-5), NULL);
    test_assert(fabs(ET_evaluate(tree) + 5.0) < 1e-6);
    ET_free(tree);

    // Testing zero as an operand in multiplication
    tree = ET_node(OP_MUL, ET_value(0), ET_value(3));
    test_assert(fabs(ET_evaluate(tree) - 0.0) < 1e-6);
    ET_free(tree);

    // Testing addition of negative and positive
    tree = ET_node(OP_ADD, ET_value(-2), ET_value(5));
    test_assert(fabs(ET_evaluate(tree) - 3.0) < 1e-6);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

/*
 * Tests the depth of more complex expression trees.
 *
 * Returns: 1 if all tests pass, 0 otherwise
 */
int test_complex_tree_depth() {
    ExprTree tree = NULL;

    // Creating a more complex tree structure
    tree = ET_node(OP_ADD,
        ET_node(OP_SUB, ET_value(10), ET_value(5)),
        ET_node(OP_MUL, ET_value(2), ET_value(3))
    );

    test_assert(ET_depth(tree) == 3);
    ET_free(tree);

    // Another complex tree
    tree = ET_node(OP_DIV,
        ET_node(OP_ADD, ET_value(8), ET_value(4)),
        ET_node(OP_SUB, ET_value(10), ET_value(2))
    );

    test_assert(ET_depth(tree) == 3);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

int main() {
    int passed = 0;
    int num_tests = 0;

    // Test each function and display results
    if (test_node_free()) {
        printf("test_node_free passed\n");
        passed++;
    } else {
        printf("test_node_free failed\n");
    }
    num_tests++;

    if (test_depth()) {
        printf("test_depth passed\n");
        passed++;
    } else {
        printf("test_depth failed\n");
    }
    num_tests++;

    if (test_evaluate()) {
        printf("test_evaluate passed\n");
        passed++;
    } else {
        printf("test_evaluate failed\n");
    }
    num_tests++;

    if (test_tree_to_string()) {
        printf("test_tree_to_string passed\n");
        passed++;
    } else {
        printf("test_tree_to_string failed\n");
    }
    num_tests++;

    if (test_count()) {
        printf("test_count passed\n");
        passed++;
    } else {
        printf("test_count failed\n");
    }
    num_tests++;

    if (test_additional_cases()) {
        printf("test_additional_cases passed\n");
        passed++;
    } else {
        printf("test_additional_cases failed\n");
    }
    num_tests++;

    if (test_negative_and_zero_operations()) {
        printf("test_negative_and_zero_operations passed\n");
        passed++;
    } else {
        printf("test_negative_and_zero_operations failed\n");
    }
    num_tests++;

    if (test_complex_tree_depth()) {
        printf("test_complex_tree_depth passed\n");
        passed++;
    } else {
        printf("test_complex_tree_depth failed\n");
    }
    num_tests++;

    printf("%d of %d tests passed.\n", passed, num_tests);
    return 0;
}
