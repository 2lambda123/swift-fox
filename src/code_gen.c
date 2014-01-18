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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sf.h"
#include "code_gen.h"
#include "parser.h"
#include "utils.h"

char *fex = NULL;

FILE* tmp_confs = NULL;

/** Prepares files before generating the compiler's output.
*/
void setFiles() {

	char *fennec_fox_lib = getenv("FENNEC_FOX_LIB");

	if (fennec_fox_lib == NULL) {
		fprintf(stderr, "\n\nFENNEC_FOX_LIB is not set!\n");
		exit(1);
	}

	char *p_fex = "support/sfc/fennec.extra";
	fex = malloc(strlen(p_fex)+strlen(fennec_fox_lib)+2);
	sprintf(fex, "%s/%s", fennec_fox_lib, p_fex);

	FILE *tmp = fopen(TEMP_CONF_FILE, "w");
	fclose(tmp);
}

void initCodeGeneration() {
	/* check if sfc directory exists */
	if (create_dir("Fennec/sfc")) {
		fprintf(stderr, "Cannot create directory %s\n", "Fennec/sfc");
		exit(1);
	}

}


/** Fill the content of the fennec.extra file.
*/
void setFennecExtra() {

	FILE *fp_fe = fopen(fex, "w");

	if (fp_fe == NULL) {
		fprintf(stderr, "You do not have a permission to write into"
							" file: %s\n", fex);
		exit(1);
	}

	struct libtab *lp;
	for(lp = libtab; lp < &libtab[NSYMS]; lp++) {
		if (lp->path && lp->used == 1) {
			fprintf(fp_fe, "PFLAGS+=-I%s\n", lp->path);
		}
	}

	fclose(fp_fe);
}

/** Finishes code generation. It is the last step after the semantic tarversal.
At this point, all the information for code generation is collected and the 
code can be generated.

\param policy_counter number of policies found in Swift Fox program.
*/
void finishCodeGeneration(int policy_counter) {
	generateCaches(0, policy_counter);
	generateDefaultParams();

	generateFennecEngineC();

	generateFennecEngineP();

	generateParams();

	setFennecExtra();
}

