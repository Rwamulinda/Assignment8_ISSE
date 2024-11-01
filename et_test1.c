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
int test_node_free()
{
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

int test_count()
{
    ExprTree tree = ET_node(OP_ADD, ET_value(1), ET_node(OP_MUL, ET_value(2), ET_value(3)));
    test_assert(ET_count(tree) == 5); // 1 (root) + 1 (left) + 1 (right) + 1 (left of right) + 1 (right of right)
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
int test_depth()
{
    ExprTree tree = ET_node(OP_ADD, ET_node(OP_SUB, ET_value(1), ET_value(2)), ET_value(3));
    test_assert(ET_depth(tree) == 3); // Depth is 3
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
int test_evaluate()
{
    ExprTree tree = ET_node(OP_ADD, ET_value(2), ET_value(3));
    test_assert(fabs(ET_evaluate(tree) - 5.0) < 1e-9); // 2 + 3 = 5
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_value(2), ET_value(3));
    test_assert(fabs(ET_evaluate(tree) - 6.0) < 1e-9); // 2 * 3 = 6
    ET_free(tree);

    tree = ET_node(OP_SUB, ET_value(5), ET_value(2));
    test_assert(fabs(ET_evaluate(tree) - 3.0) < 1e-9); // 5 - 2 = 3
    ET_free(tree);

    tree = ET_node(OP_DIV, ET_value(6), ET_value(2));
    test_assert(fabs(ET_evaluate(tree) - 3.0) < 1e-9); // 6 / 2 = 3
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
int test_tree2string()
{
    char buffer[100];
    ExprTree tree = ET_node(OP_ADD, ET_value(2), ET_value(3));
    test_assert(ET_tree2string(tree, buffer, sizeof(buffer)) == 5); // "2 + 3" should fit
    test_assert(strcmp(buffer, "2 + 3") == 0);
    ET_free(tree);

    tree = ET_node(OP_MUL, ET_node(OP_ADD, ET_value(1), ET_value(2)), ET_value(3));
    test_assert(ET_tree2string(tree, buffer, sizeof(buffer)) == 9); // "(1 + 2) * 3" should fit
    test_assert(strcmp(buffer, "(1 + 2) * 3") == 0);
    ET_free(tree);

    return 1;

test_error:
    ET_free(tree);
    return 0;
}

int test_depth_single_node() {
    ExprTree tree = ET_value(10);
    test_assert(ET_depth(tree) == 1);
    ET_free(tree);
    return 1;

 test_error:
    ET_free(tree);
    return 0;
}

  //
  // TODO: Add your code here
  //

int main()
{
  int passed = 0;
  int num_tests = 0;

  num_tests++; passed += test_node_free(); 
  num_tests++; passed += test_count();
  num_tests++; passed += test_depth();
  num_tests++; passed += test_evaluate();
  num_tests++; passed += test_tree2string();
  num_tests++; passed += test_depth_single_node();


  //
  // TODO: Add your code here
  //

  printf("Passed %d/%d test cases\n", passed, num_tests);
  fflush(stdout);
  return 0;
}

