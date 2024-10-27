# Assignment8_ISSE

Project Overview
This project implements a binary expression tree in C, representing arithmetic expressions using a tree structure. Each node is either an operator (e.g., +, -, *, /) or a value. The module supports operations like evaluating the expression, counting nodes, determining tree depth, converting the tree to a string, and freeing allocated memory.

Files Summary
expr_tree.h: Header file with function declarations and structure definitions.
expr_tree.c: Contains the logic for creating, evaluating, and managing the expression tree.
et_test.c: Test file with sample expressions to validate the tree's functionality.
Functions Overview
ET_value(double value):
Creates a node containing a numeric value.

ET_node(int operator, ExprTree left, ExprTree right):
Creates a node with an operator and two child nodes.

ET_evaluate(ExprTree tree):
Evaluates the entire expression represented by the tree.

ET_count(ExprTree tree):
Returns the total number of nodes.

ET_depth(ExprTree tree):
Calculates the depth (or height) of the tree.

ET_tree2string(ExprTree tree, char *buffer, size_t size):
Converts the tree into a readable string.

ET_free(ExprTree tree):
Frees all memory associated with the tr
