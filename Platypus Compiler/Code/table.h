/****************************************************
 * Filename: table.h
 * Transition Table and function declarations necessary
	for the scanner implementation
 * as required for CST8152 - Assignment #2.
 *	
 *  Authors: Frederic Desjardins
 *  Student#: 040941359
 *	Date: November 15th 2020

 */


/* Macro section */
#ifndef  TABLE_H_
#define  TABLE_H_ 

#ifndef BUFFER_H_
#include "buffer.h"
#endif

#ifndef TOKEN_H_
#include "token.h"
#endif

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

 /*  Source end-of-file (SEOF) sentinel symbol
  *    '\0' or one of 255,0xFF,EOF
  */
/* TODO_02: EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 255

/*  Special case tokens processed separately one by one
 *  in the token-driven part of the scanner
 *  '=' , ' ' , '(' , ')' , '{' , '}' , == , <> , '>' , '<' , ';',
 *  white space
 *  !!comment , ',' , ';' , '-' , '+' , '*' , '/', ## ,
 *  _AND_, _OR_, _NOT_ , SEOF,
 */
/* TODO_03: Error states and illegal state */
#define ES  11		/* Error state  with no retract */
#define ER  12		/* Error state  with retract */
#define IS  0		/* Illegal state */

#define MAX_INT_P 32767

 /* State transition table definition */
#define TABLE_COLUMNS 8

/* Transition table - type of states defined in separate table */
/* TODO_04: Define Transition table missing values */
int  transitionTable[][TABLE_COLUMNS] = {
	/*				[A-z](0),	0(1),	[1-9](2),	.(3),	$(4),	"(5),	SEOF(6),	other(7)	*/
	/*				L,			Z,		N,			P,		S,		Q,		E,			O			*/
	/* State 00 */  {1,			6,		4,			ES,		ES,		9,		ER,			ES},		
	/* State 01 */  {1,			1,		1,			2,		3,		2,		2,			2},	
	/* State 02 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}, // AVID		
	/* State 03 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}, // SVID		
	/* State 04 */  {ES,		4,		4,			7,		5,		ES,		5,			5},	
	/* State 05 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}, // IL	
	/* State 06 */  {ES,		6,		ES,			7,		8,		ES,		5,			5},	
	/* State 07 */  {8,			7,		7,			8,		8,		8,		8,			8},	
	/* State 08 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}, // FPL		
	/* State 09 */  {9,			9,		9,			9,		9,		10,		ER,			9},	
	/* State 10 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}, // SL		
	/* State 11 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}, // ES		
	/* State 12 */  {IS,		IS,		IS,			IS,		IS,		IS,		IS,			IS}	 // ER		
};

/* Accepting state table definition */
/* TODO_05: Define accepting states types */
#define ASWR	13	/* accepting state with retract */
#define ASNR	14	/* accepting state with no retract */
#define NOAS	15	/* not accepting state */

/* List of acceptable states */
/* TODO_06: Define list of acceptable states */
int stateType[] = {
	/* State 0 */ NOAS,
	/* State 1 */ NOAS,
	/* AVID */ ASWR,
	/* SVID */ ASNR,
	/* State 4 */ NOAS,
	/* IL */ ASWR,
	/* State 6 */ NOAS,
	/* State 7 */ NOAS,
	/* FPL */ ASWR,
	/* State 9 */ NOAS,
	/* SL */ ASNR,
	/* ES */ ASNR,
	/* ER */ ASWR
};

/* Accepting action function declarations */
/* TODO_07: Declare accepting states functions */

Token aStateFuncAVID(char* lexeme); //AVID
Token aStateFuncSVID(char* lexeme); //SVID
Token aStateFuncIL(char* lexeme); //IL
Token aStateFuncFPL(char* lexeme); //FPL
Token aStateFuncSL(char* lexeme); //SL
Token aStateFuncErr(char* lexeme); // Error


/* Defining a new type: pointer to function (of one char * argument)
   returning Token
*/
typedef Token(*PTR_AAF)(char* lexeme);

/* Accepting function (action) callback table (array) definition */
/* If you do not want to use the typedef, the equvalent declaration is:
 * Token (*finalStateTable[])(char lexeme[]) = {
 */
/* TODO_08: Define final state table */
PTR_AAF finalStateTable[] = {
	NULL,
	NULL,
	aStateFuncAVID,
	aStateFuncSVID,
	NULL,
	aStateFuncIL,
	NULL,
	NULL,
	aStateFuncFPL,
	NULL,
	aStateFuncSL,
	aStateFuncErr,
	aStateFuncErr
};

/* Keyword lookup table (_AND_, _OR_ and _NOT_ are not keywords) */
/* TODO_09: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* Keyword list */
/* TODO_10: Define the list of keywords */
const char* keywordTable[] = {
	"DO", 
	"ELSE", 
	"FALSE", 
	"IF", 
	"INPUT", 
	"OUTPUT", 
	"PROGRAM",
	"THEN", 
	"TRUE", 
	"WHILE"
};



#endif
