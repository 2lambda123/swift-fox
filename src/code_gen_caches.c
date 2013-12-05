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
#include "utils.h"

/** Generates SF output file ff_caches.h.

\param event_counter number of events found in Swift Fox program
\param policy_counter number of policies found in Swift Fox program
*/
void generateCaches(int event_counter, int policy_counter) {

	FILE *tmp_confs = fopen(TEMP_CONF_FILE, "r");

	char *full_path = get_sfc_path("", "ff_caches.h");
	FILE *fp = fopen(full_path, "w");

	struct modtab *mp;
	struct conf_ids *conf_ptr;
	int i;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
					into file: %s\n", full_path);
		exit(1);
	}

	if (tmp_confs == NULL) {
		fprintf(stderr, "You do not have a permission to write \
					into file: %s\n", TEMP_CONF_FILE);
		exit(1);
	}

	fprintf(fp, "/* Swift Fox generated code for Fennec Fox caches.h */\n");
	fprintf(fp, "#ifndef __FF_CACHES_H__\n");
	fprintf(fp, "#define __FF_CACHES_H__\n\n");


	fprintf(fp, "#define NUMBER_OF_CONFIGURATIONS\t\t%d\n", conf_id_counter);
	fprintf(fp, "#define NUMBER_OF_STATES\t\t%d\n", state_id_counter);
	fprintf(fp, "#define NUMBER_OF_EVENTS\t\t%d\n", event_id_counter);
	fprintf(fp, "#define NUMBER_OF_POLICIES\t\t%d\n", policy_counter);
	fprintf(fp, "#include <Fennec.h>\n");
	fprintf(fp, "#include \"ff_defaults.h\"\n\n");

        for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
                if (mp->lib != NULL && mp->lib->path && mp->id > 0) {
                        fprintf(fp, "#include \"%sParams.h\"\n",
                                                mp->lib->full_name);
                }
        }
        fprintf(fp, "\n");

        for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
                if (mp->lib != NULL && mp->lib->path && mp->id > 0) {
			fprintf(fp, "/* %s - %s */\n", mp->lib->full_name, mp->lib->path);
                        fprintf(fp, "#define %s\t%d\n", mp->name, mp->id);
                }
        }
        fprintf(fp, "\n");




	/* Generate Configurations */

	fprintf(fp, "uint16_t active_state = %d;\n\n", active_state);
	fprintf(fp, "struct stack_configuration configurations[NUMBER_OF_CONFIGURATIONS] = {\n");

	/* generate code specifying each configuration's definition */

	for( i = 0; i < conf_id_counter; i++ ) {
		fprintf(fp, "\t{\n");
   		fprintf(fp, "\t\t/* %s */\n", conftab[i].conf->id->name); 
		fprintf(fp, "\t\t.conf_id = %d,\n", conftab[i].conf->counter);
		fprintf(fp, "\t\t.application = %s,\n", conftab[i].conf->app->name);
		fprintf(fp, "\t\t.network = %s,\n", conftab[i].conf->net->name);
		fprintf(fp, "\t\t.mac = %s,\n", conftab[i].conf->mac->name);
		fprintf(fp, "\t\t.radio = %s,\n", conftab[i].conf->radio->name);
		if (conftab[i].conf->level == UNKNOWN) {
			fprintf(fp, "\t\t.level = F_MINIMUM_STATE_LEVEL\n");
		} else {
			fprintf(fp, "\t\t.level = %d\n", conftab[i].conf->level);
		}
		fprintf(fp, "\t}\n");
		if (i+1 < conf_id_counter) {
			fprintf(fp, "\t,\n");
		}
	}

	fprintf(fp, "};\n\n");


	/* Generate States */

	//int confs_per_state = 0;


	for( i = 0; i < state_id_counter; i++ ) {
		fprintf(fp, "conf_t %s_confs[%d] = {", statetab[i].state->id->name, statetab[i].state->confs_counter);
		conf_ptr = statetab[i].state->confs;
		while (conf_ptr) { 
			if (conf_ptr->conf) {
				fprintf(fp, "%d", conf_ptr->conf->conf->counter);
			}
			conf_ptr = conf_ptr->confs;
			if (conf_ptr != NULL) {
				fprintf(fp, ", ");

			}
		}
		fprintf(fp, "};\n\n");

   		fprintf(fp, "/* %s */\n", statetab[i].state->id->name); 
		fprintf(fp, "struct stack_params %s_params[%d] = {\n", statetab[i].state->id->name, statetab[i].state->confs_counter);
		conf_ptr = statetab[i].state->confs;
		while (conf_ptr) { 
			if (conf_ptr->conf) {
				fprintf(fp, "\t{\n");
				fprintf(fp, "\t\t&%s_%s_ptr,\n", conf_ptr->conf->conf->id->name, conf_ptr->conf->conf->app->lib->full_name);
				fprintf(fp, "\t\t&%s_%s_ptr,\n", conf_ptr->conf->conf->id->name, conf_ptr->conf->conf->net->lib->full_name);
				fprintf(fp, "\t\t&%s_%s_ptr,\n", conf_ptr->conf->conf->id->name, conf_ptr->conf->conf->mac->lib->full_name);
				fprintf(fp, "\t\t&%s_%s_ptr\n", conf_ptr->conf->conf->id->name, conf_ptr->conf->conf->radio->lib->full_name);
				fprintf(fp, "\t}\n");
			}
			conf_ptr = conf_ptr->confs;
			if (conf_ptr != NULL) {
				fprintf(fp, ", ");

			}
		}
		fprintf(fp, "};\n\n");
	}



	fprintf(fp, "struct state states[NUMBER_OF_STATES] = {\n");
	for( i = 0; i < state_id_counter; i++ ) {
                fprintf(fp, "\t{\n");
   		fprintf(fp, "\t\t/* %s */\n", statetab[i].state->id->name); 
                fprintf(fp, "\t\t.state_id = %d,\n", statetab[i].state->counter);
                fprintf(fp, "\t\t.num_confs = %d,\n", statetab[i].state->confs_counter);
		fprintf(fp, "\t\t.conf_list = %s_confs,\n", statetab[i].state->id->name);
		fprintf(fp, "\t\t.conf_params = %s_params\n", statetab[i].state->id->name);
		

		fprintf(fp, "\t}\n");
		if (i+1 < state_id_counter) {
			fprintf(fp, "\t,\n");
		}

	}
	fprintf(fp, "};\n\n");


	/* module_t to event_id */
	fprintf(fp, "struct event_module_conf event_module_conf[NUMBER_OF_EVENTS] = {\n");
	fprintf(fp, "\t/* [event_id, module_id, conf_id */\n");
	for ( i = 0; i < conf_id_counter; i++ ) {
		if (conftab[i].conf->id->type == TYPE_PROCESS_EVENT) {
			fprintf(fp, "\t{%d, %d, %d},\n",
					conftab[i].conf->id->value,
					conftab[i].conf->app->id,
					conftab[i].conf->counter);
		}
	}

	fprintf(fp, "};\n\n");



/*
	fprintf(fp, "struct fennec_event eventsTable[%d] = {\n", event_counter);

	for ( i = 0; i < event_counter; ) {

		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\t.operation = %s,\n", relopToLetter(evtab[i].op));
		fprintf(fp, "\t\t.value = %d,\n", evtab[i].value);

		if (evtab[i].addr == UNKNOWN) {
			fprintf(fp, "\t\t.addr = TOS_NODE_ID\n");
		} else {
			fprintf(fp, "\t\t.addr = %d\n", evtab[i].addr);
		}
		if (++i < event_counter) {
			fprintf(fp, "\t},\n");
		} else {
			fprintf(fp, "\t}\n");
		}
	}

	fprintf(fp, "};\n\n");
*/


        fprintf(fp, "struct fennec_policy policies[NUMBER_OF_POLICIES] = {\n");
	for(i = 0; i < policy_counter; ) {
		fprintf(fp, "\t{\n");
		fprintf(fp, "\t\t.src_conf = %d,\n", poltab[i].policy->from->value);
		fprintf(fp, "\t\t.event_mask = %d,\n", get_policy_mask(poltab[i].policy));
		fprintf(fp, "\t\t.dst_conf = %d\n\n", poltab[i].policy->to->value);

		if (++i < policy_counter) {
			fprintf(fp, "\t},\n");
		} else {
			fprintf(fp, "\t}\n");
		}
	}

	fprintf(fp, "};\n\n");

	fprintf(fp, "#endif\n\n");

	fclose(fp);
	fclose(tmp_confs);
}

/** Prepares SF configuration for the output. Each configuration's
module is marked as used and the configuration is added to the 
conftab list of all configurations.

\param c pointer to configuration structure
*/
void generateConfiguration(struct confnode* c) {

}

/** Marks the initial state.

\param i pointer to the initial state structure
*/
void generateInitial(struct initnode *i) {
	active_state = i->init;
}
