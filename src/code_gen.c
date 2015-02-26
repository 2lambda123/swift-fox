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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
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

	/* start with current working directory */
	char cwd[2048];
	if ( getcwd(cwd, sizeof(cwd) ) != NULL) {
		fprintf(fp_fe, "PFLAGS+=-I%s\n", cwd);
	} else {
		perror("getcwd() error");
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

	generateFennecEngineC();

	generateFennecEngineP();
	generateVariableConstants();

	setFennecExtra();
}

