/** \file
    Swift Fox Compiler
    Copyright (C) 2009-2013 Marcin Szczodrak

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

void module_params_interface_app(struct modtab *mp) {
	char *full_path = get_sfc_path(mp->lib->full_name, "Params.nc");
	FILE *fp = fopen(full_path, "w");
	struct paramtype *pt;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "interface %sParams {\n", (mp->lib->full_name));
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
	char *full_path = get_sfc_path(mp->lib->full_name, "Params.h");
	FILE *fp = fopen(full_path, "w");
	struct paramtype *pt;

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _FF_MODULE_%s_H_\n", mp->lib->full_name);
	fprintf(fp, "#define _FF_MODULE_%s_H_\n\n", mp->lib->full_name);

//	for(pt = mp->lib->params; pt != NULL; pt = pt->child) {
//        	fprintf(fp, "%s %s_%s;\n", type_name(pt->type), mp->lib->full_name, pt->name);
//	}
//	fprintf(fp, "\n");

	fprintf(fp, "struct %s_params {\n", mp->lib->full_name);

	for(pt = mp->lib->params; pt != NULL; pt = pt->child) {
		fprintf(fp, "\t%s %s;\n", type_name(pt->type), pt->name);
	}

	fprintf(fp, "};\n\n");

	fprintf(fp, "struct %s_params_ptr {\n", mp->lib->full_name);

	for(pt = mp->lib->params; pt != NULL; pt = pt->child) {
		fprintf(fp, "\t%s *%s;\n", type_name(pt->type), pt->name);
	}

	fprintf(fp, "};\n\n");



	//fprintf(fp, "struct %s_params %s_data;\n", mp->lib->full_name, mp->lib->full_name);

	fprintf(fp, "#endif\n");

	fclose(fp);
	free(full_path);
}

void generateParams() {
	struct modtab *mp;
	for(mp = modtab; mp < &modtab[NSYMS]; mp++) {
		if ((mp->lib != NULL) && (mp->lib->path) && (mp->id > 0) && (mp->lib->used == 1)) {
			module_params_interface_app(mp);
			module_params_h(mp);
		}
	}
}


