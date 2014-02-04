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

void module_params_interface_app(struct modtab *mp) {
	char *full_path = get_sfc_path(mp->lib->name, "Params.nc");
	FILE *fp = fopen(full_path, "w");
	struct paramtype *pt;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "interface %sParams {\n", (mp->lib->name));
//	fprintf(fp, "\tevent void receive_status(uint16_t status_flag);\n");
//	fprintf(fp, "\tcommand void send_status(uint16_t status_flag);\n");


	if (mp->lib->params == NULL) {
		/**
		nesC does not allow empty interfaces, so if there
		is a module that does not take any parameters, we need to
		fake at least one dummy function on parameter interface
		*/ 
		fprintf(fp, "\tcommand void dummy();\n");
	}

        for(pt = mp->lib->params; pt != NULL; pt = pt->child ) {
                fprintf(fp, "\tcommand %s get_%s();\n", 
				type_name(pt->type), pt->name);
                fprintf(fp, "\tcommand error_t set_%s(%s new_%s);\n", 
				pt->name, type_name(pt->type), pt->name);
        }

	fprintf(fp, "}\n\n");

	fclose(fp);
	free(full_path);
}


void module_params_h(struct modtab *mp) {
	char *full_path = get_sfc_path(mp->lib->name, "Params.h");
	FILE *fp = fopen(full_path, "w");
	struct paramtype *pt;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _FF_MODULE_%s_H_\n", mp->lib->name);
	fprintf(fp, "#define _FF_MODULE_%s_H_\n\n", mp->lib->name);

	fprintf(fp, "struct %s_params {\n", mp->lib->name);

	for(pt = mp->lib->params; pt != NULL; pt = pt->child) {
		fprintf(fp, "\t%s %s;\n", type_name(pt->type), pt->name);
	}

	fprintf(fp, "};\n\n");

	fprintf(fp, "struct %s_params_ptr {\n", mp->lib->name);

	for(pt = mp->lib->params; pt != NULL; pt = pt->child) {
		fprintf(fp, "\t%s *%s;\n", type_name(pt->type), pt->name);
	}

	fprintf(fp, "};\n\n");

	fprintf(fp, "#endif\n");

	fclose(fp);
	free(full_path);
}

void generateParams() {
	int i;
	for( i = 0; i < conf_id_counter; i++ ) {
		module_params_interface_app(conftab[i].conf->app);
		module_params_h(conftab[i].conf->app);
		module_params_interface_app(conftab[i].conf->net);
		module_params_h(conftab[i].conf->net);
		module_params_interface_app(conftab[i].conf->mac);
		module_params_h(conftab[i].conf->mac);
		module_params_interface_app(conftab[i].conf->radio);
		module_params_h(conftab[i].conf->radio);
	}
}


