%{
/*  
    Swift Fox Compiler
    Copyright (C) 2009-2012 Marcin Szczodrak

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "sf.h"
#include "traverse.h"

void initialize(void);
void gc(void);
void checkForRemotePath(struct libtab*);

int policy_counter	= 0;
FILE *fcode		= NULL;

int state_id_counter	= 0;
int module_id_counter 	= 1;
int event_id_counter	= 0;
int conf_id_counter	= 0;
int state_defined	= 0;

int active_state;

int file_status;
char *file_name;
char *error_location;
char *conf_state_suffix = "_cts";

void yyerror(char *errmsg);
int yylex(void);

%}

%union {
	struct symtab		*symp;
	struct modtab		*modp;
	struct libtab		*libp;
	int 			ival;
	char 			*str;
	double			dval;
	struct confnode		*confp;
	struct confnodes	*confsp;
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
%token APPLICATION NETWORK MAC RADIO 
%token EQ SOURCE LF 
%token OPEN_BRACE CLOSE_BRACE OPEN_PARENTHESIS CLOSE_PARENTHESIS
%token OPEN_SQUARE_BRACE CLOSE_SQUARE_BRACE
%token LEVEL AT 
%token U_INT_EIGHT U_INT_SIXTEEN U_INT_THIRTY_TWO FLOAT DOUBLE


%token <dval>	CONSTANT
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
%type <confp>	configuration
%type <confsp>	configurations
%type <confid>	conf_id
%type <confids> conf_ids
%type <confsp>	defined_configurations
%type <statep>	state
%type <statesp>	states
%type <statesp>	defined_states
%type <vars>	global_variables
%type <var>	global_variable
%type <pol>	policy;
%type <pols>	policies;
%type <initp>	initial_configuration
%type <prog>	program
%type <ival>	module_type
%type <ival> 	conf_level
%type <ival> 	array_part
%type <dval> 	assign_value
%type <ival> 	state_level
%type <mtl>	module_types
%type <mtl>	next_module_type
%type <parv>	parameters
%type <parv>	next_parameter
%type <ival>	param_type
%type <defv>	default_value
%type <ival>	configuration_type

%%

swiftfox: library program 
		{
			/* verbose; to be removed */
			/* printTable(); */
			
			/* root node for the constructed AST */
			//struct program *p = $2;

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

program: global_variables defined_configurations defined_states policies initial_configuration 
		{
			/* root node */
			$$		= calloc(1, sizeof(struct program));
			
			/* link the node appropriately */
			$2->parent	= NULL;  
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


global_variables: global_variables global_variable
                {
			/* global_variable set */
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


global_variable: param_type IDENTIFIER array_part assign_value newlines 
		{
			$$		= calloc(1, sizeof(struct variable));
			$$->type 	= $1;
			$$->name	= $2;
			$$->length	= $3;
			$$->value	= $4;
		}

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


defined_configurations: configurations configuration
		{
			/* configurations set */
			$$		= calloc(1, sizeof(struct confnodes));
			
			/* link the child nodes */
			if ($1 != NULL)
				$1->parent = $$;
			$2->parent	= $$;
			
			$$->confs	= $1;
			$$->conf	= $2;
		}	
	;

configurations: configurations configuration
		{
			/* configurations set */
			$$		= calloc(1, sizeof(struct confnodes));
			
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

configuration: configuration_type IDENTIFIER conf_level OPEN_BRACE newlines module newlines module newlines module newlines module newlines CLOSE_BRACE newlines
		{

			/* configuration node */
			$$		= calloc(1, sizeof(struct confnode));

			/* init */
			$2->type	= $1;
			$$->id		= $2;
	
			/* level */
			$$->level	= $3;

			/* set ids */
			
			if ($6->id == 0) {
				$6->id = module_id_counter;
				//$6->conf = $$;
				++module_id_counter;
			}

			if ($8->id == 0) {
				$8->id = module_id_counter;
				//$8->conf = $$;
				++module_id_counter;
			}

			if ($10->id == 0) {
				$10->id = module_id_counter;
				//$10->conf = $$;
				++module_id_counter;
			}

			if ($12->id == 0) {
				$12->id = module_id_counter;
				//$12->conf = $$;
				++module_id_counter;
			}

			/* link application module */
			if (($6 == NULL) || (($6->type != TYPE_APPLICATION && $6->type != TYPE_EVENT))) {
				fprintf(stderr, "Undefined application module in %s\n", $2->name);
				yyerror("expecting application module");
			}
			$$->app			= $6;
			$$->app_params		= $6->params;
			$$->app->lib->used 	= 1;


			/* link network module */
			if (($8 == NULL) || ($8->type != TYPE_NETWORK)) {
				fprintf(stderr, "Undefined network module in %s\n", $2->name);
				yyerror("expecting network module");
			}
			$$->net			= $8;
			$$->net_params		= $8->params;
			$$->net->lib->used 	= 1;


			/* link mac module */
			if (($10 == NULL) || ($10->type != TYPE_MAC)) {
				fprintf(stderr, "Undefined mac module in %s\n", $2->name);
				yyerror("expecting mac module");
			}
			$$->mac			= $10;
			$$->mac_params		= $10->params;
			$$->mac->lib->used 	= 1;


			/* link radio module */
			if (($12 == NULL) || ($12->type != TYPE_RADIO)) {
				fprintf(stderr, "Undefined radio module in %s\n", $2->name);
				yyerror("expecting radio module");
			}
			$$->radio		= $12;
			$$->radio_params	= $12->params;
			$$->radio->lib->used 	= 1;

			$2->value	= conf_id_counter;

			if ($1 == TYPE_PROCESS_EVENT) {
				$2->value = event_id_counter;
				event_id_counter++;
			}


			$$->counter	= conf_id_counter;

			conftab[conf_id_counter].conf = $$;

			++conf_id_counter;

		}
	;


configuration_type: CONFIGURATION
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


conf_level: LEVEL CONSTANT
		{
			$$ = $2;
		}
        |       
                {
                        $$ = UNKNOWN;
                }                       
        ;



module: IDENTIFIER OPEN_PARENTHESIS parameters CLOSE_PARENTHESIS
		{
			$$ = proc_module($1->name);
			$$->params = $3;
		}		
	;

parameters: IDENTIFIER next_parameter
                {
                        struct symtab *sp = symlook($1->name);
                        if (sp == NULL)
                                yyerror("symtab pointer not found");

                        /* paramvalue node */
                        $$              = calloc(1, sizeof(struct paramvalue));
			$$->child	= $2;
                        $$->value       = $1;
			$$->num_value	= 0;
                }
        | CONSTANT next_parameter
                {
                        /* pramvalue node */
                        $$              = calloc(1, sizeof(struct paramvalue));
                        $$->child       = $2;
                        $$->value       = NULL;
                        $$->num_value	= $1;
                }
	|	
		{
                        $$ = NULL;
		}
        ;

next_parameter: COMMA IDENTIFIER next_parameter 
                {
                        struct symtab *sp = symlook($2->name);
                        if (sp == NULL)
                                yyerror("symtab pointer not found");

                        /* paramvalue node */
                        $$              = calloc(1, sizeof(struct paramvalue));
                        $$->child       = $3;
                        $$->value       = $2;
			$$->num_value	= 0;
                }
        | COMMA CONSTANT next_parameter
		{
                        /* pramvalue node */
                        $$              = calloc(1, sizeof(struct paramvalue));
                        $$->child       = $3;
                        $$->value       = NULL;
			$$->num_value	= $2;
		}
	|
                {
                        $$ = NULL;
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

			/* configuration node */
			$$		= calloc(1, sizeof(struct statenode));

			/* init */
			$2->type	= TYPE_STATE;
			$$->id		= $2;

			/* level */
			$$->level	= $3;


			$$->confs	= $6;

			if ($6 != NULL) {
				$$->confs_counter = $6->count;
			} else {
				$$->confs_counter = 0;
			}
	
			$$->counter	= state_id_counter;
			$2->value	= state_id_counter;
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
			if ($1 != NULL) { 
				$1->parent = $$;
				$$->count = $1->count + 1;
			} else {
				$$->count = 1;
			}
			$3->parent	= $$;

			$$->confs	= $1;
			$$->conf	= $3;

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

policy: FROM IDENTIFIER GOTO IDENTIFIER WHEN conf_ids newlines
		{
			/* policy node */
			$$ 		= calloc(1, sizeof(struct policy));

			/* link child nodes */
			$$->from	= $2;
			$$->to		= $4;

			$$->event_confs	= $6; 
			$$->counter	= policy_counter;

			if (proc_policy($$)) {
				fprintf(stderr, "Failed to store policy\n");
				exit(1);
			}
			++policy_counter;
		}


initial_configuration: START IDENTIFIER newlines
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
	;

definitions: definitions definition 
        |
        ;

definition: USE module_type IDENTIFIER PATH OPEN_PARENTHESIS newlines module_types CLOSE_PARENTHESIS
		{
			/* iterator */
			char *p = NULL;
			
			/* lookup */
			struct symtab *sp = NULL;
			char *full_name = NULL;
		
			/* check for library re-declarations */
			if ((sp = symlook($3->name)) != NULL && sp->type != TYPE_UNKNOWN)
				/* failed */
				yyerror("redeclaration of library");

			/* fix the child properties */
			$3->type = $2;
			$3->lib = $4;

			/* set params for the library */
			$4->params = $7;

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
			        full_name = malloc(strlen($4->name)+strlen("App")+1);
			        sprintf(full_name, "%sApp", $4->name);
				$4->full_name = full_name;
				$4->type = TYPE_APPLICATION;
				$4->used = 0;
				$4->id = 0;
				break;

			case TYPE_EVENT:
			        full_name = malloc(strlen($4->name)+strlen("App")+1);
			        sprintf(full_name, "%sApp", $4->name);
				$4->full_name = full_name;
				$4->type = TYPE_EVENT;
				$4->used = 0;
				$4->id = 0;
				break;

			case TYPE_NETWORK:
				/* network */
			        full_name = malloc(strlen($4->name)+strlen("Net")+1);
			        sprintf(full_name, "%sNet", $4->name);
				$4->full_name = full_name;
				$4->type = TYPE_NETWORK;
				$4->used = 0;
				$4->id = 0;
				break;

			case TYPE_MAC:
                                /* mac */
			        full_name = malloc(strlen($4->name)+strlen("Mac")+1);
			        sprintf(full_name, "%sMac", $4->name);
				$4->full_name = full_name;
                                $4->type = TYPE_MAC;
				$4->used = 0;
                                $4->id = 0;
				break;
                        
			case TYPE_RADIO:
                                /* radio */
			        full_name = malloc(strlen($4->name)+strlen("Radio")+1);
			        sprintf(full_name, "%sRadio", $4->name);
				$4->full_name = full_name;
                                $4->type = TYPE_RADIO;
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
        | MAC 			{ $$ = TYPE_MAC; }
        | RADIO 		{ $$ = TYPE_RADIO; }
	| SOURCE 		{ $$ = TYPE_EVENT; }
	|			{ $$ = TYPE_UNKNOWN; }
        ;


module_types: param_type IDENTIFIER default_value next_module_type
		{
			struct symtab *sp = symlook($2->name);
			if (sp == NULL)
                                yyerror("symtab pointer not found");


                        $$              = calloc(1, sizeof(struct paramtype));
			$$->type	= $1;
                        $$->name   	= $2->name;
			$$->def_val	= $3;
                        $$->child       = $4;
		}
	|
		{
			$$ = NULL;
		}
	;


default_value: RELOP CONSTANT
		{
			$$		= calloc(1, sizeof(struct defvalue));
			$$->def_value	= $2;
			$$->def_valid	= 1;
		}
	|
		{
			$$		= calloc(1, sizeof(struct defvalue));
			$$->def_value	= 0;
			$$->def_valid	= 0;
		}
	;


next_module_type: newlines COMMA newlines param_type IDENTIFIER default_value next_module_type
		{
			struct symtab *sp = symlook($5->name);
			if (sp == NULL)
                                yyerror("symtab pointer not found");

                        $$              = calloc(1, sizeof(struct paramtype));
                        $$->type   	= $4;
                        $$->name   	= $5->name;
			$$->def_val	= $6;
			$$->child	= $7;
		}
	|
		{
			$$ = NULL;
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
start_parser(int argc, char *argv[]) {

	/* check the file extention */
	if (rindex(argv[1], '.') != NULL && !strcmp(rindex(argv[1], '.'), ".sfp")) {
		argv[1][strlen(argv[1])-4] = '\0';
	}
		
	/* init */
	(void)memset(program_file, 0, PATH_SZ);
	(void)memset(library_file, 0, PATH_SZ);
	(void)memset(fennec_library_file, 0, PATH_SZ);

	(void)snprintf(program_file, PATH_SZ, "%s.sfp", argv[1]);
	(void)snprintf(library_file, PATH_SZ, "%s.sfl", argv[1]);
	(void)snprintf(fennec_library_file, PATH_SZ, "%s/%s", 
			getenv("FENNEC_FOX_LIB"), STD_FENNEC_FOX_LIB);


	if ((error_location = malloc(strlen("fennec_library") + 1)) == NULL) {
		/* failed */
		(void)fprintf(stderr, 
			"Cannot allocate memory for error_location\n");
		exit(1);
	}

	/* cleanup */	
	(void)atexit(gc);

	initialize();
	
	/* process libraries */
	lineno = 1;
	tokenpos = 0;
	
	/* try fennec fox standart libray located at ($FENNEC_FOX_LIB)/STD_FENNEC_FOX_LIB */
	yyin = fopen(fennec_library_file, "r");
	strcpy(error_location, "fennec library");
	file_status = 1;
	file_name = fennec_library_file;

	if (!yyin) {
		/* failed */
		(void)fprintf(stderr, 
			"%s.sfl: no such file or directory and no standard library\n",
			argv[1]);
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
yyerror(char *errmsg) {
	
	/* error in program */
	(void)fprintf(stderr, "\nsfc in %s %s\n", error_location, file_name);

	/* line */
        (void)fprintf(stderr, "%s at line %d, position %d:\n", errmsg, lineno, 
							tokenpos - yyleng + 1);
	(void)fprintf(stderr, "%s\n", linebuf);
	(void)fprintf(stderr, "%*s\n", tokenpos - yyleng + 1, "^");

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
			lineno		= 1;
			tokenpos	= 0;
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
			lineno		= 1;
			tokenpos	= 0;
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

/* process_module */
struct modtab *
proc_module(char *s) {
	char * c;

	/* symtab ptr */
	struct symtab *sp = find_module_symtab(s);

        /* iterator */
        struct modtab *mp = NULL;

	/* check if this is a know module */
	if (sp == NULL) {
		yyerror("no such module");
	}

        /* loop */
        for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
                /* is it free */
                if(!mp->lib) {
			mp->id = 0;
			mp->type = sp->type;
			mp->lib = sp->lib;
			mp->params = NULL;
			mp->name = strdup(sp->lib->full_name);
			for(c = mp->name; *c != '\0'; c++ ) {
				*c = toupper(*c);
			}
                        return mp;
                }
		if (strcmp(mp->lib->def, sp->name) == 0) {
			return mp;
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

/* finds configuration record based on its string ID */
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
			checkForRemotePath(lp);
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
	char *keywords[] = {"configuration", "start", "use", "application",
			"network", "source", "event-condition", "from", "goto",
			"conf", "event", "on", "off", 			
			"mac", "radio", // new keywords
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

/* garbage collection */
void
gc(void) {
	
	/* cleanup */
	(void)fclose(yyin);
}

/* check if the path is remote */
void
checkForRemotePath(struct libtab *lp) {

        /* check if it is http:// */
        if (!strncmp(lp->path, "http://",7)) {

                /* make temp dir for all downloaded libraries */
                mkdir(TEMP_DIR, S_IRWXU);

                char *p = NULL;
                /* extract the name from the path */
                if ((p = rindex(lp->path, '/')) != NULL) {
                        p++;
                }

                /* make dir for this library */
                char *new = malloc(strlen(p)+strlen(TEMP_DIR)+2);
                sprintf(new, "%s/%s", TEMP_DIR, p);
                mkdir(new, S_IRWXU);

                /* prepare system command */
                char *command = malloc(strlen(lp->path)+strlen(new)+40);
                sprintf(command, "wget -q -nd -r --no-parent -A.nc -P %s %s", new, lp->path);

                if( system(command) ) {
                        fprintf(stderr, "Swift Fox Error: Could not download files from %s\n", lp->path);
                        exit(1);
                }
                free(lp->path);
                lp->path = strdup(new);
                free(command);
                free(new);
        }
}

