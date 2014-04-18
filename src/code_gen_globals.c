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
	fprintf(fp, "%s %s[%d] = {%Lf};\n\n", type_name(sh->type), 
						sh->name->name,
						sh->length,
						sh->value);

	free(full_path);
	fclose(fp);
}
