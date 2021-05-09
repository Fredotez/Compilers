/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2020
*************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2019
* Author: Svillen Ranev
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, 021, 022, or 023]
* Assignment: A3.
* Date: Sep 01 2020
* Professor: Paulo Sousa / Haider Miraj
* Purpose: This file is the main header for Parser (.h)
* Function list: (...).
*************************************************************/

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "token.h"
#include "buffer.h"

/* Global vars */
// TODO_01: lookahead
extern Token lookahead;
// TODO_02: syntaxErrorNumber
int syntaxErrorNumber;
extern Buffer* stringLiteralTable;
extern int line;
extern Token processToken();
extern char* keywordTable[];

/* Constants */
#define	NO_ATTR (-1)
// TODO_03: Create ALL constants for all keywords
// Example: #define KEYWORD1 CONSTANTVALUE1

#define DO 0
#define ELSE 1
#define FALSE 2
#define IF 3
#define INPUT 4
#define OUTPUT 5
#define PROGRAM 6
#define THEN 7 
#define TRUE 8
#define WHILE 9

//...

/* Function definitions */
void processParser(void);
void matchToken(int, int);
void syncErrorHandler(int);
void printError();

//Function definition for non terminals, taken from Platypus Language specifications.
void program(void);
void opt_statements(void);
void statements(void);
void statement(void);
void statements_prime(void);
void assignment_statement(void);
void assignment_expression(void);
void selection_statement(void);
void iteration_statement(void);
void pre_condition(void);
void input_statement(void);
void variable_list(void);
void variable_list_prime(void);
void variable_identifier(void);
void output_statement(void);
void output_statement_prime(void);
void arithmetic_expression(void);
void unary_arithmetic_expression(void);
void additive_arithmetic_expression(void);
void additive_arithmetic_expression_prime(void);
void multiplicative_arithmetic_expression(void);
void multiplicative_arithmetic_expression_prime(void);
void primary_arithmetic_expression(void);
void string_expression(void);
void string_expression_prime(void);
void primary_string_expression(void);
void conditional_expression(void);
void logical_OR_expression(void);
void logical_OR_expression_prime(void);
void logical_AND_expression(void);
void logical_AND_expression_prime(void);
void logical_NOT_expression(void);
void relational_expression(void);
void primary_a_relational_expression_prime(void);
void primary_a_relational_expression(void);
void primary_s_relational_expression(void);
void primary_s_relational_expression_prime(void);
//...
