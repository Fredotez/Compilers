/*******************************************************************
 * Filename: parcer.c
 * PURPOSE:
 *    Recursive Descent Predictive Parser (RDPP) for the PLATYPUS 2.0 language.
 *    CST8152, Assignment #3
 *    
 *
 *    Created by: Frederic Desjardins
 *    Student#: 040941359
 *    Date: December 13th 2020
 *
 */


#include "parser.h"

#define debugMode 0

Token lookahead;

/*************************************************************
 * Process Parser
 ************************************************************/
void processParser(void) {
	if (debugMode)
		fprintf(stderr, "[00] Processing parser...\n");
	lookahead = processToken();
	program();
	matchToken(SEOF_T, NO_ATTR);
	printf("%s\n", "PLATY: Source file parsed");
}

/*************************************************************
 * Match Token
 ************************************************************/


void matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	case REL_OP_T:
	case ART_OP_T:
	case LOG_OP_T:
		// TODO_05
		if (tokenAttribute != lookahead.attribute.get_int) {
			syncErrorHandler(tokenCode); 
			return;
		}
		else {
			break;
		}
	default:
		//TO
		if (lookahead.code == tokenCode) {
			matchFlag = 1;
		}
		else {
			matchFlag = 0;
		}
		break;
	}
	if (matchFlag && lookahead.code == SEOF_T) {
		// TODO_07
		return;
	}
	if (matchFlag) {
		// TODO_08
		lookahead = processToken();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = processToken();
			syntaxErrorNumber++;
			return;
		}
	}
	else {
		//TODO_09
		syncErrorHandler(tokenCode); 
		return;
	}
}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/
void syncErrorHandler(int syncTokenCode) {
	// TODO_10
	printError();
	syntaxErrorNumber++;
	
	while (lookahead.code != syncTokenCode) {
	// TODO_11
		lookahead = processToken();
		if (lookahead.code == SEOF_T) {
			if (syncTokenCode != SEOF_T) {
				exit(syntaxErrorNumber);
			} else { return; }
		}
	}
	
	if (lookahead.code != SEOF_T) {
		// TODO_12
		lookahead = processToken();
	}
		return;
}


/*************************************************************
 * Print Error
 ************************************************************/
void printError() {
	Token t = lookahead;
	if (debugMode)
		fprintf(stderr, "[39] PrintError\n");
	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
		// TODO_13

	case  ERR_T: /* -1   Error token */
		
		printf("%s\n", t.attribute.err_lex);
		break;
	case  SEOF_T: /* 0   Source end-of-file token */
		
		printf("SEOF_T\t\t%d\t\n", t.attribute.seof);
		break;
	case  AVID_T: /* 1   Arithmetic Variable identifier token */
	case  SVID_T:/* 2  String Variable identifier token */
		
		printf("%s\n", t.attribute.vid_lex);
		break;
	case  FPL_T: /* 3  Floating point literal token */
	
		printf("%5.1f\n", t.attribute.flt_value);
		break;
	case INL_T: /*  4   Integer literal token */
	
		printf("%d\n", t.attribute.get_int);
		break;
	case STR_T:/*   5   String literal token */

		printf("%s\n", bufferGetString(stringLiteralTable, t.attribute.str_offset));
		break;

	case SCC_OP_T: /* 6   String concatenation operator token */
		
		printf("NA\n");
		break;

	case  ASS_OP_T:/* 7   Assignment operator token */
		
		printf("NA\n");
		break;
	case  ART_OP_T:/* 8   Arithmetic operator token */
		
		printf("%d\n", t.attribute.get_int);
		break;
	case  REL_OP_T: /* 9   Relational operator token */
		printf("%d\n", t.attribute.get_int);
		break;
	case  LOG_OP_T:/*10  Logical operator token */
		printf("%d\n", t.attribute.get_int);
		break;

	case  LPR_T: /*11  Left parenthesis token */
		printf("NA\n");
		break;
	case  RPR_T: /*12  Right parenthesis token */
		printf("NA\n");
		break;
	case LBR_T: /* 13   Left brace token */
		printf("NA\n");
		break;
	case RBR_T: /* 14  Right brace token */
		printf("NA\n");
		break;

	case KW_T: /*  15  Keyword token */		
		printf("%s\n", keywordTable[t.attribute.get_int]);
		break;

	case COM_T: /* 16  Comma token */
		printf("NA\n");
		break;
	case EOS_T: /* 17  End of statement (;) */
		printf("NA\n");
		break;
	case RTE_T: /* 18  Run time error token */
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code: %d\n", t.code);
	}
	
}


/*************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (PLATYPUS)}.
 ************************************************************/
void program(void) {
	if (debugMode)
		fprintf(stderr, "[01] Program\n");
	matchToken(KW_T, PROGRAM);
	matchToken(LBR_T, NO_ATTR);
	opt_statements(); 
	matchToken(RBR_T, NO_ATTR);
	printf("%s\n", "PLATY: Program parsed");
}

/*************************************************************
 * Optional statements
 * <opt_statements> -> <statements> | e
 * FIRST(<opt_statements>) = {AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT), KW_T(OUTPUT),e }
 ************************************************************/

void opt_statements(void) {

	switch (lookahead.code) {
	case AVID_T:
	case SVID_T:
		statements(); 
		break;
	case KW_T:
		
		if (lookahead.attribute.get_int == IF
			|| lookahead.attribute.get_int == WHILE
			|| lookahead.attribute.get_int == INPUT
			|| lookahead.attribute.get_int == OUTPUT) {
			statements();
			break;
		}

	default: 
		printf("PLATY: Opt_statements parsed\n");
	}

}

/*************************************************************
 * Statements
 * <statements> -> <statement> <statementPrime> 
 ************************************************************/
void statements(void) {

	statement();
	statements_prime();

}
/*************************************************************
 * Statement
 * <statement> -> <assignment statement> | <selection statement> | <iteration statement> | <input statement> | <output statement>
 * FIRST (<statement>) = {AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT), KW_T(OUTPUT)}
 ************************************************************/
void statement(void) {
	switch (lookahead.code) {
	case AVID_T:
	case SVID_T:
		assignment_statement();
		break;
	case KW_T:
		switch (lookahead.attribute.get_int) {
		case IF: 
			selection_statement(); 
			break;
		case WHILE: 
			iteration_statement(); 
			break;
		case INPUT: 
			input_statement(); 
			break;
		case OUTPUT: 
			output_statement(); 
			break;
		}
		break;
	default: 
		printError(); 
		break;
	}

}

/*************************************************************
 * Statement
 * FIRST (<statementPrime>) = {AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(INPUT), KW_T(OUTPUT),e }
 ************************************************************/
void statements_prime(void) {
	switch (lookahead.code) {
	case AVID_T:
	case SVID_T:
		statements();
		break;
	case KW_T:

		if (lookahead.attribute.get_int == IF
			|| lookahead.attribute.get_int == WHILE
			|| lookahead.attribute.get_int == INPUT
			|| lookahead.attribute.get_int == OUTPUT) {
			statement();
			statements_prime();
			break;
		}
		else {
			break;
		}
	}

}

/*************************************************************
 * Assignment statement
 * <assignment statement> -> <assignment expression>
 * FIRST (<assignment statement>) = {AVID_T, SVID_T}
 ************************************************************/
void assignment_statement(void) {

	assignment_expression();
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Assignment statement parsed\n");

}
/*************************************************************
 * Assignment expression
 * <assignment expression> -> AVID = <arithmetic expression> | SVID = <string expression>
 * FIRST (<assignment expression>) = {AVID_T, SVID_T}
 ************************************************************/
void assignment_expression(void) {

	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR); /*call match to advance lookahead*/
		matchToken(ASS_OP_T, EQ);
		arithmetic_expression();
		printf("PLATY: Assignment expression (arithmetic) parsed\n");
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASS_OP_T, EQ);
		string_expression();
		printf("PLATY: Assignment expression (string) parsed\n");
		break;
	default: 
		printError(); 
		break;
	}

}
/*************************************************************
 * Selection statement
 * <selection statement> -> if <pre-condition> (<conditional expression>) THEN {<opt_statements>} ELSE {<opt_statements>};
 * FIRST (<selection statement>) = {KW_T(IF)}
 ************************************************************/
void selection_statement(void) {

	matchToken(KW_T, IF);
	pre_condition();
	matchToken(LPR_T, NO_ATTR);
	conditional_expression();
	matchToken(RPR_T, NO_ATTR);
	matchToken(KW_T, THEN);
	matchToken(LBR_T, NO_ATTR);
	opt_statements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(KW_T, ELSE);
	matchToken(LBR_T, NO_ATTR);
	opt_statements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Selection statement parsed\n");

}
/*************************************************************
 * Iteration statement
 * <iteration statement> -> WHILE <pre-condition> (<conditional expression>) DO {<statements>}
 * FIRST (<iteration statement>) = {KW_T(WHILE)}
 ************************************************************/
void iteration_statement(void) {

	matchToken(KW_T, WHILE);
	pre_condition();
	matchToken(LPR_T, NO_ATTR);
	conditional_expression();
	matchToken(RPR_T, NO_ATTR);
	matchToken(KW_T, DO);
	matchToken(LBR_T, NO_ATTR);
	statements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Iteration statement parsed\n");


}
/*************************************************************
 * Pre condition
 * <pre-condition> -> TRUE | FALSE
 * FIRST (<pre-condition>) = {KW_T(TRUE), KW_T(FALSE)}
 ************************************************************/
void pre_condition(void) {

	switch (lookahead.code) {
	case KW_T:
		switch (lookahead.attribute.get_int) {
		case TRUE: 
			matchToken(KW_T, TRUE);
			break;
		case FALSE: 
			matchToken(KW_T, FALSE);
			break;
		default: 
			printError();
			break;
		}
		break;
	default: 
		printError();
		break;
	}

}
/*************************************************************
 * Input statement
 * <input statement> -> INPUT (<variable list>);
 ************************************************************/
void input_statement(void) {
	matchToken(KW_T, INPUT);
	matchToken(LPR_T, NO_ATTR);
	variable_list();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Input statement parsed\n");
}
/*************************************************************
 * Variable List
 * <variable list> -> <variable identifier> <variable list prime>
 * FIRST (<variable list>) = {AVID_T, SVID_T}
 ************************************************************/
void variable_list(void) {
	variable_identifier();
	variable_list_prime();
	printf("PLATY: Variable list parsed\n");

}
/*************************************************************
 * Variable List Prime
 * FIRST (<variable list prime >) = {COM_T, e}
 ************************************************************/
void variable_list_prime(void) {
	switch (lookahead.code) {
	case COM_T: 
		matchToken(COM_T, NO_ATTR);
		variable_identifier(); 
		variable_list_prime();
		break;
	}

}
/*************************************************************
 * Variable identifier
 * <variable identifier> -> AVID | SVID
 * FIRST (<variable identifier>) = {AVID_T, SVID_T}
 ************************************************************/
void variable_identifier(void) {
	switch (lookahead.code) {
	case AVID_T:
	case SVID_T: 
		matchToken(lookahead.code, NO_ATTR);
		break;
	default: 
		printError(); 
		break;
	}

}
/*************************************************************
 * Output statement
 * <output statement> -> OUTPUT (<output statementPrime>);
 ************************************************************/
void output_statement(void) {

	matchToken(KW_T, OUTPUT);
	matchToken(LPR_T, NO_ATTR);
	output_statement_prime();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("PLATY: Output statement parsed\n");

}
/*************************************************************
 * Output variable list
 * <output statement Prime> -> <opt_variable list> | STR_T
 * FIRST (<opt_variable list >) = {AVID_T, SVID_T, STR_T, e}
 ************************************************************/
void output_statement_prime(void) {
	switch (lookahead.code) {
	case AVID_T:
	case SVID_T: 
		variable_list();
		break;
	case STR_T: 
		matchToken(STR_T, NO_ATTR);
		printf("PLATY: Output list (string literal) parsed\n");
		break;
	default:
		printf("PLATY: Output list (empty) parsed\n");
		break;
	}


}
/*************************************************************
 * Arithmetic expression
 * <arithmetic expression> -> <unary arithmetic expression> | <additive arithmetic expression>
 * FIRST (<arithmetic expression>) = {-, +, AVID_T, FPL_T, INT_L}
 ************************************************************/
void arithmetic_expression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case ADD:
		case SUB:
			unary_arithmetic_expression();
			break;
		default: 
			printError();
			break;
		}
		break;
	case AVID_T:
	case FPL_T:
	case INL_T:
	case LPR_T: 
		additive_arithmetic_expression();
		break;
	default: 
		printError(); 
		break;
	}
	printf("PLATY: Arithmetic expression parsed\n");

}
/*************************************************************
 * Unary arithmetic expression
 * <unary arithmetic expression> -> - <primary arithmetic expression> | + < primary arithmetic expression>
 * FIRST (<unary arithmetic expression>) = {-, +}
 ************************************************************/
void unary_arithmetic_expression(void) {
	switch(lookahead.code) {
	case ART_OP_T:		
		switch (lookahead.attribute.arr_op) {
		case ADD:
			matchToken(ART_OP_T, ADD);
			primary_arithmetic_expression();
			break;
		case SUB:
			matchToken(ART_OP_T, SUB);
			primary_arithmetic_expression();
			break;
		default:
			printError();
			break;
		}
	}
	printf("PLATY: Unary arithmetic expression parsed\n");
}
/*************************************************************
 * additive arithmetic expression
 * <additive arithmetic expression> -> + <multiplicative arithmetic expression> <additive arithmetic expression prime>
 * FIRST (<additive arithmetic expression>) = {AVID_T, FPL_T, INT_L, (}
 ************************************************************/
void additive_arithmetic_expression(void) {

	multiplicative_arithmetic_expression();
	additive_arithmetic_expression_prime();

}
/*************************************************************
 * additive arithmetic expression prime
 * <additive arithmetic expression prime> -> + <multiplicative arithmetic expression><additive arithmetic expression prime> |
 * - <multiplicative arithmetic expression><additive arithmetic expression prime> | e
 * FIRST (<additive arithmetic expression prime>) = {+, -, e}
 ************************************************************/
void additive_arithmetic_expression_prime(void) {

	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case ADD:
		case SUB:
			matchToken(ART_OP_T, lookahead.attribute.arr_op);
			multiplicative_arithmetic_expression();
			additive_arithmetic_expression_prime();
			printf("PLATY: Additive arithmetic expression parsed\n");
			break;
		default:
			break;
		}
	}

}
/*************************************************************
 * multiplicative arithmetic expression
 * <multiplicative arithmetic expression> -> <primary arithmetic expression> <multiplicative arithmetic expression prime>
 * FIRST (<multiplicative arithmetic expression>) = {AVID_T, FPL_T, INT_L, (}
 ************************************************************/
void multiplicative_arithmetic_expression(void) {

	primary_arithmetic_expression();
	multiplicative_arithmetic_expression_prime();

}
/*************************************************************
 * multiplicative arithmetic expression prime
 * <multiplicative arithmetic expression prime> -> * <primary arithmetic expression> <multiplicative arithmetic expression prime> | / <primary arithmetic expression> <multiplicative arithmetic expression prime> | e
 * FIRST (<multiplicative arithmetic expression>) = {*, /, e}
 ************************************************************/
void multiplicative_arithmetic_expression_prime(void) {

	switch (lookahead.code) {
	case ART_OP_T:
		switch (lookahead.attribute.arr_op) {
		case MUL:
		case DIV:
			matchToken(ART_OP_T, lookahead.attribute.arr_op);
			primary_arithmetic_expression();
			multiplicative_arithmetic_expression_prime();
			printf("PLATY: Multiplicative arithmetic expression parsed\n");
			break;
		default:
			break;
		}
	}

}
/*************************************************************
 * primary arithmetic expression
 * <primary arithmetic expression> -> AVID_T | FPL_T | INL_T | (<arithmetic expression>)
 * FIRST (<primary arithmetic expression>) = {AVID_T, FPL_T, INT_L, (}
 ************************************************************/
void primary_arithmetic_expression(void) {

	switch (lookahead.code) {
	case AVID_T:
	case FPL_T:
	case INL_T:
		matchToken(lookahead.code, NO_ATTR);
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmetic_expression();
		matchToken(RPR_T, NO_ATTR);
		break;
	default: 
		printError(); 
		break;
	}
	printf("PLATY: Primary arithmetic expression parsed\n");


}
/*************************************************************
 * string expression
 * <string expression> -> <primary string expression> <string expression prime>
 * FIRST (<string expression >) = {SVID_T, STR_T}
 ************************************************************/
void string_expression(void) {
	primary_string_expression();
	string_expression_prime();
	printf("PLATY: String expression parsed\n");
}
/*************************************************************
 * string expression prime
 * <string expression prime> -> $$ <primary string expression> <string expression prime> | e
 * FIRST (<string expression prime>) = {$$, e}
 ************************************************************/
void string_expression_prime(void) {
	switch (lookahead.code) {
	case SCC_OP_T:
		matchToken(SCC_OP_T, NO_ATTR);
		primary_string_expression();
		string_expression_prime();
		break;
	default:
		break;
	}

}
/*************************************************************
 * primary string expression
 * <primary string expression> -> SVID_T | STR_T
 * FIRST (<primary string expression>) = {SVID_T, STR_T}
 ************************************************************/
void primary_string_expression(void) {

	switch (lookahead.code) {
	case SVID_T:
	case STR_T: 
		matchToken(lookahead.code, NO_ATTR);
		break;
	default: 
		printError(); 
		break;
	}
	printf("PLATY: Primary string expression parsed\n");
}
/*************************************************************
 * conditional expression
 * <conditional expression> -> <logical OR expression>
 * FIRST (<conditional expression >) = {AVID_T, SVID_T, INL_T, FPL_T, STR_T}
 ************************************************************/
void conditional_expression(void) {
	logical_OR_expression();
	printf("PLATY: Conditional expression parsed\n");
}
/*************************************************************
 * logical Or expression
 * <logical OR expression> -> <logical AND expression> <logical OR expression prime>
 * FIRST (<logical OR expression>) = {AVID_T, SVID_T, INL_T, FPL_T, STR_T}
 ************************************************************/
void logical_OR_expression(void) {

	logical_AND_expression();
	logical_OR_expression_prime();

}
/*************************************************************
 * logical Or expression prime
 * <logical OR expression prime> -> _OR_ <logical AND expression> <logical OR expression prime> | e
 * FIRST (<logical OR expression prime>) = {_OR_, e}
 ************************************************************/
void logical_OR_expression_prime(void) {

	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.log_op) {
		case OR:
			matchToken(LOG_OP_T, OR);
			logical_AND_expression();
			logical_OR_expression_prime();
			printf("PLATY: Logical OR expression parsed\n");
			break;
		}
		break;
	}

}
/*************************************************************
 * logical AND expression
 * <logical AND expression> -> <logical NOT expression> <logical AND expression prime>
 * FIRST (<logical AND expression>) = {AVID_T, SVID_T, INL_T, FPL_T, STR_T}
 ************************************************************/
void logical_AND_expression(void) {

	relational_expression();
	logical_AND_expression_prime();

}
/*************************************************************
 * logical AND expression prime
 * <logical AND expression prime> -> _AND_ <logical NOT expression> <logical AND expression prime> | e
 * FIRST (<logical AND expression prime>) = {_AND_, e}
 ************************************************************/
void logical_AND_expression_prime(void) {
	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.log_op) {
		case AND:
			matchToken(LOG_OP_T, AND);
			logical_NOT_expression(); 
			logical_AND_expression_prime();
			printf("PLATY: Logical AND expression parsed\n");
			break;
		}
		break;
	}
}
/*************************************************************
 * logical NOT expression prime
 * <logical NOT expression> -> _NOT_ <relational expression> | <relational expression>
 * FIRST (<logical NOT expression>) = {_NOT_, e}
 ************************************************************/
void logical_NOT_expression(void) {

	switch (lookahead.code) {
	case LOG_OP_T:
		switch (lookahead.attribute.log_op) {
		case NOT:
			matchToken(LOG_OP_T, NOT);
			relational_expression();
			printf("PLATY: Logical NOT expression parsed\n");
			break;
		}
		break;
	default:
		relational_expression(); 
		break;
	}

}
/*************************************************************
 * relational expression
 * <relational expression> -> <primary a_relational expression> <primary a_relational expression prime> |
   <primary s_relational expression><primary s_relational expression prime>

 * FIRST (<relational expression>) = {AVID_T, SVID_T, INL_T, FPL_T, STR_T}
 ************************************************************/
void relational_expression(void) {

	switch (lookahead.code) {
	case AVID_T:
	case FPL_T:
	case INL_T:
		primary_a_relational_expression();
		primary_a_relational_expression_prime();
		break;
	case SVID_T:
	case STR_T:
		primary_s_relational_expression();
		primary_s_relational_expression_prime();
		break;
	default: 
		printError();
		break;
	}
	printf("PLATY: Relational expression parsed\n");

}


/*************************************************************
 * primary a relational expression
 * <primary a_relational expression> -> AVID_T | FPL_T | INL_T
 * FIRST (<primary a _relational expression>) = {AVID_T, INL_T, FPL_T}
 ************************************************************/
void primary_a_relational_expression(void) {

	switch (lookahead.code) {
	case AVID_T:
	case FPL_T:
	case INL_T: 
		matchToken(lookahead.code, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("PLATY: Primary a_relational expression parsed\n");


}

/*************************************************************
 * primary a relational expression prime
 * <primary a_relational expression prime> -> == <primary a_relational expression>|
	<> <primary a_relational expression> |
	> <primary a_relational expression> |
	< <primary a_relational expression>

 * FIRST (<primary a_relational expression prime>) = {==, <>, >, <}
 ************************************************************/
void primary_a_relational_expression_prime(void) {

	switch (lookahead.code)
	{
	case REL_OP_T:
		switch (lookahead.attribute.rel_op)
		{
		case EQ:
		case NE:
		case GT:
		case LT:
			matchToken(REL_OP_T, lookahead.attribute.rel_op);
			primary_a_relational_expression();
			break;
		default:
			printError();
			break;
		}
		break;
	default:
		printError();
		break;
	}

}
/*************************************************************
 * primary s relational expression
 * <primary s_relational expression> -> <primary string expression>

 * FIRST (<primary s _relational expression>) = {SVID_T, STR_T}
 ************************************************************/
void primary_s_relational_expression(void) {

	primary_string_expression();
	printf("PLATY: Primary s_relational expression parsed\n");

}
/*************************************************************
 * primary s relational expression prime
 * <primary s_relational expression prime> -> == <primary s _relational expression>|
	<> <primary s _relational expression> |
	> <primary s _relational expression> |
	< <primary s _relational expression> 

 * FFIRST (<primary s_relational expression prime>) = {==, <>, >, <}
 ************************************************************/
void primary_s_relational_expression_prime(void) {

	switch (lookahead.code)
	{
	case REL_OP_T:
		switch (lookahead.attribute.rel_op)
		{
		case EQ:
		case NE:
		case GT:
		case LT:
			matchToken(REL_OP_T, lookahead.attribute.rel_op);
			primary_s_relational_expression();
			break;
		default:
			printError();
			break;
		}
		break;
	default:
		printError();
		break;
	}

}