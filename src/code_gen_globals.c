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
#include "utils.h"

int generate_globals = 1;

void initGlobalDataValues() {
	char *full_path = get_sfc_path("", "global_data_values.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _FF_GLOBAL_DATA_VALUES_H_\n");
	fprintf(fp, "#define _FF_GLOBAL_DATA_VALUES_H_\n\n");
	fprintf(fp, "#include \"Fennec.h\"\n\n");
	fprintf(fp, "#include \"global_data.h\"\n\n");

	fprintf(fp, "struct global_data fennec_global_data = {\n");
	
	free(full_path);
	fclose(fp);
}

void initLocalDataValues() {
	char *full_path = get_sfc_path("", "local_data_values.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _FF_LOCAL_DATA_VALUES_H_\n");
	fprintf(fp, "#define _FF_LOCAL_DATA_VALUES_H_\n\n");
	fprintf(fp, "#include \"Fennec.h\"\n\n");
	fprintf(fp, "#include \"local_data.h\"\n\n");

	fprintf(fp, "struct local_data fennec_local_data = {\n");
	
	free(full_path);
	fclose(fp);
}

void initSharedDataValues() {
	char *full_path = get_sfc_path("", "shared_data_values.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _FF_SHARED_DATA_VALUES_H_\n");
	fprintf(fp, "#define _FF_SHARED_DATA_VALUES_H_\n\n");
	fprintf(fp, "#include \"Fennec.h\"\n\n");
	fprintf(fp, "#include \"shared_data.h\"\n\n");

	fprintf(fp, "struct shared_data fennec_shared_data = {\n");
	
	free(full_path);
	fclose(fp);
}


void initGlobalDataH() {
	/* global struct */
	char *full_path = get_sfc_path("", "global_data.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _GLOBAL_DATA_H_\n");
	fprintf(fp, "#define _GLOBAL_DATA_H_\n\n");

	fprintf(fp, "struct global_data {\n");

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}

void finishGlobalDataH() {
	/* global struct */
	char *full_path = get_sfc_path("", "global_data.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}

void initSharedDataH() {
	/* global struct */
	char *full_path = get_sfc_path("", "shared_data.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _SHARED_DATA_H_\n");
	fprintf(fp, "#define _SHARED_DATA_H_\n\n");

	fprintf(fp, "struct shared_data {\n");

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}

void finishSharedDataH() {
	/* global struct */
	char *full_path = get_sfc_path("", "shared_data.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}

void globalDataMsgH() {
	/* global struct */
	char *full_path = get_sfc_path("", "global_data_msg.h");
	FILE *fp = fopen(full_path, "w");
	int i;
	int global_number = 0;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _GLOBAL_DATA_MSG_H_\n");
	fprintf(fp, "#define _GLOBAL_DATA_MSG_H_\n\n");
	fprintf(fp, "#include \"global_data.h\"\n\n");

	fprintf(fp, "nx_struct global_data_msg {\n");

	for( i = 0; i < NVARS; i++ ) {
		if (vartab[i].length == 0) {
			break;
		}

		if ((vartab[i].class_type != TYPE_VARIABLE_GLOBAL) ||
			(vartab[i].used != 1) ||
			(vartab[i].gname != NULL)) {
			continue;
		}

		if (vartab[i].length > 1) {
			fprintf(fp, "\tnx_%s %s[%d];\n", type_name(vartab[i].type), 
						vartab[i].name,
						vartab[i].length);
		} else {
			fprintf(fp, "\tnx_%s %s;\n", type_name(vartab[i].type), 
						vartab[i].name);
		}
		global_number++;
	}

	fprintf(fp, "};\n\n\n");

	fprintf(fp, "#define NUMBER_OF_GLOBALS \t%d\n\n", global_number);

	fprintf(fp, "nx_struct global_data_msg fennec_global_data_nx;\n\n");

	fprintf(fp, "struct variable_info global_data_info[NUMBER_OF_GLOBALS] = {\n");
	for( i = 0; i < NVARS; i++ ) {
		if (vartab[i].length == 0) {
			break;
		}

		if ((vartab[i].class_type != TYPE_VARIABLE_GLOBAL) ||
			(vartab[i].used != 1) ||
			(vartab[i].gname != NULL)) {
			continue;
		}

		fprintf(fp, "\t{ %s, \t%d, \t%d },\n", vartab[i].cap_name,
				vartab[i].offset, 
				(type_size(vartab[i].type) * vartab[i].length));
	}

	fprintf(fp, "};\n\n");

	fprintf(fp, "void globalDataSyncWithNetwork(uint8_t var_id) {\n");
	fprintf(fp, "\tswitch(var_id) {\n");
	for( i = 0; i < NVARS; i++ ) {
		if (vartab[i].length == 0) {
			break;
		}

		if ((vartab[i].class_type != TYPE_VARIABLE_GLOBAL) ||
			(vartab[i].used != 1) ||
			(vartab[i].gname != NULL)) {
			continue;
		}
		fprintf(fp, "\tcase %s:\n", vartab[i].cap_name);
		fprintf(fp, "\t\tfennec_global_data.%s = (%s) fennec_global_data_nx.%s;\n",
			vartab[i].name, type_name(vartab[i].type), vartab[i].name);
		fprintf(fp, "\t\tbreak;\n\n");
	}
	fprintf(fp, "\t};\n");
	fprintf(fp, "};\n\n\n");

	fprintf(fp, "void globalDataSyncWithLocal(uint8_t var_id) {\n");
	fprintf(fp, "\tswitch(var_id) {\n");
	for( i = 0; i < NVARS; i++ ) {
		if (vartab[i].length == 0) {
			break;
		}

		if ((vartab[i].class_type != TYPE_VARIABLE_GLOBAL) ||
			(vartab[i].used != 1) ||
			(vartab[i].gname != NULL)) {
			continue;
		}

		fprintf(fp, "\tcase %s:\n", vartab[i].cap_name);
		fprintf(fp, "\t\tfennec_global_data_nx.%s = (nx_%s) fennec_global_data.%s;\n",
			vartab[i].name, type_name(vartab[i].type), vartab[i].name);
		fprintf(fp, "\t\tbreak;\n\n");
	}
	fprintf(fp, "\t};\n");
	fprintf(fp, "};\n\n\n");

	fprintf(fp, "#if defined(FENNEC_TOS_PRINTF) || defined(FENNEC_COOJA_PRINTF)\n");
	fprintf(fp, "void printfGlobalData() {\n");
	for( i = 0; i < NVARS; i++ ) {
		if (vartab[i].length == 0) {
			break;
		}

		if ((vartab[i].class_type != TYPE_VARIABLE_GLOBAL) ||
			(vartab[i].used != 1) ||
			(vartab[i].gname != NULL)) {
			continue;
		}

		switch(vartab[i].type) {
		case TYPE_BOOL:
		case TYPE_UINT8_T:
		case TYPE_UINT16_T:
		case TYPE_NXUINT8_T:
		case TYPE_NXUINT16_T:
			fprintf(fp, "\tprintf(\"%s -> %%u  |  \", fennec_global_data.%s);\n",
				vartab[i].name, vartab[i].name);
			break;
		case TYPE_UINT32_T:
		case TYPE_NXUINT32_T:
			fprintf(fp, "\tprintf(\"%s -> %%lu  |  \", fennec_global_data.%s);\n",
				vartab[i].name, vartab[i].name);
			break;
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
			fprintf(fp, "\tprintf(\"%s -> %%f  |  \", fennec_global_data.%s);\n",
				vartab[i].name, vartab[i].name);
			break;
		default:
			fprintf(fp, "\tprintf(\"%s -> %%d  |  \", fennec_global_data.%s);\n",
				vartab[i].name, vartab[i].name);
		}
	}
	fprintf(fp, "\tprintf(\"\\n\");\n");
	fprintf(fp, "};\n\n");


	fprintf(fp, "void printfGlobalDataNx() {\n");
	for( i = 0; i < NVARS; i++ ) {
		if (vartab[i].length == 0) {
			break;
		}

		if ((vartab[i].class_type != TYPE_VARIABLE_GLOBAL) ||
			(vartab[i].used != 1) ||
			(vartab[i].gname != NULL)) {
			continue;
		}

		switch(vartab[i].type) {
		case TYPE_BOOL:
		case TYPE_UINT8_T:
		case TYPE_UINT16_T:
		case TYPE_NXUINT8_T:
		case TYPE_NXUINT16_T:
			fprintf(fp, "\tprintf(\"%s -> %%u  |  \", fennec_global_data_nx.%s);\n",
				vartab[i].name, vartab[i].name);
			break;
		case TYPE_UINT32_T:
		case TYPE_NXUINT32_T:
			fprintf(fp, "\tprintf(\"%s -> %%lu  |  \", fennec_global_data_nx.%s);\n",
				vartab[i].name, vartab[i].name);
			break;
		case TYPE_FLOAT:
		case TYPE_DOUBLE:
			fprintf(fp, "\tprintf(\"%s -> %%f  |  \", fennec_global_data_nx.%s);\n",
				vartab[i].name, vartab[i].name);
			break;
		default:
			fprintf(fp, "\tprintf(\"%s -> %%d  |  \", fennec_global_data_nx.%s);\n",
				vartab[i].name, vartab[i].name);
		}
	}
	fprintf(fp, "\tprintf(\"\\n\");\n");
	fprintf(fp, "};\n");

	fprintf(fp, "#endif\n\n");

	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}


void initLocalDataH() {
	/* local struct */
	char *full_path = get_sfc_path("", "local_data.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _LOCAL_DATA_H_\n");
	fprintf(fp, "#define _LOCAL_DATA_H_\n\n");

	fprintf(fp, "struct local_data {\n");

	free(full_path);
	fclose(fp);
	/* end of definig local variables */
}


void finishLocalDataH() {
	/* local struct */
	char *full_path = get_sfc_path("", "local_data.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of definig local variables */
}


void finishGlobalDataValues() {
	/* global variables init */
	char *full_path = get_sfc_path("", "global_data_values.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of initializing global variables */
}

void finishLocalDataValues() {
	/* global variables init */
	char *full_path = get_sfc_path("", "local_data_values.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of initializing global variables */
}

void finishSharedDataValues() {
	/* global variables init */
	char *full_path = get_sfc_path("", "shared_data_values.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of initializing global variables */
}

void switchGlobalToLocalDataStorage() {
	generate_globals = 0;
}


void addGlobalVariable(struct variable *sh) {
	/* global struct */
	char *full_path = get_sfc_path("", "global_data.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	if (sh->used == 1) {
		if (sh->length > 1) {
			fprintf(fp, "\t%-10s %s[%d];\n", type_name(sh->type), 
						sh->name,
						sh->length);
		} else {
			fprintf(fp, "\t%-10s %s;\n", type_name(sh->type), 
						sh->name);
		}
	}

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}


void addSharedVariable(struct variable *sh) {
	/* global struct */
	char *full_path = get_sfc_path("", "shared_data.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	if (sh->used == 1) {
		if (sh->length > 1) {
			fprintf(fp, "\t%-10s %s[%d];\n", type_name(sh->type), 
						sh->name,
						sh->length);
		} else {
			fprintf(fp, "\t%-10s %s;\n", type_name(sh->type), 
						sh->name);
		}
	}

	free(full_path);
	fclose(fp);
	/* end of definig global variables */
}


void addLocalVariable(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen) {
	/* local struct */
	char *full_path = get_sfc_path("", "local_data.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	if (sh->class_type == TYPE_VARIABLE_LOCAL) {
		if (sh->length > 1) {
			fprintf(fp, "\t%-10s %s_%s_%s[%d];\n", type_name(sh->type), 
						current_process_gen->name, current_module_gen->name, sh->name,
						sh->length);
		} else {
			fprintf(fp, "\t%-10s %s_%s_%s;\n", type_name(sh->type), 
						current_process_gen->name, current_module_gen->name, sh->name);
		}
	}

	free(full_path);
	fclose(fp);
	/* end of definig local variables */
}


void generateVariable(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen) {
	if (generate_globals) {
		if (sh->class_type == TYPE_VARIABLE_GLOBAL) {
			addGlobalVariable(sh);
			setGlobalVariableValue(sh, current_process_gen, current_module_gen);
		} else {
			addSharedVariable(sh);
			setSharedVariableValue(sh, current_process_gen, current_module_gen);
		}
	} else {
		addLocalVariable(sh, current_process_gen, current_module_gen);
		setLocalVariableValue(sh, current_process_gen, current_module_gen);
	}
}	

void setGlobalVariableValue(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen) {
	char *full_path = get_sfc_path("", "global_data_values.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	if ((sh->class_type == TYPE_VARIABLE_GLOBAL) && (sh->used == 1)) {
		if (sh->length > 1) {
			fprintf(fp, "\t.%-50s = {%-15Lf},\t/* %d */\n", sh->name,
						sh->value, sh->offset);
		} else {
			fprintf(fp, "\t.%-55s =  %-15Lf, \t/* %d */\n", sh->name,
						sh->value, sh->offset);
		}
	}

	free(full_path);
	fclose(fp);
}


void setSharedVariableValue(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen) {
	char *full_path = get_sfc_path("", "shared_data_values.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	if ((sh->class_type == TYPE_VARIABLE_SHARED) && (sh->used == 1)) {
		if (sh->length > 1) {
			fprintf(fp, "\t.%-50s = {%-15Lf},\t/* %d */\n", sh->name,
						sh->value, sh->offset);
		} else {
			fprintf(fp, "\t.%-55s =  %-15Lf, \t/* %d */\n", sh->name,
						sh->value, sh->offset);
		}
	}

	free(full_path);
	fclose(fp);
}

void setLocalVariableValue(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen) {
	char *full_path = get_sfc_path("", "local_data_values.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	if (sh->class_type == TYPE_VARIABLE_LOCAL) {
		char *full_var_name = malloc(strlen(current_process_gen->name) + 
				strlen(current_module_gen->name) +
				strlen(sh->name) + 3);
		sprintf(full_var_name, "%s_%s_%s", current_process_gen->name,
				current_module_gen->name, sh->name);
		if (sh->length > 1) {
			fprintf(fp, "\t.%-55s = {%-15Lf},\t/* %d */\n",
				full_var_name, sh->value, sh->offset);
		} else {
			fprintf(fp, "\t.%-55s =  %-15Lf ,\t/* %d */\n",
				full_var_name, sh->value, sh->offset);
		}
	}

	free(full_path);
	fclose(fp);
}


void setProcessesLookupTable() {
	/* variable_lookup struct */
	char *full_path = get_sfc_path("", "variable_lookup.h");
	FILE *fp = fopen(full_path, "w");
	struct variables *mvar;
	int i;
	int vc;
	int j;
	char *cap_name;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _VARIABLE_LOOKUP_DATA_H_\n");
	fprintf(fp, "#define _VARIABLE_LOOKUP_DATA_H_\n\n");
	fprintf(fp, "#include <Fennec.h>\n\n");

	fprintf(fp, "struct variable_reference variable_lookup[%d] = {\n", number_of_variables_in_cache);

	j = 0;
	for( i = 0; i < conf_id_counter; i++ ) {
		for (vc = 0, mvar = conftab[i].conf->app->variables; vc < conftab[i].conf->app_var_num && mvar != NULL; vc++) {
			if (mvar->var->class_type == TYPE_VARIABLE_GLOBAL) {
				cap_name = strdup(mvar->var->gname);
				cap_name = str_toupper(cap_name);
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_global_data.%s), %s },\n",
						j, mvar->var->cap_name, mvar->var->gname,
						cap_name);
			}

			if (mvar->var->class_type == TYPE_VARIABLE_SHARED) {
				cap_name = strdup(mvar->var->gname);
				cap_name = str_toupper(cap_name);
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_shared_data.%s), %s },\n",
						j, mvar->var->cap_name, mvar->var->gname,
						cap_name);
			}

			if (mvar->var->class_type == TYPE_VARIABLE_LOCAL) {
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_local_data.%s_%s_%s), UNKNOWN },\n",
						j, mvar->var->cap_name, conftab[i].conf->name,
						conftab[i].conf->app->name, mvar->var->name);
			}
			mvar = mvar->vars;
			j++;
		}

		conftab[i].conf->app_var_offset = variable_cache;
		variable_cache += conftab[i].conf->app_var_num;

		for (vc = 0, mvar = conftab[i].conf->net->variables; vc < conftab[i].conf->net_var_num && mvar != NULL; vc++) {
			if (mvar->var->class_type == TYPE_VARIABLE_GLOBAL) {
				cap_name = strdup(mvar->var->gname);
				cap_name = str_toupper(cap_name);
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_global_data.%s), %s },\n",
						j, mvar->var->cap_name, mvar->var->gname,
						cap_name);
			}

			if (mvar->var->class_type == TYPE_VARIABLE_SHARED) {
				cap_name = strdup(mvar->var->gname);
				cap_name = str_toupper(cap_name);
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_shared_data.%s), %s },\n",
						j, mvar->var->cap_name, mvar->var->gname,
						cap_name);
			}

			if (mvar->var->class_type == TYPE_VARIABLE_LOCAL) {
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_local_data.%s_%s_%s), UNKNOWN },\n",
						j, mvar->var->cap_name, conftab[i].conf->name,
						conftab[i].conf->net->name, mvar->var->name);
			}
			mvar = mvar->vars;
			j++;
		}

		conftab[i].conf->net_var_offset = variable_cache;
		variable_cache += conftab[i].conf->net_var_num;

		for (vc = 0, mvar = conftab[i].conf->am->variables; vc < conftab[i].conf->am_var_num && mvar != NULL; vc++) {
			if (mvar->var->class_type == TYPE_VARIABLE_GLOBAL) {
				cap_name = strdup(mvar->var->gname);
				cap_name = str_toupper(cap_name);
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_global_data.%s), %s },\n",
						j, mvar->var->cap_name, mvar->var->gname,
						cap_name);
			}

			if (mvar->var->class_type == TYPE_VARIABLE_SHARED) {
				cap_name = strdup(mvar->var->gname);
				cap_name = str_toupper(cap_name);
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_shared_data.%s), %s },\n",
						j, mvar->var->cap_name, mvar->var->gname,
						cap_name);
			}

			if (mvar->var->class_type == TYPE_VARIABLE_LOCAL) {
				fprintf(fp, "/* %3d */   { %-35s, &(fennec_local_data.%s_%s_%s), UNKNOWN },\n",
						j, mvar->var->cap_name, conftab[i].conf->name,
						conftab[i].conf->am->name, mvar->var->name);
			}
			mvar = mvar->vars;
			j++;
		}

		conftab[i].conf->am_var_offset = variable_cache;
		variable_cache += conftab[i].conf->am_var_num;

	}
	fprintf(fp, "};\n\n");
	fprintf(fp, "#endif\n\n");

	free(full_path);
	fclose(fp);
	/* end of adding variable lookup */
}



