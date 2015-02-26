%{

/*
 * Copyright (c) 2010-2014, Columbia University.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of the Columbia University nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL COLUMBIA UNIVERSITY BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
  * Swift Fox compiler
  *
  * @author: Marcin K Szczodrak
  */

#include <stdio.h>
#include <math.h>
#ifdef __DEBUG__	/* link with the testing suite */
#include <unistd.h>
#include "common.h"
#else			/* link with YACC/Bison */
#include <fcntl.h>
#include <ctype.h>
#include <stdlib.h>
#include "parser.h"
#include "sf.h"

#define YY_NO_INPUT
#endif

/* handle locations */
int yycolumn = 1;

#define YY_USER_ACTION yycolumn += yyleng;


%}
%option nounput
%option yylineno

delim		[ \v\f\r]
tab		\t+
whitespace	{delim}+
letter		[A-Za-z]
digit		[0-9]
number		(-)*[1-9]{digit}*
floating	{digit}+\.{digit}+		
newline		\n
identifier	({letter}|_)({letter}|{digit}|_)*
hex		0x([A-Fa-f]|{digit})+
constant	{floating}|{digit}|{number}|{hex}|{floating}
env_const	\$\(({letter}|{digit}|_)*\)
path		(http:\/\/|\.|\/|{env_const})({letter}|{digit}|\.|\/|_|-|~)*
comment		#.*

%%


^.* {
			//yycolumn = 1;
			(void)memset(linebuf, 0, BUF_SZ);
			strncpy(linebuf, yytext, BUF_SZ - 1);  
			yyless(0);
			yycolumn = 1;
}

{newline}       {
#ifndef __DEBUG__
#endif
                        return LF;                              }


{tab}		{
#ifndef __DEBUG__
#endif
								}


{whitespace}	{
#ifndef __DEBUG__
#endif
							}
{comment}	{
#ifndef __DEBUG__
#endif
							}
any		{
#ifndef __DEBUG__
			yylval.symp = symlook(yytext);
#endif
			return ANY;			}
(proc|process)	{
#ifndef __DEBUG__
#endif
			return CONFIGURATION;		}
,		{
#ifndef __DEBUG__
#endif
			return COMMA;			}

state			{
#ifndef __DEBUG__
#endif
			return STATE;			}

(event-condition|event)	{
#ifndef __DEBUG__
#endif
			return EVENT;			}
from		{
#ifndef __DEBUG__
#endif
			return FROM;			}
goto		{
#ifndef __DEBUG__
#endif
			return GOTO;			}

start		{
#ifndef __DEBUG__
#endif
			return START;			}

(at)           {
#ifndef __DEBUG__
#endif
                        return AT;                   }

(level|l)           {
#ifndef __DEBUG__
#endif
                        return LEVEL;                   }

use		{
#ifndef __DEBUG__
#endif
			return	USE;			}
application	{
#ifndef __DEBUG__
#endif	
			return APPLICATION;		}
network		{
#ifndef __DEBUG__
#endif
			return NETWORK;			}
am         {
#ifndef __DEBUG__
#endif
                        return AM;			}
source		{
#ifndef __DEBUG__
#endif
			return SOURCE;			}
when		{
#ifndef __DEBUG__
#endif
			return WHEN;			}
and		{
#ifndef __DEBUG__
			yylval.ival = AND; 
#endif
			return AND;			}
or		{
#ifndef __DEBUG__
			yylval.ival = OR; 
#endif
			return OR;			}
"="		{
#ifndef __DEBUG__
			yylval.ival = EQ;
#endif
			return RELOP;			}
\{		{
#ifndef __DEBUG__
#endif
			return OPEN_BRACE;		}
\}		{
#ifndef __DEBUG__
#endif
			return CLOSE_BRACE;		}
\(              {
#ifndef __DEBUG__
#endif
                        return OPEN_PARENTHESIS;              }
\)              {
#ifndef __DEBUG__
#endif
			return CLOSE_PARENTHESIS;		}
"["              {
#ifndef __DEBUG__
#endif
                        return OPEN_SQUARE_BRACE;              }
"]"              {
#ifndef __DEBUG__
#endif
                        return CLOSE_SQUARE_BRACE;               }
"*"	{
#ifndef __DEBUG__
#endif
			return STAR;			}
"!"	{
#ifndef __DEBUG__
#endif
			return EXCLAMATION;		}
"@"	{
#ifndef __DEBUG__
#endif
			return MALPA;			}
"~"	{
#ifndef __DEBUG__
#endif
			return TILDE;			}
bool		{
#ifndef __DEBUG__
			yylval.ival = TYPE_BOOL;
#endif
			return VARIABLE_TYPE;		}
uint8_t		{
#ifndef __DEBUG__
			yylval.ival = TYPE_UINT8_T;
#endif
			return VARIABLE_TYPE;		}
uint16_t	{
#ifndef __DEBUG__
			yylval.ival = TYPE_UINT16_T;
#endif
			return VARIABLE_TYPE;		}
uint32_t	{
#ifndef __DEBUG__
			yylval.ival = TYPE_UINT32_T;
#endif
			return VARIABLE_TYPE;		}
nx_uint8_t		{
#ifndef __DEBUG__
			yylval.ival = TYPE_NXUINT8_T;
#endif
			return VARIABLE_TYPE;		}
nx_uint16_t	{
#ifndef __DEBUG__
			yylval.ival = TYPE_NXUINT16_T;
#endif
			return VARIABLE_TYPE;		}
nx_uint32_t	{
#ifndef __DEBUG__
			yylval.ival = TYPE_NXUINT32_T;
#endif
			return VARIABLE_TYPE;		}
float		{
#ifndef __DEBUG__
			yylval.ival = TYPE_FLOAT;
#endif
			return VARIABLE_TYPE;		}
double		{
#ifndef __DEBUG__
			yylval.ival = TYPE_DOUBLE;
#endif
			return VARIABLE_TYPE;		}
int8_t         {
#ifndef __DEBUG__
			yylval.ival = TYPE_INT8_T;
#endif
			return VARIABLE_TYPE;           }
int16_t        {
#ifndef __DEBUG__
			yylval.ival = TYPE_INT16_T;
#endif
			return VARIABLE_TYPE;           }
int32_t	{
#ifndef __DEBUG__
			yylval.ival = TYPE_INT32_T;
#endif
			return VARIABLE_TYPE;		}
{identifier}	{
#ifndef __DEBUG__
			yylval.symp = symlook(yytext);
#endif
			return IDENTIFIER;		}
{path}		{
#ifndef __DEBUG__
			yylval.libp = liblook(yytext);
#endif
			return PATH;			}
{constant}	{
#ifndef __DEBUG__
			yylval.ldval = atof(yytext);
#endif
			return CONSTANT;		}

%%

#ifdef __DEBUG__	/* link with the testing suite */
int
main(int argc, char **argv) {

	int tok;	/* token from the scanner */

        /* try to open the first argument for input */
        if ((argc != 1) && ((yyin = fopen(argv[1], "r")) == NULL))
		/* failed */
		yyin = stdin;	/* read from stdin */

	/* call the scanner repetitively */
	while ((tok = yylex()) != 0)
		/* print the identified token */
		(void)fprintf(stdout, "%d ", tok);
	/* EOL */
	(void)fprintf(stdout, "\n");
	
	/* cleanup */
	(void)fclose(yyin);

	/* finish */
	return EXIT_SUCCESS;
}
#endif
