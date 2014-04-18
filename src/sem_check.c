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


#include "code_gen.h"
#include "sem_check.h"

/** 
map configurations to names 
*/
struct symtab *map_conf_id[NSYMS];

/**
 map event-conditions to names 
*/
struct symtab *map_evt_id[NSYMS];

/**
zero map_conf_id 
*/
void
init_sem_conf(void) {
	/* clear */
	(void)memset(map_conf_id,
		0,
		sizeof(struct symtab *) * NSYMS);
}

/** 
zero map_evt_id 
*/
void
init_sem_evt(void) {
	/* clear */
	(void)memset(map_evt_id,
		0,
		sizeof(struct symtab *) * NSYMS);
}


void
checkState(struct statenode *s) {
	int i;
//	struct statenode *state;
//	struct statetab		*stateptr;
	struct conf_ids *conf_ptr;

	/*
	check for redclarations
	*/
	
	for (i = 0; i < state_id_counter; i++) {
		if ((statetab[i].state != s) && (statetab[i].state->id == s->id)) {
			/* redeclaration */
			(void)fprintf(stderr, "error: redeclaration of state %s\n",
				s->id->name);
			/* terminate */
			exit(1);
		}
	}


	/* check if every configuration is defined */
	for (conf_ptr = s->confs; conf_ptr != NULL; conf_ptr = conf_ptr->confs) {
		if (conf_ptr->conf->id->type == TYPE_UNKNOWN) {
			/* undefined configuration */
			(void)fprintf(stderr, "error: undefined configuration %s in state %s\n",
				conf_ptr->conf->id->name, s->id->name);
			/* terminate */
			exit(1);
		}
	}

	/* check number of dominant AM modules in the state */
	for (conf_ptr = s->confs; conf_ptr != NULL; conf_ptr = conf_ptr->confs) {
		int not_inferior = 0;
		struct conf_ids *cp;
		for(cp = conf_ptr; cp != NULL; cp = cp->confs) {
			if (conf_ptr->conf->conf->am->lib == cp->conf->conf->am->lib) {
				not_inferior += !(cp->conf->conf->am_inferior);
			}

			if (not_inferior > 1) {
				/* ambigious match of AM in the state */
				(void)fprintf(stderr, "error: too many dominant AM modules %s in the state %s\n",
					conf_ptr->conf->conf->am->lib->name, s->id->name);
				/* terminate */
				exit(1);
			}
		}
	}

	/* OK, no problems found */
	return;
}


/** 
semantic checks for the configuration nodes 
/param c pointer to confnode
*/
void
checkConfiguration(struct confnode* c) {
	/* iterators */
	int i			= 0;
	struct symtab *sp	= NULL;

	/* flag */
	int found		= 0;

	
	/** 
	traverse the previous configurations 
	*/
	while ((i < NSYMS - 1) && (map_conf_id[i] != NULL)) {
		/** 
		check for redeclaration 
		*/
		if (map_conf_id[i] == c->id)
			goto conf_err;
		/* next */
		i++;
	}

	/** 
	generate the mapping between name and configuration 
	*/
	map_conf_id[i] = c->id;
	
	/** 
	check for undeclared application 
	*/
	if (c->app == NULL || c->app->type == TYPE_UNKNOWN || c->app->lib == NULL)
		goto app_err;

	if (c->app->type == TYPE_KEYWORD) {
		/* loop */
		found = 0;
		for (sp = symtab; sp < &symtab[NSYMS]; sp++) 
			if (sp->name &&
				!strcmp(sp->name, c->app->lib->def)) 
				if (sp->type && ( (sp->type == TYPE_APPLICATION) ||
					(sp->type == TYPE_EVENT))) {
					/* found */
					found = 1;
					break;
				}
		
		/* undeclared application */
		if (!found)
			goto app_err;
	}
	
	/** 
	check for undeclared network 
	*/
	if (c->net == NULL || c->net->type == TYPE_UNKNOWN || c->net->lib == NULL)
		goto net_err;
	
	if (c->net->type == TYPE_KEYWORD) {
		if (c->net->lib == NULL)
			goto net_err;
		/* loop */
		found = 0;
		for (sp = symtab; sp < &symtab[NSYMS]; sp++)
			if (sp->name &&
				!strcmp(sp->name, c->net->lib->def))
				if (sp->type && (sp->type == TYPE_NETWORK)) {
					/* found */
					found = 1;
					break;
				}

		/* undeclared network */
		if (!found)
			goto net_err;
	}
	
	/** 
	check for undeclared am 
	*/
	if (c->am == NULL || c->am->type == TYPE_UNKNOWN || c->am->lib == NULL)
		goto am_err;

        /* check for undeclared am */
        if (c->am->type == TYPE_KEYWORD) {
                /* loop */
                found = 0;
                for (sp = symtab; sp < &symtab[NSYMS]; sp++)
                        if (sp->name &&
                                !strcmp(sp->name, c->am->lib->def))
                                if (sp->type && (sp->type == TYPE_AM)) {
                                        /* found */
                                        found = 1;
                                        break;
                                }

                /* undeclared am */
                if (!found)
                        goto am_err;
        }

	/* check if states are defined */

	if (state_defined == 0) {
		addConfState(c);
	}

        /* success */
        return;

conf_err:
	/* redeclaration */
	(void)fprintf(stderr, "error: redeclaration of configuration %s\n",
			c->id->name);
	/* terminate */
	exit(1);

app_err:
	(void)fprintf(stderr, "error: undeclared application in configuration %s\n",
			c->id->name);

	/* terminate */
	exit(1);

net_err:
	(void)fprintf(stderr, "error: undeclared network in configuration %s\n",
			c->id->name);
	/* terminate */
	exit(1);

am_err:
	(void)fprintf(stderr, "error: undeclared am in configuration %s\n",
			c->id->name);

        /* terminate */
        exit(1);
}

/**
checks if a module is valid

/param c pointer to configuration that has this module
/param mp pointer to module structure modtab
/param mod_par_val pointer to structure with module parameter values
/param par_type pointer to structure with module parameter tyles
*/
void
checkSingleModule(struct confnode *c, struct modtab *mp, 
		struct paramvalue **mod_par_val, struct paramtype *par_type) {

        /**
	check if there are too many parameters 
	*/
        for(; *mod_par_val != NULL; mod_par_val = &(*mod_par_val)->child) {
                /* check if there are more passed parameters then
                 * there are defined variables 
                 */
                if ((par_type == NULL) && (*mod_par_val != NULL)) {
		        (void)fprintf(stderr, "error: Too many parameters in "
			"configuration %s in module %s defined as %s\n",
        	                c->id->name, mp->lib->name, mp->lib->def);
			/* terminate */
			exit(1);
                        break;
                }

                if (par_type != NULL)
                        par_type = par_type->child;
        }

        /** 
	check if parameters are missing 
	*/
        for (; (*mod_par_val == NULL) && (par_type != NULL); 
		par_type = par_type->child, mod_par_val = &(*mod_par_val)->child) {
                if (par_type->def_val->def_valid) {
			/* TODO: add warning message? */
			//printf("adding default param\n");
			/* extend module's parameter list with the 
			 * default values */
			*mod_par_val = calloc(1, sizeof(struct paramvalue));
			(*mod_par_val)->num_value = par_type->def_val->def_value;
			(*mod_par_val)->child = NULL;
                } else {
			/* parameters are missing and there are no default
			 * values defined to fill the missing ones */
		        (void)fprintf(stderr, "error: Missing parameters in "
			"configuration %s in module %s defined as %s - default "
					"values are not defined\n",
        	                c->id->name, mp->lib->name, mp->lib->def);
			/* terminate */
			exit(1);
                }
        }
}

/** 
semantic check for modules passed to configuration 

/param c pointer to configuration node
*/
void 
checkConfigurationModules(struct confnode *c) {

	/** 
	check application module params 
	*/
	checkSingleModule(c, c->app, &(c->app_params), c->app->lib->params);

	/** 
	check network module params 
	*/
	checkSingleModule(c, c->net, &(c->net_params), c->net->lib->params);

	/** 
	check am module params 
	*/
	checkSingleModule(c, c->am, &(c->am_params), c->am->lib->params);
}

/** 
semantic checks for the policy nodes 

/param p pointer to policy node
*/
void
checkPolicy(struct policy *p) {
	/* iterator */
	struct symtab *sp	= NULL;

	/* flag */
	int found		= 0;
	
	/** 
	check the __from__ state 
	*/
	if (strcmp(p->from->name, "any")) {	
		/* loop */
		found = 0;
		for (sp = symtab; sp < &symtab[NSYMS]; sp++)
			if (sp->name &&
				!strcmp(sp->name, p->from->name))
				if (sp->type && (sp->type == TYPE_STATE)) {
					/* found */
					found = 1;
					break;
				}
	
		/* undeclared from */
		if (!found)
			goto from_conf_err;
	}
	
	/* loop */
	found = 0;
	for (sp = symtab; sp < &symtab[NSYMS]; sp++)
		if (sp->name &&
			!strcmp(sp->name, p->to->name))
			if (sp->type && (sp->type == TYPE_STATE)) {
				/* found */
				found = 1;
				break;
			}

	/* check the __to__ state */
	if (!found)
		goto to_conf_err;
	
	/* success */	
	return;

from_conf_err:
	/* undeclared __from__  state */
	(void)fprintf(stderr, "error: undeclared 'from' state %s\n",
			p->from->name);
	/* terminate */
	exit(1);

to_conf_err:
	/* undeclared __to__ state */
	(void)fprintf(stderr, "error: undeclared 'to' state %s\n",
			p->to->name);
	/* terminate */
	exit(1);
}

void
updateStatesWithEvents(struct policy *p) {
	int i;
	struct conf_ids *cids;

	for (i = 0; i < state_id_counter; i++) {
		/** 
		if we have 'any' policy then append configurtion to any state 
		*/
		if (!(strcmp(p->from->name, "any")) || 
			!(strcmp(p->from->name, statetab[i].state->id->name))) {

			/* find the end of the conf_ids list */
			cids = statetab[i].state->confs;

			if (cids == NULL) {
				statetab[i].state->confs = p->event_confs;
			} else {
				while(cids->confs != NULL) {
					cids = cids->confs;
				}
				cids->confs = p->event_confs;
			}
		}
	}	
}


/** 
semantic checks for the initial node 

/param i pointer to initnode with the initial state
*/
void
checkInitial(struct initnode *i) {

	/* iterator */
	struct symtab *sp	= NULL;
	
	/* loop */
	for (sp = symtab; sp < &symtab[NSYMS]; sp++) {
		if (sp->name && !strcmp(sp->name, i->id->name)) {
			if (sp->type && (sp->type == TYPE_STATE)) {
				/* found */
				return;
			}
			if ((state_defined == 0) && sp->type &&
				(sp->type == TYPE_PROCESS_REGULAR)) {
				/* found */
				return;
			}
		}
	}
	

	/* undeclared initial configuration */
	(void)fprintf(stderr, "error: undeclared 'initial' state %s\n",
			i->id->name);
	/* terminate */
	exit(1);
}

/**
adds configuration module

/param c pointer to configuration node
/param m pointer to a pointer to module structure
/param p pointer to a pointer to a parameter value structue
/param module_name string with module name
*/

void
addConfModule(struct confnode *c, struct modtab **m, struct paramvalue **p, char *module_name ) {
}

/**
checks control state
*/
void
checkControlState(void) {
}



void
addConfState(struct confnode *c) {

	struct statenode *newstate = calloc(1, sizeof(struct statenode));
	struct conf_id *ci = calloc(1, sizeof(struct conf_id));
	struct conf_ids *cis = calloc(1, sizeof(struct conf_ids));
	char *state_name = calloc(1, sizeof(c->id->name) + strlen(conf_state_suffix) + 2);

	if ((newstate == NULL) || (cis == NULL) || (ci == NULL) ||
						(state_name == NULL)) {
		fprintf(stderr, "addConfState: malloc failed\n");
		exit(1);
	}

	sprintf(state_name, "%s%s", c->id->name, conf_state_suffix);
	struct symtab *st = symlook(state_name);

	if (st == NULL) {
		fprintf(stderr, "addConfState: symlook failed\n");
		exit(1);
	}


	/* create conf_id */
	ci->conf = c;
	ci->id = c->id;

	/* create conf_ids with one conf_id */
	cis->confs = NULL;
	cis->conf = ci;

	/* create state entry */
	st->type = TYPE_STATE;
	newstate->id = st;
	newstate->level = UNKNOWN;
	newstate->confs = cis;
	newstate->counter = state_id_counter;
	++state_id_counter;

	statetab[newstate->counter].state = newstate;
}
