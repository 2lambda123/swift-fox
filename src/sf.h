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


#ifndef SF_H
#define SF_H

#define TREE_TRAVERSE 		1
#define TREE_CHECK_SEMANTIC 	2 
#define TREE_GENERATE_CODE 	3

#define NSYMS 			256
#define NMODS 			256
#define NLIBS 			256
#define NEVS 			256
#define NCONFS			256
#define NSTATES			256
#define NPOLS			256
#define NVARS			256

#define TYPE_UNKNOWN		0

#define TYPE_APPLICATION	1
#define TYPE_NETWORK		2
#define TYPE_EVENT		3
#define TYPE_AM			4

#define TYPE_TYPE		8

#define TYPE_CONFIGURATION	9
#define TYPE_PROCESS		10
#define TYPE_PROCESS_REGULAR	11
#define TYPE_PROCESS_EVENT	12
#define TYPE_STATE		13
#define TYPE_KEYWORD		14
#define TYPE_VARIABLE_GLOBAL	15
#define TYPE_VARIABLE_LOCAL	16
#define TYPE_VARIABLE_DEFAULT	17
#define TYPE_VARIABLE_SHARED	18

#define TYPE_BOOL		19
#define TYPE_UINT8_T		20
#define TYPE_UINT16_T		21
#define TYPE_UINT32_T		22
#define TYPE_FLOAT		23
#define TYPE_DOUBLE		24
#define TYPE_INT8_T		25
#define TYPE_INT16_T		26
#define TYPE_INT32_T		27
#define TYPE_NXUINT8_T		28
#define TYPE_NXUINT16_T		29
#define TYPE_NXUINT32_T		30

#define PATH_SZ			256

#define SEC_CONV		1024
#define MIN_CONV		60 * SEC_CONV
#define HR_CONV			60 * MIN_CONV

#define NUMBER_OF_FF_MODULES	7

#define BUF_SZ			1024

#define TEMP_DIR                "/tmp/swift_fox"

#define STD_FENNEC_FOX_LIB	"support/sfc/fennec.sfl"

#define UNKNOWN			-1

extern char *ffmodules;
extern int active_state;

extern int state_id_counter;    
extern int module_id_counter;
extern int event_id_counter;
extern int conf_id_counter;
extern int state_defined;
extern int unique_variable_id;

extern int policy_counter;

extern int variable_memory_offset;
extern int global_memory_size;
extern int shared_memory_size;

extern char *conf_state_suffix;

extern int sfc_debug;
extern int variable_cache;

int start_parser(char *pfile, char *lfile);

extern int yylineno;
extern int yycolumn;

char linebuf[BUF_SZ];

extern int generate_globals;

extern int adjust_global_offset;
extern int adjust_shared_offset;
extern int number_of_variables_in_cache;

/**
Symbol Table 
stores all symbols passed from lexer
*/
struct symtab {
	char			*name;
	int			value;
	int			type;
	struct libtab		*lib;
} symtab[NSYMS];

struct symtab *symlook(char *);
struct symtab *find_module_symtab(char *);

struct defvalue {
	long double		def_value;
	int 			def_valid;
};

struct libtab {
        char			*path;
	char			*def;
	char			*name;
	struct variables	*variables;
	int			used;
	int			type;
	int			id;
} libtab[NLIBS];

struct libtab *liblook(char *);

struct evtab {
	char			*name;
	int			num;
	int			op;	
	int			value;
	int			addr;
} evtab[NEVS];

struct evtab *evlook(char *);

struct variable {
	/** pointer to the structure with parent variables */
	struct variables	*parent;
	/** type of a variable encoded as intiger */
	int			type;
	/** memory offset in bytes */
	int			offset;
	/** pointer to a symtab containing the variable's name */
	char			*name;
	/** full varibale name, per process, per module */
	char			*cap_name;
	/** name if the variable points to global */
	char			*gname;
	/** lenght of the variable - use for arrays */
	int			length;
	/* default variable value */
	long double		value;
	/* variable initialized */
	int			init;
	/* variable type */
	int			class_type;
	/* variable id */
	int			id;
	/* is variable used */
	int			used;
	/* original global */
	int			global;
} vartab[NVARS];


struct variables {
	/** pointer to previous variable */
	struct variables	*parent;	
	/** pointer to further variable */
	struct variables	*vars;
	/** pointer to the variable structure */
	struct variable		*var;
};

struct variable *find_variable(char *varname);

struct modtab {
	char			*name;
        int            		type;
	int			id;
	char			*id_name;
	int			duplicate;
        struct libtab   	*lib;
	struct variables	*variables;
} modtab[NMODS];

struct modtab *proc_module(char *);

struct confnode {
	struct processnodes	*parent;
	struct symtab		*id;
	char			*name;
	struct modtab		*app;
	struct modtab		*net;
	struct modtab		*am;
	int			app_var_num;
	int			*app_var_name;
	int			app_var_offset;
	int			net_var_num;
	int			*net_var_name;
	int			net_var_offset;
	int			am_var_num;
	int			*am_var_name;
	int			am_var_offset;
	char			*app_id_name;
	char			*net_id_name;
	char			*am_id_name;
	int			am_dominant;			
	int			counter;
	char			*id_name;
	int			daemon;
};

struct conftab {
        struct confnode		*conf;
} conftab[NCONFS];


struct processnodes {
	struct processnodes	*parent;
	struct processnodes	*confs;
	struct confnode		*conf;
};

struct confnode *conflook(struct symtab *conf_id);


struct conf_id {
	struct symtab		*id;
	struct confnode		*conf;
};

struct conf_ids {
	struct conf_ids		*confs;
	struct conf_id		*conf;
};

struct statenode {
	struct statenodes	*parent;
	struct symtab		*id;
	struct conf_ids		*confs;
	char			*id_name;
	int			counter;
	int 			level;
};

struct statetab {
        struct statenode	*state;
} statetab[NSTATES];


struct statenodes {
	struct statenodes	*parent;
	struct statenodes	*states;
	struct statenode	*state;
};


struct policy {
	/** pointer to parent policy */
	struct policies		*parent;
	/** pointer to a symtab with configuration that is the source of reconfiguration */
	struct symtab		*from;
	/** pointer to a symtab with configuration that is the destination of reconfiguration */
	struct symtab		*to;
	/** points to configurations that process events */
	struct conf_ids		*event_confs;
	/** policy counter, and this policy number */
	int 			counter;
	/** light/fast switch policy */
	int			fast;
};

struct policies {
	/** pointer to parent policies */
	struct policies		*parent;
	/** pointer to further policies */
	struct policies		*pols;
	/** pointer to a single policy structure */
	struct policy 		*pol;
};

struct initnode {
	struct symtab 		*id;
	int 			init;
};

struct program {
	struct variables	*vars;
	struct processnodes	*defcon;
	struct statenodes	*defstate;
	struct policies		*defpol;
	struct initnode		*init;
};


struct poltab {
	struct policy		*policy;
} poltab[NPOLS];

int proc_policy(struct policy *p);

#endif
