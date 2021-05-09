/*******************************************************************
 * Filename: buffer.h
 * PURPOSE:
 *    BUFFER.H: Helper file for buffer.c
 *
 *
 *    Created by: Frederic Desjardins
 *    Student#: 040941359
 *    Date: November 15th 2020
 *
 */


#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */


/* constant definitions */
#define RT_FAIL_1 (-1)         /* operation failure return value 1 */
#define RT_FAIL_2 (-2)         /* operation failure return value 2 */
#define LOAD_FAIL (-2)         /* load fail return value */

#define DEFAULT_INIT_CAPACITY 200   /* default initial buffer capacity */
#define DEFAULT_INC_FACTOR 15       /* default increment factor */


/* You should add your own constant definitions here */

/* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS	0xFFFC 	// 1111.1111 1111.1100
#define SET_EOB			0xFFFD	// 1111 1111 1111 1101 OR
#define RESET_EOB		0xFFFE  // 1111.1111 1111.1110 AND
#define CHECK_EOB		0xFFFD	// 1111.1111 1111.1101 if (flag & checkebo == checkeob)
#define SET_R_FLAG		0xFFFE	// 1111.1111 1111.1110 OR
#define RESET_R_FLAG	0xFFFD	// 1111 1111 1111 1101 AND
#define CHECK_R_FLAG	0xFFFE	// 1111 1111 1111 1110 if (flag & checkr == checkr)


/* user data type declarations */
/* user data type declarations */
typedef struct BufferEntity {
	char* string;   /* pointer to the beginning of character array (character buffer) */
	short capacity;    /* current dynamic memory size (in bytes) allocated to character buffer */
	short addCPosition;  /* the offset (in chars) to the add-character location */
	short getCPosition;  /* the offset (in chars) to the get-character location */
	short markCPosition; /* the offset (in chars) to the mark location */
	char  increment; /* character array increment factor */
	char  opMode;       /* operational mode indicator*/
	unsigned short flags;     /* contains character array reallocation flag and end-of-buffer flag */
} Buffer, * pBuffer;

/*
IMPORTANT NOTE (2DO):
Place your function declarations here.
Do not include the function header comments here.
Place them in the buffer.c file
*/


short bufferRetract(Buffer* const pBE);

int bufferClear(Buffer* const pBE);

int bufferIsEmpty(Buffer* const pBE);

Buffer* bufferSetEnd(Buffer* const pBE, char symbol);

pBuffer bufferCreate(short initCapacity, char incFactor, char opMode);

char bufferGetChar(Buffer* const pBE);

pBuffer bufferAddChar(pBuffer const pBE, char symbol);

int bufferLoad(FILE* const fi, Buffer* const pBE);

int bufferPrint(pBuffer const pBE, char newLine);

short bufferGetCapacity(Buffer* const pBE);

short bufferGetAddCPosition(Buffer* const pBE);

int bufferGetOpMode(Buffer* const pBE);

size_t bufferGetIncrement(Buffer* const pBE);

int bufferRewind(Buffer* const pBE);

unsigned short bufferGetFlags(pBuffer const pBE);

char* bufferGetString(Buffer* const pBE, short charPosition);

void bufferFree(Buffer* const pBE);

short bufferSetMarkPosition(pBuffer const pBE, short mark);

short bufferReset(Buffer* const pBE);

short bufferGetCPosition(Buffer* const pBE);

#endif
