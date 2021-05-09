/*******************************************************************
 * Filename: scanner.c
 * PURPOSE: 
 *    SCANNER.C: Functions implementing a Lexical Analyzer (Scanner)
 *    as required for CST8152, Assignment #2
 *    scanner_init() must be called before using the scanner.
 *    The file is incomplete;
 * 
 *    Created by: Frederic Desjardins
 *    Student#: 040941359
 *    Date: November 15th 2020
 * 
 */

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */

  /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern pBuffer stringLiteralTable;		/* String literal table */
int line;								/* current line number of the source code */
extern int errorNumber;					/* defined in platy_st.c - run-time error number */

static char debugMode = 0;				/* optional for debugging */

/* Local(file) global objects - variables */
static pBuffer lexemeBuffer;			/* pointer to temporary lexeme buffer */
static pBuffer sourceBuffer;			/* pointer to input source buffer */
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int nextTokenClass(char c);		/* character class function */
static int getNextState(int, char);		/* state machine function */
static int isKeyword(char* kw_lexeme);	/* keywords lookup functuion */



/* TODO_02: Follow the standard and adjust all function headers */

/*************************************************************
 * Intitializes scanner
 ************************************************************/
int initScanner(pBuffer psc_buf) {
	if (bufferIsEmpty(psc_buf)) return EXIT_FAILURE;/*1*/
	/* in case the buffer has been read previously  */
	bufferRewind(psc_buf);
	bufferClear(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS;/*0*/
/*   scerrnum = 0;  *//*no need - global ANSI C */
}



/*************************************************************
 * Process Token
 ************************************************************/
Token processToken(void) {
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c;	/* input symbol */
	int state = 0;		/* initial state of the FSM */
	short lexStart = 0;		/* start offset of a lexeme in the input char buffer (array) */
	short lexEnd = 0;		/* end offset of a lexeme in the input char buffer (array)*/

	short lexLength = 0;		/* token length */
	int i = 0;				/* counter */
	int isError = 0;		/* error identification */
	unsigned char newc;	/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */

		c = bufferGetChar(sourceBuffer);


		/* Part 1: Implementation of token driven scanner */
		/* every token is possessed by its own dedicated code */
		/* TODO_03: Token driven scanner implementation using switch */

		/* Switch case to handle special characters */
		switch (c) {
		
		case '\n':
			line++;
			break;
		case ' ':			
			break;
		case '\t':
			break;
		case '#':
			break;
		case '&':
			break;
		case CHARSEOF0:
			currentToken.attribute.seof = CHARSEOF0;
			return currentToken;
		case CHARSEOF255:
			currentToken.attribute.seof = CHARSEOF255;
			return currentToken;
		case '!':

			// set first char after ! to newc
			newc = bufferGetChar(sourceBuffer);

			if (newc == '!') {
				while (c != '\n')
				{
					c = bufferGetChar(sourceBuffer);
				}
				line++;
				break;
			}
			else {
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = '!';
				currentToken.attribute.err_lex[1] = newc;
				while (c != '\n')
				{
					c = bufferGetChar(sourceBuffer);
				}
				line++;
				return currentToken;
			}

		/* Parentheses */
		case '(': 
			currentToken.code = LPR_T; 			
			return currentToken;
		case ')': 
			currentToken.code = RPR_T; 
			return currentToken;
		case '{': 
			currentToken.code = LBR_T; 
			return currentToken;
		case '}': 
			currentToken.code = RBR_T; 
			return currentToken;
		/* Operations */
		case '-': 
			currentToken.code = ART_OP_T; 
			currentToken.attribute.arr_op = SUB; 
			return currentToken;
		case '+': 			
			currentToken.code = ART_OP_T; 
			currentToken.attribute.arr_op = ADD; 
			return currentToken;
		case '*': 
			currentToken.code = ART_OP_T; 
			currentToken.attribute.arr_op = MUL; 
			return currentToken;
		case '/': 
			currentToken.code = ART_OP_T; 
			currentToken.attribute.arr_op = DIV; 
			return currentToken;
		/* Relationship Opertors */
		case '>': 
			currentToken.code = REL_OP_T; 
			currentToken.attribute.rel_op = GT;
			return currentToken;
		case '<': 
			
			currentToken.code = REL_OP_T;

			//Check is <> rel op is not eq
			if (bufferGetChar(sourceBuffer) == '>')
			{
				currentToken.attribute.rel_op = NE;
				
			}
			else {
				bufferRetract(sourceBuffer);
				currentToken.attribute.rel_op = LT;
			}
			return currentToken;

		case '=':	
			// if next is another = it is a relationship op, else assignment;
			if (bufferGetChar(sourceBuffer) == '=')
			{
				currentToken.code = REL_OP_T;
				currentToken.attribute.rel_op = EQ;
				
			}
			else {				
				currentToken.code = ASS_OP_T;	
				bufferRetract(sourceBuffer);
			}
			return currentToken;

		case ';': 
			currentToken.code = EOS_T; 
			return currentToken;
		case ',': 
			currentToken.code = COM_T; 
			return currentToken;
		case '$': 

			// set first char after ! to newc
			newc = bufferGetChar(sourceBuffer);

			if (newc == '$') {
				currentToken.code = SCC_OP_T;
			} 
			else if (newc != '$')
			{
				currentToken.code = ERR_T;
				currentToken.attribute.err_lex[0] = c;
			}
			 
			return currentToken;

		case '_':

			bufferSetMarkPosition(sourceBuffer, bufferGetCPosition(sourceBuffer));
			newc = bufferGetChar(sourceBuffer);
			
			
			if (newc == 'A') {
				if (bufferGetChar(sourceBuffer) == 'N') {
				
					if (bufferGetChar(sourceBuffer) == 'D') {
						
						if (bufferGetChar(sourceBuffer) == '_') {
							
							currentToken.code = LOG_OP_T;
							currentToken.attribute.log_op = AND;
							return currentToken;
						}
					}
				}
			}
			else if (newc == 'O' && bufferGetChar(sourceBuffer) == 'R' && bufferGetChar(sourceBuffer) == '_') {

				currentToken.code = LOG_OP_T;
				currentToken.attribute.log_op = OR;
				return currentToken;
			}
			else if (newc == 'N' && bufferGetChar(sourceBuffer) == 'O' && bufferGetChar(sourceBuffer) == 'T' && bufferGetChar(sourceBuffer) == '_') {

				currentToken.code = LOG_OP_T;
				currentToken.attribute.log_op = NOT;
				return currentToken;
			}
			
			currentToken.code = ERR_T;
			currentToken.attribute.err_lex[0] = '_';
			currentToken.attribute.err_lex[1] = '\0';
			bufferReset(sourceBuffer);
			return currentToken;

	
		
			/* Part 2: Implementation of Finite State Machine (DFA)
					   or Transition Table driven Scanner
					   Note: Part 2 must follow Part 1 to catch the illegal symbols
			*/
			/* TODO_04: Transition driven scanner implementation inside default */

		default: // general case
			/* ... */

			state = getNextState(state, c);


			int currentStateType = stateType[state];

			
			lexStart = bufferSetMarkPosition(sourceBuffer, bufferGetCPosition(sourceBuffer) - 1);


			while (currentStateType == NOAS)
			{
				newc = bufferGetChar(sourceBuffer);				
				state = getNextState(state, newc);
				currentStateType = stateType[state];
				
			}
			
			if (currentStateType == ASWR) {				
				bufferRetract(sourceBuffer);
			}

			lexEnd = bufferGetCPosition(sourceBuffer);

			lexLength = lexEnd - lexStart;

			

			if (lexemeBuffer = bufferCreate(lexLength, 0, 'f'))
			{
			
				bufferReset(sourceBuffer);
			
				for (i = lexStart; i < lexEnd; i++)
				{
					c = bufferGetChar(sourceBuffer);
					bufferAddChar(lexemeBuffer, c);
				}
				bufferSetEnd(lexemeBuffer, '\0');


				currentToken = finalStateTable[state](bufferGetString(lexemeBuffer, 0));
			
				bufferFree(lexemeBuffer);

				return currentToken;
			}
			else {
				currentToken.code = ERR_T;
				errorNumber = 1;
				strcpy(currentToken.attribute.err_lex, "RUN TIME ERROR: ");
				return currentToken;
			}

			currentToken.code = ERR_T;
			currentToken.attribute.err_lex[0] = c;
			currentToken.attribute.err_lex[1] = CHARSEOF0;

			break;
			
		} // switch

	} //while

} //processToken


/* DO NOT MODIFY THE CODE OF THIS FUNCTION
YOU CAN REMOVE THE COMMENTS ONLY */
/*************************************************************
 * Get Next State
 ************************************************************/

int getNextState(int state, char c) {
	int col;
	int next;
	if (debugMode)
		fprintf(stderr, "[03] getNextState\n");
	col = nextTokenClass(c);

	next = transitionTable[state][col];
	
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
	/*
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:

	Assertion failed: test, file filename, line linenum

	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	*/
	assert(next != IS);

	/*
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUF is used - see the top of the file.
	*/
#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	
	return next;
}

/*************************************************************
 * Get Next Token Class
 ************************************************************/


int nextTokenClass(char c) {
	int val = -1;
	if (debugMode)
		fprintf(stderr, "[04] NextTokenClass\n");

	/*
	THIS FUNCTION RETURNS THE COLUMN NUMBER IN THE TRANSITION
	TABLE st_table FOR THE INPUT CHARACTER c.
	SOME COLUMNS MAY REPRESENT A CHARACTER CLASS .
	FOR EXAMPLE IF COLUMN 2 REPRESENTS [A-Za-z]
	THE FUNCTION RETURNS 2 EVERY TIME c IS ONE
	OF THE LETTERS A,B,...,Z,a,b...z.
	PAY ATTENTION THAT THE FIRST COLOMN IN THE TT IS 0 (has index 0)
	*/

	/* TODO_05: the logic to return the next column in TT */
	/*				[A-z](0),	0(1),	[1-9](2),	.(3),	$(4),	"(5),	SEOF(6),	other(7) */

	if (isalpha(c)) {
		c = 'L';
	}
	else if (isdigit(c) && c != '0') {
		c = 'D';
	}

	/* Switch case to handle input */
	switch (c) {
	case 'L' :
		val = 0;
		break;
	case '0' :	
		val = 1;
		break;
	case 'D':		
		val = 2;
		break;
	case '.':
		val = 3;
		break;
	case '$':
		val = 4;
		break;
	case '"':
		val = 5;
		break;
	case CHARSEOF0:
	case CHARSEOF255:
		val = 6;
		break;
	default:
		val = 7;
		break;

	} //switch

	return val;
}


/*************************************************************
 * Acceptance State Function AVID
 ************************************************************/
 /* TODO_06: Implement the method to recognize AVID */
 /*
HERE YOU WRITE THE DEFINITIONS FOR YOUR ACCEPTING FUNCTIONS.
************************************************************
ACCEPTING FUNCTION FOR THE arithmentic variable identifier AND keywords (VID - AVID/KW)
REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER
*/
Token aStateFuncAVID(char lexeme[]) {

	Token currentToken = { 0 };

	//printf("In AVID");

	int keyWordIndex = isKeyword(lexeme);

	if (keyWordIndex != -1)
	{		
		currentToken.code = KW_T;	
		currentToken.attribute.kwt_idx = keyWordIndex;	
		return currentToken;
	}
	else
	{
		currentToken.code = AVID_T;
	}

	int lexemeLength = strlen(lexeme);
	int i = 0;

	if (lexemeLength > VID_LEN) {

		for (i = 0; i < VID_LEN; i++) {
			currentToken.attribute.vid_lex[i] = lexeme[i];

		}

		currentToken.attribute.vid_lex[VID_LEN] = CHARSEOF0;
		
	}
	else {
		strcpy(currentToken.attribute.vid_lex, lexeme);
	}


	/* ... */
	return currentToken;
}

/*************************************************************
 * Acceptance State Function SVID
 ************************************************************/
/* TODO_07: Implement the method to recognize SVID */
/*
ACCEPTING FUNCTION FOR THE string variable identifier (VID - SVID)
REPLACE XX WITH THE CORRESPONDING ACCEPTING STATE NUMBER
*/
Token aStateFuncSVID(char lexeme[]) {

	
	Token currentToken = { 0 };

	currentToken.code = SVID_T;

	int lexemeLength = strlen(lexeme);
	int i = 0;

	if (lexemeLength > VID_LEN) {

		lexemeLength = VID_LEN;

	}
	for (i = 0; i < lexemeLength - 1; i++) {
		currentToken.attribute.vid_lex[i] = lexeme[i];

	}

	currentToken.attribute.vid_lex[lexemeLength - 1] = '$';
	currentToken.attribute.vid_lex[lexemeLength] = CHARSEOF0;

	/* ... */
	return currentToken;
}

/*************************************************************
 * Acceptance State Function IL
 ************************************************************/
/* TODO_08: Implement the method to recognize IL */
/*
ACCEPTING FUNCTION FOR THE integer literal(IL) - decimal constant (DIL)
*/
Token aStateFuncIL(char lexeme[]) {
	Token currentToken = { 0 };


	int lexemeLength = strlen(lexeme);
	int lexemeValue = atoi(lexeme);

	/* (int)lexeme is out of range */
	if (lexemeValue <= SHRT_MAX && lexemeValue >= SHRT_MIN)
	{
		
		currentToken.code = INL_T;
		currentToken.attribute.int_value = lexemeValue;
		
	}
	else {

		currentToken.code = ERR_T;
		
		if (lexemeLength > ERR_LEN) {
			int i;
			for (i = 0; i < ERR_LEN - 2; i++) {
				currentToken.attribute.err_lex[i] = lexeme[i];
			}
			currentToken.attribute.err_lex[ERR_LEN - 2] = '.';
			currentToken.attribute.err_lex[ERR_LEN - 1] = '.';
			currentToken.attribute.err_lex[ERR_LEN] = '.';
		} else {

				strcpy(currentToken.attribute.err_lex, lexeme);
			}
	}

	/* ... */
	return currentToken;
}

/*************************************************************
 * Acceptance State Function FPL
 ************************************************************/
/* TODO_09: Implement the method to recognize FPL */
/*
ACCEPTING FUNCTION FOR THE floating-point literal (FPL)
*/
Token aStateFuncFPL(char lexeme[]) {
	Token currentToken = { 0 };


	float lexemeValue = (float) atof(lexeme);
	int lexemeLength = strlen(lexeme);

	if (lexemeValue < INT_MAX && lexemeValue > INT_MIN)
	{

		currentToken.code = FPL_T;
		currentToken.attribute.flt_value = lexemeValue;

	}
	else {
		currentToken.code = ERR_T;

		if (lexemeLength > ERR_LEN) {
			int i;
			for (i = 0; i < ERR_LEN - 2; i++) {
				currentToken.attribute.err_lex[i] = lexeme[i];
			}
			currentToken.attribute.err_lex[ERR_LEN - 2] = '.';
			currentToken.attribute.err_lex[ERR_LEN - 1] = '.';
			currentToken.attribute.err_lex[ERR_LEN] = '.';
		}
		else {

			strcpy(currentToken.attribute.err_lex, lexeme);
		}

	}


	/* ... */
	return currentToken;
}

/*************************************************************
 * Acceptance State Function SL
 ************************************************************/
/* TODO_10: Implement the method to recognize SL */
/*
ACCEPTING FUNCTION FOR THE string literal (SL)
*/
Token aStateFuncSL(char lexeme[]) {
	Token currentToken = { 0 };

	int i;
	int lexemeLength = strlen(lexeme);

	currentToken.code = STR_T;

	currentToken.attribute.str_offset = bufferGetAddCPosition(stringLiteralTable);

	for (i = 0; i < lexemeLength; i++) {

		if (lexeme[i] != '"') {
			bufferAddChar(stringLiteralTable, lexeme[i]);
		}
		else if (lexeme[i] == '\n') {
			line++;
		}

	}

	bufferAddChar(stringLiteralTable, CHARSEOF0);



	/* ... */
	return currentToken;
}

/*************************************************************
 * Acceptance State Function Error
 ************************************************************/
/* TODO_11: Implement the method to deal with Error Token */
/*
ACCEPTING FUNCTION FOR THE ERROR TOKEN
*/
Token aStateFuncErr(char lexeme[]) {
	Token currentToken = { 0 };

	currentToken.code = ERR_T;
	int lexemeLength = strlen(lexeme);
	int i;

	if (lexemeLength > ERR_LEN) {
		
		for (i = 0; i < ERR_LEN - 2; i++) {

			if (lexeme[i] == '\n') {
				line++;
			}
			else {
				currentToken.attribute.err_lex[i] = lexeme[i];
			}
		}

			currentToken.attribute.err_lex[ERR_LEN - 2] = '.';
			currentToken.attribute.err_lex[ERR_LEN - 1] = '.';
			currentToken.attribute.err_lex[ERR_LEN] = '.';

	}
	else {
		for (i = 0; i < ERR_LEN; i++) {

			if (lexeme[i] == '\n') {
				line++;
			}
			else {
				currentToken.attribute.err_lex[i] = lexeme[i];
			}
		}
	}

	
	/* ... */
	return currentToken;
}

/*************************************************************
 * Check if it is Keyword
 ************************************************************/
/* TODO_11: Checks if a string is a keyword, returning the position in the list */

int isKeyword(char* kw_lexeme) {

	int i; 
	
	for (i = 0; i < KWT_SIZE; i++)
	{
		if (strcmp(keywordTable[i], kw_lexeme) == 0) {
			return i;
		}
	}
	return -1;
}

/*
HERE YOU WRITE YOUR ADDITIONAL FUNCTIONS (IF ANY).
FOR EXAMPLE
*/
