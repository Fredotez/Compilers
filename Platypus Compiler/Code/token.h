/*************************************************************
 * Filename: token.h
 * Token declarations necessary for the scanner implementation
 * CST8152, Assignment #2
 *
 *    Created by: Frederic Desjardins
 *    Student#: 040941359
 *    Date: November 15th 2020
*************************************************************/




#ifndef TOKEN_H_
#define TOKEN_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 8   /* variable identifier length */
#define ERR_LEN 20  /* error message length */
#define INL_LEN 5   /* maximum number of digits for IL */

/* Token codes: Numbers from -1 to 18 (old: 0 to 19) */
#define ERR_T     -1 /* Error token */
#define SEOF_T    0  /* Source end-of-file token */
#define AVID_T    1  /* Arithmetic Variable identifier token */
#define SVID_T    2  /* String Variable identifier token: Ending in $ (old: @) */
#define FPL_T     3  /* Floating point literal token */
#define INL_T     4  /* Integer literal token */
#define STR_T     5  /* String literal token */
#define SCC_OP_T  6  /* String concatenation operator token: $$ (old: ## or >>) */
#define ASS_OP_T  7  /* Assignment operator token */
#define ART_OP_T  8  /* Arithmetic operator token */
#define REL_OP_T  9  /* Relational operator token */ 
#define LOG_OP_T 10  /* Logical operator token */
#define LPR_T    11  /* Left parenthesis token */
#define RPR_T    12  /* Right parenthesis token */
#define LBR_T    13  /* Left brace token */
#define RBR_T    14  /* Right brace token */
#define KW_T     15  /* Keyword token */
#define COM_T    16  /* Comma token */
#define EOS_T    17  /* End of statement (semicolon) */
#define RTE_T    18  /* Run-time error token */

/* Operators token attributes */
typedef enum ArithmeticOperators { ADD, SUB, MUL, DIV } Arr_Op;
typedef enum RelationalOperators { EQ, NE, GT, LT } Rel_Op;
typedef enum LogicalOperators { AND, OR, NOT } Log_Op;
typedef enum SourceEndOfFile { SEOF_0, SEOF_EOF } S_Eof;

/* Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	int get_int;      /* integer attributes accessor */
	Arr_Op arr_op;    /* arithmetic operator attribute code */
	Rel_Op rel_op;    /* relational operator attribute code */
	Log_Op log_op;    /* logical operator attribute code */
	S_Eof seof;        /* source-end-of-file attribute code */
	int int_value;    /* integer literal attribute (value) */
	int kwt_idx;      /* keyword index in the keyword table */
	short str_offset; /* sring literal offset from the beginning of the string literal buffer (str_LTBL->cb_head) */
	float flt_value;    /* floating-point literal attribute (value) */
	char vid_lex[VID_LEN]; /* variable identifier token attribute */
	char err_lex[ERR_LEN]; /* error token attribute */
} TokenAtt;

/* Should be used if no symbol table is implemented */
typedef struct vidTokenAttibutes {
	unsigned char flags;
	union {
		short int_value;
		float flt_value;
		void* str_locator;
	} values;
} VidAtt;

/* Token declaration */
typedef struct Token {
	int code;     /* token code */
	TokenAtt attribute; /* token attribute */
	VidAtt avid_attribute; /* not used in this scanner implementation - for further use */
} Token;

#endif
