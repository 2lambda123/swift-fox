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
		//printf("0x%1x 0x%1x\n", statetab[i].state, s);
		if ((statetab[i].state != s) && (statetab[i].state->id == s->id)) {
			goto state_err;
		}
	}


	/* check if every configuration is defined */
	for (i = 0; i < state_id_counter; i++) {
		for (conf_ptr = statetab[i].state->confs; conf_ptr != NULL; conf_ptr = conf_ptr->confs) {
			if (conf_ptr->conf->id->type == TYPE_UNKNOWN) {
				goto undef_conf;
			}
		}
	}


	/* OK, no problems found */
	return;


state_err:
	/* redeclaration */
	(void)fprintf(stderr, "error: redeclaration of state %s\n",
			s->id->name);
	/* terminate */
	exit(1);



undef_conf:
	/* undefined configuration */
	(void)fprintf(stderr, "error: undefined configuration %s in state %s\n",
			conf_ptr->conf->id->name, statetab[i].state->id->name);
	/* terminate */
	exit(1);
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
	check for undeclared mac 
	*/
	if (c->mac == NULL || c->mac->type == TYPE_UNKNOWN || c->mac->lib == NULL)
		goto mac_err;

        /* check for undeclared mac */
        if (c->mac->type == TYPE_KEYWORD) {
                /* loop */
                found = 0;
                for (sp = symtab; sp < &symtab[NSYMS]; sp++)
                        if (sp->name &&
                                !strcmp(sp->name, c->mac->lib->def))
                                if (sp->type && (sp->type == TYPE_MAC)) {
                                        /* found */
                                        found = 1;
                                        break;
                                }

                /* undeclared mac */
                if (!found)
                        goto mac_err;
        }

	/** 
	check for undeclared network 
	*/
	if (c->radio == NULL || c->radio->type == TYPE_UNKNOWN || c->radio->lib == NULL)
		goto radio_err;

        /* check for undeclared radio */
        if (c->radio->type == TYPE_KEYWORD) {
		if (c->radio->lib == NULL)
			goto radio_err;

                /* loop */
                found = 0;
                for (sp = symtab; sp < &symtab[NSYMS]; sp++)
                        if (sp->name &&
                                !strcmp(sp->name, c->radio->lib->def))
                                if (sp->type && (sp->type == TYPE_RADIO)) {
                                        /* found */
                                        found = 1;
                                        break;
                                }

                /* undeclared radio */
                if (!found)
                        goto radio_err;
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

mac_err:
	(void)fprintf(stderr, "error: undeclared mac in configuration %s\n",
			c->id->name);

        /* terminate */
        exit(1);

radio_err:
	(void)fprintf(stderr, "error: undeclared radio in configuration %s\n",
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
	check mac module params 
	*/
	checkSingleModule(c, c->mac, &(c->mac_params), c->mac->lib->params);

	/** 
	check radio module params 
	*/
	checkSingleModule(c, c->radio, &(c->radio_params), c->radio->lib->params);
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
			statetab[i].state->confs_counter = statetab[i].state->confs_counter + 
						p->event_confs->count;
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
	cis->count = 1;
	cis->confs = NULL;
	cis->conf = ci;

	/* create state entry */
	st->type = TYPE_STATE;
	newstate->id = st;
	newstate->level = UNKNOWN;
	newstate->confs = cis;
	newstate->confs_counter = 1;
	newstate->counter = state_id_counter;
	st->value = state_id_counter;
	++state_id_counter;

	statetab[newstate->counter].state = newstate;
}
