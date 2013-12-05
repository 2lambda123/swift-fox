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

void startGlobalVariables() {
	char *full_path = get_sfc_path("", "ff_globals.h");
	FILE *fp = fopen(full_path, "w");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#ifndef _FF_GLOBALS_H_\n");
	fprintf(fp, "#define _FF_GLOBALS_H_\n\n");

	free(full_path);
	fclose(fp);
}


void endGlobalVariables() {
	char *full_path = get_sfc_path("", "ff_globals.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	fprintf(fp, "#endif\n");

	free(full_path);
	fclose(fp);
}


void addGlobalVariable(struct variable *sh) {
	char *full_path = get_sfc_path("", "ff_globals.h");
	FILE *fp = fopen(full_path, "a");

	if (fp == NULL) {
		fprintf(stderr, "You do not have a permission to write \
						into file: %s\n", full_path);
		exit(1);
	}

	sh->name->type = TYPE_VARIABLE_GLOBAL;
	fprintf(fp, "%s %s[%d] = {%f};\n\n", type_name(sh->type), 
						sh->name->name,
						sh->length,
						sh->value);

	free(full_path);
	fclose(fp);
}
