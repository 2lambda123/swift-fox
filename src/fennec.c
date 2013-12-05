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
