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
#include <string.h>
#include "traverse.h"
#include "sem_check.h"
#include "code_gen.h"
#include "utils.h"
#include "dbg_utils.h"

struct modtab *current_module_gen = NULL;
struct confnode *current_process_gen = NULL;

/** 
AST (Abstract Syntax Tree) traversal; 
main entry point 

\param p pointer to a program structure 

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)

\param policy_counter number of policies
*/
void
traverse_program(struct program* p, int f, int policy_counter){
	if (p != NULL) {
		switch (f) {
			/** 
			plain traversal 
			*/
			case TREE_TRAVERSE:
				if (sfc_debug) {
					printf("traverse: first pass (%d)\n", f);
				}
				traverse_processnodes(p->defcon, f);
				traverse_statenodes(p->defstate, f);
				traverse_policies(p->defpol, f);
				traverse_initnode(p->init, f);
				break;
			/** 
			traversal for semantic checking 
			*/
			case TREE_CHECK_SEMANTIC:
				if (sfc_debug) {
					printf("traverse: check seminatic (%d)\n", f);
				}
				/** 
				zero the mapping structure 
				*/
				init_sem_conf();
				/** 
				zero the mapping structure 
				*/
				init_sem_evt();

				traverse_variables(p->vars, f);

				if (adjust_global_offset > 0) {
					print_variables(TYPE_VARIABLE_GLOBAL);
				}

				if (adjust_shared_offset > 0) {
					print_variables(TYPE_VARIABLE_SHARED);
				}
		
				if (sfc_debug) {
					printf("traverse processnodes\n");
				}
				traverse_processnodes(p->defcon, f);

				if (sfc_debug) {
					printf("traverse statenodes\n");
				}
				traverse_statenodes(p->defstate, f);

				if (sfc_debug) {
					printf("traverse policies\n");
				}
				traverse_policies(p->defpol, f);

				if (sfc_debug) {
					printf("traverse initnode\n");
				}
				traverse_initnode(p->init, f);

				if (sfc_debug) {
					printf("traverse setUniqueVariableIDs\n");
				}
				setUniqueVariableIDs();
				break;

			/**
			traversal for code generation 
			*/
			case TREE_GENERATE_CODE:
				if (sfc_debug) {
					printf("traverse: generate code (%d)\n", f);
				}
				setFiles();

				initCodeGeneration();

				initGlobalDataValues();
				initLocalDataValues();
				initSharedDataValues();

				initGlobalDataH();
				initSharedDataH();
				traverse_variables(p->vars, f);
				finishGlobalDataH();
				finishSharedDataH();
				globalDataMsgH();

				switchGlobalToLocalDataStorage();

				initLocalDataH();
				traverse_processnodes(p->defcon, f);
				setProcessesLookupTable();
				finishLocalDataH();

				traverse_statenodes(p->defstate, f);
				traverse_policies(p->defpol, f);
				traverse_initnode(p->init, f);

				finishGlobalDataValues();
				finishLocalDataValues();
				finishSharedDataValues();

				finishCodeGeneration(policy_counter);
				break;
			/** 
			make the compiler happy 
			*/
			default:
				printf("traverse: UNNOWN STATE (%d)\n", f);
				break;
		}
	}
}

/** 
shared variables traversal 

\param sh pointer to header of a list of variables

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/
void
traverse_variables(struct variables* sh, int f) {
        if (sh != NULL){
                switch (f) {
                        case TREE_TRAVERSE:
                                traverse_variables(sh->vars, f);
                                traverse_variable(sh->var, f);
                                break;

                        case TREE_CHECK_SEMANTIC:
                                traverse_variables(sh->vars, f);
                                traverse_variable(sh->var, f);
                                break;

                        case TREE_GENERATE_CODE:
                                traverse_variables(sh->vars, f);
                                traverse_variable(sh->var, f);
                                break;

                        default:
                                break;
                }
        }
}

/** 
inspects a single variable

\param sh pointer to a variable structure

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/
void
traverse_variable(struct variable* sh, int f) {
	switch (f) {
	case TREE_TRAVERSE:
		sh->used = 1;
		break;

	case TREE_CHECK_SEMANTIC:
		pruneUnusedGlobalVariable(sh);
		pruneUnusedSharedVariable(sh);
		break;

	case TREE_GENERATE_CODE:
		generateVariable(sh, current_process_gen, current_module_gen);
		break;

	default:
		break;
	}
}


/** 
configuration traversal

\param c pointer to header of a list of configuration

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/

void
traverse_processnodes(struct processnodes* c, int f) {	
	if (c != NULL) {
		switch (f) {
		case TREE_TRAVERSE:
			traverse_processnodes(c->confs, f);
			traverse_process(c->conf, f);
			break;

		case TREE_CHECK_SEMANTIC:
			traverse_processnodes(c->confs, f);
			traverse_process(c->conf, f);
			break;

		case TREE_GENERATE_CODE:
			traverse_processnodes(c->confs, f);
			traverse_process(c->conf, f);
			break;
			
		default:
			break;
		}

	}
}


/** 
visit a single configuration

\param c pointer to a configuration structure

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/

void
traverse_process(struct confnode* c, int f) {	
	switch (f) {
		case TREE_TRAVERSE:
			traverse_variables(c->app->variables, f);
			traverse_variables(c->net->variables, f);
			traverse_variables(c->am->variables, f);
			break;

		case TREE_CHECK_SEMANTIC:
			updateProcessVariables(c);
			checkConfiguration(c);
			checkConfigurationModules(c);
			break;
        
		case TREE_GENERATE_CODE:
			print_process(c);

			current_process_gen = c;
			current_module_gen = c->app;
			traverse_variables(c->app->variables, f);

			current_module_gen = c->net;
			traverse_variables(c->net->variables, f);

			current_module_gen = c->am;
			traverse_variables(c->am->variables, f);
			break;

		default:
                        break;
	}
}


void
traverse_statenodes(struct statenodes* s, int f) {	
	if (s != NULL){
		switch (f) {
		case TREE_TRAVERSE:
			traverse_statenodes(s->states, f);
			traverse_statenode(s->state, f);
			break;

		case TREE_CHECK_SEMANTIC:
			traverse_statenodes(s->states, f);
			traverse_statenode(s->state, f);
			break;

		case TREE_GENERATE_CODE:
			traverse_statenodes(s->states, f);
			traverse_statenode(s->state, f);
			break;
			
		default:
			break;
		}
	} else {
		switch(f) {
		case TREE_CHECK_SEMANTIC:
			break;

		default:
			break;
		}

	}
}


void
traverse_statenode(struct statenode* s, int f) {	
	switch (f) {
		case TREE_TRAVERSE:
			break;

		case TREE_CHECK_SEMANTIC:
			checkState(s);
			break;
        
		case TREE_GENERATE_CODE:
			break;

		default:
                        break;
	}
}



/** 
policy traversal

\param p pointer to header of a list of policies

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/
void traverse_policies(struct policies* p, int f) {
	if (p != NULL){
		switch (f) {
			case TREE_TRAVERSE:
				traverse_policies(p->pols, f);
				traverse_policy(p->pol, f);
                                break;

			case TREE_CHECK_SEMANTIC:
				traverse_policies(p->pols, f);
				traverse_policy(p->pol, f);
                                break;
        
			case TREE_GENERATE_CODE:
				traverse_policies(p->pols, f);
				traverse_policy(p->pol, f);
                                break;

			default:
				break;
		}
	}
}

/** 
visit a policy definition

\param p pointer to single policy structure

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/
void
traverse_policy(struct policy* p, int f) {
	switch (f) {
        	case TREE_TRAVERSE:
			break;

       		case TREE_CHECK_SEMANTIC:
			checkPolicy(p);
			updateStatesWithEvents(p);
			break;
        
       		case TREE_GENERATE_CODE:
			//proc_policy(p);
			break;

       		default:
			break;
	}
}
/** 
initial node visit

\param i pointer to the initial state

\param f type of traversar (TREE_TRAVERSE, TREE_CHECK_SEMANTIC, TREE_GENERATE_CODE)
*/
void
traverse_initnode(struct initnode *i, int f) {
	switch (f) {
        	case TREE_TRAVERSE:
			break;

       		case TREE_CHECK_SEMANTIC:
			checkInitial(i);
			break;
        
       		case TREE_GENERATE_CODE:
			generateInitial(i);
			break;

       		default:
			break;
	}
}
