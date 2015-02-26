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


#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include <stdarg.h>
#include "sf.h"
#include "utils.h"
#include "dbg_utils.h"
#include "traverse.h"

void initialize(void);

int policy_counter	= 0;
FILE *fcode		= NULL;

int state_id_counter	= 0;
int event_id_counter	= 0;
int conf_id_counter	= 0;
int state_defined	= 0;
int module_id_counter	= 0;

int variable_memory_offset = 0;
int global_memory_size = 0;
int shared_memory_size = 0;

int active_state;

int file_status;
char *file_name;
char *error_location;
char *conf_state_suffix = "_cts";

void yyerror(const char *errmsg, ...);
int yylex(void);
int sfc_debug = 0;

%}

%locations

%union {
	struct symtab		*symp;
	struct modtab		*modp;
	struct libtab		*libp;
	int 			ival;
	char 			*str;
	double			dval;
	long double		ldval;
	struct confnode		*confp;
	struct processnodes	*confsp;
	struct conf_id		*confid;
	struct conf_ids		*confids;
	struct event_id		*eventid;
	struct event_ids	*eventids;
	struct statenode	*statep;
	struct statenodes	*statesp;
	struct policy		*pol;
	struct policies		*pols;
	struct initnode		*initp;
	struct program		*prog;
	struct variable		*var;
	struct variables	*vars;
	struct paramtype	*mtl;
	struct paramvalue	*parv;
	struct defvalue		*defv;
}

%token STATE CONFIGURATION COMMA EVENT 
%token FROM GOTO START USE WHEN 
%token APPLICATION NETWORK AM  
%token EQ SOURCE LF 
%token OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token OPEN_SQUARE_BRACE CLOSE_SQUARE_BRACE STAR EXCLAMATION
%token LEVEL AT MALPA TILDE
%token U_INT_EIGHT U_INT_SIXTEEN U_INT_THIRTY_TWO FLOAT DOUBLE


%token <ldval>	CONSTANT
%token <symp>	IDENTIFIER
%token <str>	VARIABLE_LINE
%token <libp>	PATH
%token <ival>	RELOP
%token <ival>	AND
%token <ival>	OR
%token <ival>	VARIABLE_TYPE
%token <symp>	ANY
%token <modp>	MODULES
%type <modp>	module
%type <confp>	process
%type <confsp>	processes
%type <confid>	conf_id
%type <confids> conf_ids
%type <confsp>	defined_processes
%type <statep>	state
%type <statesp>	states
%type <statesp>	defined_states
%type <vars>	defined_global_variables
%type <vars>	global_variables
%type <var>	global_variable
%type <pol>	policy;
%type <pols>	policies;
%type <initp>	initial_process
%type <prog>	program
%type <ival>	module_type
%type <ival> 	state_level
%type <ival> 	process_level
%type <ival> 	module_level
%type <ival> 	fast_switch
%type <ival> 	global	
%type <ival> 	array_part
%type <ldval> 	assign_value
%type <vars>	module_variables
%type <var>	module_variable
%type <vars>	parameters
%type <var>	parameter
%type <ival>	param_type
%type <ival>	process_type

%%

swiftfox: library program 
		{
			/* verbose; to be removed */
			/* printTable(); */
			
			/* root node for the constructed AST */
			//struct program *p = $2;

			/* traverse the AST for semantic checking */	
			traverse_program($2,
					TREE_TRAVERSE,
					policy_counter);


			/* traverse the AST for semantic checking */	
			traverse_program($2,
					TREE_CHECK_SEMANTIC,
					policy_counter);

			/* traverse the AST for code generation */
			traverse_program($2,
					TREE_GENERATE_CODE,
					policy_counter);
		}
	;

program: defined_global_variables defined_processes defined_states policies initial_process 
		{
			/* root node */
			$$		= calloc(1, sizeof(struct program));
			
			/* link the node appropriately */
			if ($2 != NULL) 
				$2->parent = NULL;  
			if ($3 != NULL )
				$3->parent = NULL;
			if ($4 != NULL ) 
				$4->parent = NULL;
		
			/* init */
			$$->vars	= $1;
			$$->defcon	= $2;
			$$->defstate	= $3;
			$$->defpol	= $4;
			$$->init	= $5;
		}
	;


defined_global_variables: global_variables
		{
			$$ = $1;

			if (sfc_debug) {
				printf("\n");
			}
			print_variables(TYPE_VARIABLE_GLOBAL);
			print_variables(TYPE_VARIABLE_SHARED);
		}

global_variables: global_variables global_variable
                {
			/* variable set */
			$$ 		= calloc(1, sizeof(struct variables));

			/* link the child nodes */
			if ($1 != NULL)
				$1->parent = $$;

			$2->parent	= $$;

			$$->vars	= $1;
			$$->var		= $2;
		}
        |       
		{
                        $$ = NULL;
                }                       
        ;


global_variable: param_type global IDENTIFIER array_part assign_value newlines 
		{
			$$ 		= find_variable($3->name);
			$$->type 	= $1;
			$$->length	= $4;
			$$->value	= $5;
			if ($2) {
				$$->offset	= global_memory_size;
				$$->class_type	= TYPE_VARIABLE_GLOBAL;
				global_memory_size += (type_size($$->type) * $$->length);
			} else {
				$$->offset	= shared_memory_size;
				$$->class_type	= TYPE_VARIABLE_SHARED;
				shared_memory_size += (type_size($$->type) * $$->length);
			}
			$$->init	= 1;
			$$->cap_name 	= strdup($3->name);
			$$->cap_name	= str_toupper($$->cap_name);
			$$->global	= 1;
		}

global: MALPA
		{
			$$ = 1;
		}
	|
		{
			$$ = 0;
		}
	;


array_part: OPEN_SQUARE_BRACE CONSTANT CLOSE_SQUARE_BRACE
		{
			$$ = (int)$2;
		}
	|
		{
			$$ = 1;
		}
	;

assign_value: RELOP CONSTANT
		{
			if ($1 == EQ) {
				$$ = $2;
			} else {
				$$ = 0;
			}
		}
	|
		{
			$$ = 0;
		}
	; 


defined_processes: processes process
		{
			/* processes set */
			$$		= calloc(1, sizeof(struct processnodes));
			
			/* link the child nodes */
			if ($1 != NULL)
				$1->parent = $$;
			$2->parent	= $$;
			
			$$->confs	= $1;
			$$->conf	= $2;
		}	
	|
		{
			$$ = NULL;
		}
	;

processes: processes process
		{
			/* processes set */
			$$		= calloc(1, sizeof(struct processnodes));
			
			/* link the child nodes */
			if ($1 != NULL) 
				$1->parent = $$;
			$2->parent	= $$;

			$$->confs	= $1;
			$$->conf	= $2;
		}
	| 	
		{ 
			$$ = NULL; 
		}			
	;

process: process_type IDENTIFIER process_level OPEN_BRACE newlines module newlines module newlines module_level module newlines CLOSE_BRACE newlines
		{
			/* process node */
			$$		= calloc(1, sizeof(struct confnode));

			/* init */
			$2->type	= $1;
			$$->id		= $2;
			$$->daemon	= $3;

			/* link application module */
			if (($6 == NULL) || (($6->type != TYPE_APPLICATION && $6->type != TYPE_EVENT))) {
				fprintf(stderr, "Undefined application module in %s\n", $2->name);
				yyerror("expecting application module");
			}
			$$->app			= $6;
			$$->app->lib->used 	= 1;


			/* link network module */
			if (($8 == NULL) || ($8->type != TYPE_NETWORK)) {
				fprintf(stderr, "Undefined network module in %s\n", $2->name);
				yyerror("expecting network module");
			}
			$$->net			= $8;
			$$->net->lib->used 	= 1;


			/* link am module */
			if (($11 == NULL) || ($11->type != TYPE_AM)) {
				fprintf(stderr, "Undefined am module in %s\n", $2->name);
				yyerror("expecting am module");
			}
			$$->am			= $11;
			$$->am_dominant		= $10;
			$$->am->lib->used 	= 1;


			$2->value	= conf_id_counter;

			if ($1 == TYPE_PROCESS_EVENT) {
				$2->value = event_id_counter;
				event_id_counter++;
			}

			$$->name = $2->name;
			$$->id_name = conf_module_name($$->name, "process");

			$$->counter	= conf_id_counter;
			$$->app_id_name = conf_module_name($$->name, $$->app->lib->name);
			$$->net_id_name = conf_module_name($$->name, $$->net->lib->name);
			$$->am_id_name = conf_module_name($$->name, $$->am->lib->name);

			conftab[conf_id_counter].conf = $$;

			++conf_id_counter;
		}
	;


process_type: CONFIGURATION
		{
			$$ = TYPE_PROCESS_REGULAR;
		}
	| EVENT
		{
			$$ = TYPE_PROCESS_EVENT;
		}
	|
		{
			$$ = TYPE_UNKNOWN;
		}
	;

process_level: EXCLAMATION 
		{
			$$ = 1;
		}
        |       
                {
                        $$ = 0;
                }                       
        ;

module_level: STAR
		{
			$$ = 1;
		}
	|
		{
			$$ = 0;
		}
	;

module: IDENTIFIER OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS
		{
			$$ = proc_module($1->name);
			$$->variables = $3;
			if ($3 != NULL) {
				$3->parent = NULL;
			}
		}		
	;

parameters: parameters parameter
                {

			$$              = calloc(1, sizeof(struct variables));

			/* link the child nodes */
			if ($1 != NULL)
                                $1->parent = $$;

                        $2->parent      = $$;

                        $$->vars        = $1;
                        $$->var         = $2;
                }
	|	
		{
                        $$ = NULL;
		}
        ;

parameter: CONSTANT
		{
			$$		= find_variable("const");
			$$->class_type	= TYPE_VARIABLE_LOCAL;
			$$->value	= $1;
		}
	| newlines COMMA newlines CONSTANT
		{
			$$		= find_variable("const");
			$$->class_type	= TYPE_VARIABLE_LOCAL;
			$$->value	= $4;
		}

	| IDENTIFIER
                {
			struct variable *vp	= find_variable($1->name);
			if ((vp->class_type != TYPE_VARIABLE_GLOBAL) && (vp->class_type != TYPE_VARIABLE_SHARED)) {
				fprintf(stderr, "Variable %s is not global (or shared)\n", $1->name);
				yyerror("undefined variable");
			}
			$$ = vp;

                }
	| newlines COMMA newlines IDENTIFIER
                {
			struct variable *vp	= find_variable($4->name);
			if ((vp->class_type != TYPE_VARIABLE_GLOBAL) && (vp->class_type != TYPE_VARIABLE_SHARED)) {
				fprintf(stderr, "Variable %s is not global (or shared)\n", $4->name);
				yyerror("undefined variable");
			}
			$$ = vp;
                }
        ;


defined_states: states state
		{
			/* states set */
			$$		= calloc(1, sizeof(struct statenodes));
			
			/* link the child nodes */
			if ($1 != NULL)
				$1->parent = $$;
			$2->parent	= $$;

			$$->states	= $1;
			$$->state	= $2;
		}	
	|
		{
			$$ = NULL;
		}
	;


states: states state
		{
			/* states set */
			$$		= calloc(1, sizeof(struct statenodes));
			
			/* link the child nodes */
			if ($1 != NULL) 
				$1->parent = $$;
			$2->parent	= $$;

			$$->states	= $1;
			$$->state	= $2;

		}
	| 	
		{ 
			$$ = NULL; 
		}			
	;



state: STATE IDENTIFIER state_level OPEN_BRACE newlines conf_ids newlines CLOSE_BRACE newlines
		{

			/* process node */
			$$		= calloc(1, sizeof(struct statenode));

			/* init */
			$2->type	= TYPE_STATE;
			$$->id		= $2;

			/* level */
			$$->level	= $3;


			$$->confs	= $6;

			$$->counter	= state_id_counter;
			$2->value	= state_id_counter;
			$$->id_name	= conf_module_name($$->id->name, "state");
			++state_id_counter;

			statetab[$$->counter].state = $$;

			state_defined	= 1;
		}
	;

state_level: LEVEL CONSTANT
		{
			$$ = $2;
		}
        |       
                {
                        $$ = UNKNOWN;
                }                       
        ;


conf_ids: conf_ids newlines conf_id
		{
			/* states set */
			$$		= calloc(1, sizeof(struct conf_ids));
			
			/* link the child nodes */
			$$->confs	= $1;
			$$->conf	= $3;

		}
	| conf_ids AND newlines conf_id
		{
			/* states set */
			$$		= calloc(1, sizeof(struct conf_ids));
			
			/* link the child nodes */
			$$->confs	= $1;
			$$->conf	= $4;
		}
	| 	
		{ 
			$$ = NULL; 
		}			
	;


conf_id: IDENTIFIER
		{
			$$		= calloc(1, sizeof(struct conf_id));

			$$->id		= $1;
			$$->conf	= conflook($1);	
		}
	;


policies: policies policy 
		{
			/* policies set */
			$$ = calloc(1, sizeof(struct policies));

			/* link child nodes */
			if ($1 != NULL) 
				$1->parent = $$;
			$2->parent = $$;

			$$->pols	= $1;
			$$->pol		= $2;
		}
	|	
		{
			$$ = NULL;
		}		
	;

policy: fast_switch FROM IDENTIFIER GOTO IDENTIFIER WHEN conf_ids newlines
		{
			/* policy node */
			$$ 		= calloc(1, sizeof(struct policy));

			/* link child nodes */
			$$->from	= $3;
			$$->to		= $5;

			$$->event_confs	= $7; 
			$$->counter	= policy_counter;
			$$->fast	= $1;

			if (proc_policy($$)) {
				fprintf(stderr, "Failed to store policy\n");
				exit(1);
			}
			++policy_counter;
		}
	;

fast_switch: TILDE
		{
			$$ = 1;
		}
	|
		{
			$$ = 0;
		}
	;


initial_process: START IDENTIFIER newlines
		{
			/* start node */
			$$ = calloc(1, sizeof(struct initnode));

			/* link child nodes */
			$$->id = $2;
			$$->init = symlook($2->name)->value;
		} 
	;


/* Library section */


library: newlines definitions
		{
			if (sfc_debug) {
				printf("\n");
			}
			print_variables(TYPE_VARIABLE_DEFAULT);
		}
	;

definitions: definitions definition 
        |
        ;

definition: USE module_type IDENTIFIER PATH OPEN_PARENTHESIS newlines module_variables newlines CLOSE_PARENTHESIS
		{
			/* iterator */
			char *p = NULL;
			
			/* lookup */
			struct symtab *sp = NULL;

			/* check for library re-declarations */
			if ((sp = symlook($3->name)) != NULL && sp->type != TYPE_UNKNOWN)
				/* failed */
				yyerror("redeclaration of library");

			if (sfc_debug) {
				printf("define module: %s\n", $3->name);
			}

			/* fix the child properties */
			$3->type = $2;
			$3->lib = $4;

			/* set variables for the library */
			$4->variables = $7;

			if (sfc_debug) {
				struct variables *v = $7;
				printf("\t%-10s \t%-20s \tVALUE\t\tINIT\tOFFSET\tCLASS_TYPE\n", "TYPE", "NAME");
				for (v = $7; v != NULL; v=v->vars) {
					printf("\t%-10d \t%-20s \t%-10.1Lf \t%d \t%d \t%d\n", v->var->type,
					v->var->name, v->var->value, v->var->init, v->var->offset,
					v->var->class_type);	
				}
			}

                        /* extract the name from the path */
                        if ((p = rindex($4->path, '/')) == NULL)
                                $4->name = $4->path;
                        else
                                $4->name = ++p;

                        $4->used = 0;

                        /* save the name as it was defined */
                        $4->def = $3->name;

			/* differentiate based on the definition type */
			switch($3->type) {
			case TYPE_APPLICATION:
				/* application */
				$4->type = TYPE_APPLICATION;
				$4->used = 0;
				$4->id = 0;
				break;

			case TYPE_EVENT:
				$4->type = TYPE_EVENT;
				$4->used = 0;
				$4->id = 0;
				break;

			case TYPE_NETWORK:
				/* network */
				$4->type = TYPE_NETWORK;
				$4->used = 0;
				$4->id = 0;
				break;

			case TYPE_AM:
                                /* am */
                                $4->type = TYPE_AM;
				$4->used = 0;
                                $4->id = 0;
				break;
                        
			default:
				printf("UNKNOWN TYPE\n"); 
			}

		}
		newlines
        ;

module_type: APPLICATION 	{ $$ = TYPE_APPLICATION; }
        | NETWORK 		{ $$ = TYPE_NETWORK; }
        | AM 			{ $$ = TYPE_AM; }
	| SOURCE 		{ $$ = TYPE_EVENT; }
	|			{ $$ = TYPE_UNKNOWN; }
        ;


module_variables: module_variables module_variable
		{
			$$              = calloc(1, sizeof(struct variables));

			/* link the child nodes */
			if ($1 != NULL)
				$1->parent = $$;

			$2->parent      = $$;

			$$->vars        = $1;
			$$->var         = $2;
		}
	|       
		{
			$$ = NULL;
		}                       
	;


module_variable: param_type IDENTIFIER assign_value newlines 
		{
			$$		= find_variable($2->name);
			$$->type	= $1;
			$$->value	= $3;
			$$->init	= 1;
			$$->length	= 1;
			$$->class_type	= TYPE_VARIABLE_DEFAULT;

		}
	| newlines COMMA newlines param_type IDENTIFIER assign_value newlines
		{
			$$		= find_variable($5->name);
			$$->type	= $4;
			$$->value	= $6;
			$$->init	= 1;
			$$->length	= 1;
			$$->class_type	= TYPE_VARIABLE_DEFAULT;
		}
	;


param_type: VARIABLE_TYPE
		{
			$$ = $1;
		}
	|
		{
			$$ = 0;
		}
	;

newlines: newlines newline
	|
	;

newline: LF 
	;

%%

char program_file[PATH_SZ];
char library_file[PATH_SZ];
char fennec_library_file[PATH_SZ];

extern FILE *yyin;
extern char *yytext;
extern int yyleng;

/* main entry point */
int
start_parser(char *pfile, char *lfile) {

	/* check the file extention */
	if (rindex(pfile, '.') != NULL && !strcmp(rindex(pfile, '.'), ".sfp")) {
		pfile[strlen(pfile)-4] = '\0';
	}
		
	/* init */
	(void)memset(program_file, 0, PATH_SZ);
	(void)memset(library_file, 0, PATH_SZ);
	(void)memset(fennec_library_file, 0, PATH_SZ);

	(void)snprintf(program_file, PATH_SZ, "%s.sfp", pfile);
	if (lfile != NULL) {
		(void)snprintf(library_file, PATH_SZ, "%s.sfl", lfile);
	}
	(void)snprintf(fennec_library_file, PATH_SZ, "%s/%s", 
			getenv("FENNEC_FOX_LIB"), STD_FENNEC_FOX_LIB);


	if ((error_location = malloc(strlen("fennec_library") + 1)) == NULL) {
		/* failed */
		(void)fprintf(stderr, 
			"Cannot allocate memory for error_location\n");
		exit(1);
	}

	initialize();
	
	/* process libraries */
	
	/* try fennec fox standart libray located at ($FENNEC_FOX_LIB)/STD_FENNEC_FOX_LIB */
	yyin = fopen(fennec_library_file, "r");
	strcpy(error_location, "fennec library");
	file_status = 1;
	file_name = fennec_library_file;

	if (!yyin) {
		/* failed */
		(void)fprintf(stderr, 
			"%s.sfl: no such file or directory and no standard library\n",
			lfile);
		exit(1);
	}

	/* main loop */
	do {
		yyparse();

	} while(!feof(yyin));

	return 0;
}

/* error reporting */
void
yyerror(const char *format, ...) {

	va_list arg;

	/* error in program */
//	(void)fprintf(stderr, "\nsfc in %s %s at line %d, position %d: ", error_location,
//							file_name, yylineno,
//                                                      yycolumn - yyleng + 1);

	(void)fprintf(stderr, "\n\nsfc in %s %s: ", error_location, file_name);

	va_start (arg, format);
        (void)fprintf(stderr, format, arg);
	va_end (arg);

	/* line */
	(void)fprintf(stderr, "\n\n%d: %s\n", yylineno, linebuf);
	(void)fprintf(stderr, "%*s\n", yycolumn - yyleng + 1, "^");


	/* terminate */
	exit(1);
}

/* restart the parser with the program file */
int
yywrap(void) {
	struct stat st;

	/* finish; done with library and program */
	switch(file_status) {
		case 1:
			/* re-init */
			yylineno	= 1;
			file_status 	= 2;
			file_name 	= library_file;

	        	/* open the specific library file */
			if (stat(library_file, &st) == 0) {
		        	yyin 		= fopen(library_file, "r");
				strcpy(error_location, "library");	

				if (!yyin) {
					/* failed */
					(void)fprintf(stderr, 
						"%s: no such file or directory\n",
						library_file);
		                	exit(1);
				}
				break;
			}

		case 2:
			/* re-init */
			yylineno	= 1;
			file_status 	= 3;
			file_name 	= program_file;

			/* open the program file */
		       	yyin		= fopen(program_file, "r");
			strcpy(error_location, "program");
        		if (!yyin) {
				/* failed */
				(void)fprintf(stderr, 
					"%s: no such file or directory\n",
					program_file);
		                exit(1);
	        	}	
			break;

		case 3:
			return 1;
	}

	/* default */
	return 0;
}

/* symbol lookup */
struct symtab *
symlook(char *s) {
	/* iterator */
 	struct symtab *sp = NULL;

	/* loop */
	for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
        	/* is it already here? */
	        if (sp->name && !strcmp(sp->name, s))
	      		return sp;

		/* is it free */
		if(!sp->name) {
		        /* check if this symbol has related library */
		        struct libtab *lp = NULL;
		        /* loop */
		        for(lp = libtab; lp < &libtab[NLIBS]; lp++) {
                		/* is it this the lib ? */
		                if (lp->def != NULL && !strcmp(lp->def, s)) {
					sp->lib = lp;
					sp->name = lp->def;
					return sp;
				}
			}
			sp->lib = NULL;
			sp->name = strdup(s);
			return sp;
		}
		/* otherwise continue to next */
	}
	yyerror("symtab is full");
	return sp;
}


/* symbol */
struct symtab *
find_module_symtab(char *s) {
        /* iterator */
        struct symtab *sp = NULL;

        /* loop */
        for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
                /* is it already here? */
		if (sp->name && !strcmp(sp->name, s) && (sp->lib != NULL)) {
			if (check_path(sp->lib->path)) {
				yyerror("module path does not exists");
			}
                        return sp;
		}

        }
	return NULL;
}

struct variable *
find_variable(char *varname) {
        /* iterator */
        struct variable *vp = NULL;

	/* loop */
	for(vp = vartab; vp < &vartab[NVARS]; vp++) {
        	/* is it already here? */
	        if (vp->name && (varname!=NULL) && !strcmp(vp->name, varname) &&
			(vp->class_type == TYPE_VARIABLE_GLOBAL || vp->class_type == TYPE_VARIABLE_SHARED) && vp->global) {
	      		return vp;
		}

		/* is it free */
		if(!vp->name) {
			if (varname != NULL) {
				vp->name = strdup(varname);
				vp->cap_name = strdup(vp->name);
				vp->cap_name = str_toupper(vp->cap_name);
			} else {
				vp->cap_name = NULL;
			}
			vp->used = 0;
			vp->offset = -1;
			vp->id = -1;
			vp->gname = NULL;
			vp->global = 0;
			return vp;
		}
		/* otherwise continue to next */
	}
	yyerror("vartab is full");
	return NULL;
}

/* process_module */
struct modtab *
proc_module(char *s) {
	char * c;

	/* symtab ptr */
	struct symtab *sp = find_module_symtab(s);

        /* iterator */
        struct modtab *mp = NULL;
	int duplicate = 0;

	/* check if this is a know module */
	if (sp == NULL) {
		yyerror("no such module");
	}

        /* loop */
        for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
                /* is it free */
                if(!mp->lib) {
			mp->type = sp->type;
			mp->lib = sp->lib;
			mp->variables = NULL;
			mp->name = strdup(sp->lib->name);
			mp->id = ++module_id_counter;
			mp->id_name = conf_module_name(mp->name, "module");
			mp->duplicate = duplicate;
			for(c = mp->name; *c != '\0'; c++ ) {
				*c = toupper(*c);
			}
                        return mp;
                }
		if (strcmp(mp->lib->def, sp->name) == 0) {
			duplicate = 1;
		//	return mp;
		}
        }
        yyerror("modtab is full");
	return mp;
}

/* store policy */
int
proc_policy(struct policy *p) {
	poltab[policy_counter].policy = p;
//	poltab[policy_counter].src_conf = &p->from->value;
//	poltab[policy_counter].event_mask = &p->mask_r;
//	poltab[policy_counter].dst_conf = &p->to->value;

	return 0;	
}

/* finds process record based on its string ID */
struct confnode *
conflook(struct symtab *conf_id) {
	/* iterator */
	struct conftab *cn = NULL;

	/* loop */
	for(cn = conftab; cn < &conftab[NCONFS]; cn++) {
		if (cn->conf && cn->conf->id && (cn->conf->id == conf_id)) {
			return cn->conf;
		}
	}
	yyerror("Configuration undefined");
	return NULL;
}



/* library lookup */
struct libtab*
liblook(char *l) {
	
	/* iterator */
	struct libtab *lp = NULL;

	/* loop */
        for(lp = libtab; lp < &libtab[NLIBS]; lp++) {
                /* is it already here? */
                if (lp->path && !strcmp(lp->path, l)) 
			yyerror("library already exists");

                /* is it free */
                if(!lp->path) {
                        lp->path = strdup(l);
			lp->used = 0;
                        return lp;
                }

                /* otherwise continue to next */
        }
        yyerror("libtab is full");
	return lp;
}

/* event-condition lookup */
struct evtab*
evlook(char *name) {

	/* iterator */
	struct evtab *ev = NULL;

	/* loop */
	for(ev = evtab; ev < &evtab[NEVS]; ev++) {
		/* found */
                if (ev->name && !strcmp(ev->name, name)) 
			return ev;
		
		/* insert */
		if (!ev->name) {
			ev->name = strdup(name);
			return ev;
		}
	}

	/* failed */
	yyerror("evtab is full");
	return ev;
}

/* initialize the keywords set */
void
initialize(void) {

	/* keywords set */
	char *keywords[] = {"start", "use", "application",
			"network", "source", "from", "goto",
			"on", "off", "process", "state",
			"am", // new keywords
			"when", "event"};
	
	/* size of the keywords set */
	int k_num 		= sizeof(keywords)/sizeof(char*);
	
	/* iterate */
	int i			= 0;
	struct symtab *sp	= NULL;
	
	/* init */
	for(i = 0, sp = symtab; i < k_num; i++, sp++) {
		sp->name = keywords[i];
		sp->value = 0;
		sp->type = TYPE_KEYWORD;
	}
	sp->name = "any";
	sp->value = 253;
	sp->type = TYPE_KEYWORD;
}

void printTable() {
	struct symtab *sp;
	printf("\n");
        for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
                /* is it already here? */
                if (sp->name ) {
	                        printf("%s   %d   %d\n", sp->name, sp->type, sp->value);
                } else {
			printf("\n\n");
			break;
		}
        }

        struct evtab *ev;
        for(ev = evtab; ev < &evtab[NEVS]; ev++) {
                if (ev->name) {
			printf("%s     %d    %d     %d\n", ev->name, ev->num, ev->op, ev->value);
                } else {
			printf("\n\n");
			return;
		}
        }
} 





