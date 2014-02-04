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
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

/**
implementation of the fennec 
calls underlying TinyOS make 
*/
int main(int argc, char *argv[]) {

	int i = 1;
	struct stat s;
        char *fennec_fox_lib = getenv("FENNEC_FOX_LIB");
	char *buff = malloc(strlen(fennec_fox_lib) + 15 );

        if (fennec_fox_lib == NULL) {
                fprintf(stderr, "\n\nFENNEC_FOX_LIB is not set!\n");
                exit(1);
        }

	sprintf(buff, "%s/Fennec/sfc", fennec_fox_lib);

	if (stat(buff, &s) == -1) {
		perror(buff);
                fprintf(stderr, "Swift Fox was not compiled!\n");
                exit(1);
	}

	sprintf(buff, "make -C %s", fennec_fox_lib);

	while(argv[i]!=NULL) {
		strcat(buff, " ");
		strcat(buff, argv[i]);
		i++;
	}

	printf("%s\n", buff);
	return system(buff);
}
