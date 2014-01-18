/** \file
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

#define TYPE_UNKNOWN		0

#define TYPE_APPLICATION	1
#define TYPE_NETWORK		2
#define TYPE_EVENT		3
#define TYPE_MAC		5
#define TYPE_RADIO		6

#define TYPE_TYPE		8

#define TYPE_CONFIGURATION	9
#define TYPE_PROCESS		10
#define TYPE_PROCESS_REGULAR	11
#define TYPE_PROCESS_EVENT	12
#define TYPE_STATE		13
#define TYPE_KEYWORD		14
#define TYPE_VARIABLE_GLOBAL	15

#define TYPE_UINT8_T		20
#define TYPE_UINT16_T		21
#define TYPE_UINT32_T		22
#define TYPE_FLOAT		23
#define TYPE_DOUBLE		24

#define PATH_SZ			256
#define ARGC_MAX		2

#define SEC_CONV		1024
#define MIN_CONV		60 * SEC_CONV
#define HR_CONV			60 * MIN_CONV

#define NUMBER_OF_FF_MODULES	7

#define BUF_SZ			1024

#define TEMP_DIR                "/tmp/swift_fox"

#define STD_FENNEC_FOX_LIB	"support/sfc/fennec.sfl"

#define F_APPLICATION		1
#define F_NETWORK		2
#define F_MAC			3
#define F_RADIO			4
#define F_LAYERS		4


#define UNKNOWN			-1

extern char *ffmodules;
extern int active_state;

extern int state_id_counter;    
extern int module_id_counter;
extern int event_id_counter;
extern int conf_id_counter;
extern int state_defined;

extern char *conf_state_suffix;

int start_parser(int argc, char *argv[]);

int lineno;
char linebuf[BUF_SZ];
int tokenpos;

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
	double			def_value;
	int 			def_valid;
};

struct paramtype {
        struct paramtype        *child;
	int			type;
	struct defvalue		*def_val;
        char                    *name;
};

struct paramvalue {
	struct paramvalue	*child;	
	struct symtab		*value;
	double			num_value;
};

struct libtab {
        char			*path;
	char			*def;
	char			*name;
	struct paramtype	*params;
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

struct modtab {
	char			*name;
        int            		type;
        struct libtab   	*lib;
	struct paramvalue	*params;
} modtab[NMODS];

struct modtab *proc_module(char *);

struct confnode {
	struct confnodes	*parent;
	struct symtab		*id;
	char			*name;
	struct modtab		*app;
	struct modtab		*net;
	struct modtab		*mac;
	struct modtab		*radio;
	struct paramvalue	*app_params;
	struct paramvalue	*net_params;
	struct paramvalue	*mac_params;
	struct paramvalue	*radio_params;
	char			*app_id_name;
	int			app_id_value;
	char			*net_id_name;
	int			net_id_value;
	char			*mac_id_name;
	int			mac_id_value;
	char			*radio_id_name;
	int			radio_id_value;
	int			counter;
	int 			level;
};

struct conftab {
        struct confnode		*conf;
} conftab[NCONFS];


struct confnodes {
	struct confnodes	*parent;
	struct confnodes	*confs;
	struct confnode		*conf;
};

struct confnode *conflook(struct symtab *conf_id);


struct conf_id {
	struct conf_ids		*parent;
	struct symtab		*id;
	struct confnode		*conf;
};

struct conf_ids {
	struct conf_ids		*parent;
	struct conf_ids		*confs;
	struct conf_id		*conf;
	int 			count;
};

struct statenode {
	struct statenodes	*parent;
	struct symtab		*id;
	struct conf_ids		*confs;
	int			counter;
	int 			level;
	int 			confs_counter;
};

struct statetab {
        struct statenode	*state;
} statetab[NSTATES];


struct statenodes {
	struct statenodes	*parent;
	struct statenodes	*states;
	struct statenode	*state;
};


struct variable {
	/** pointer to the structure with parent variables */
	struct variables	*parent;
	/** type of a variable encoded as intiger */
	int			type;
	/** pointer to a symtab containing the variable's name */
	struct symtab		*name;
	/** lenght of the variable - use for arrays */
	int			length;
	/* default variable value */
	double			value;
};


struct variables {
	/** pointer to previous variable */
	struct variables	*parent;	
	/** pointer to further variable */
	struct variables	*vars;
	/** pointer to the variable structure */
	struct variable		*var;
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
	struct confnodes	*defcon;
	struct statenodes	*defstate;
	struct policies		*defpol;
	struct initnode		*init;
};


struct poltab {
	struct policy		*policy;
} poltab[NPOLS];

int proc_policy(struct policy *p);

#endif
