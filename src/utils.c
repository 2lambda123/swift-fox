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
#include <sys/stat.h>
#include "utils.h"
#include "sf.h"
#include "parser.h"


/**
converts type value (int) into string

\param type_value constant representing a type

\return string C-compliant type
*/
char * type_name(int type_value) {
	switch(type_value) {
	case TYPE_UINT8_T:
		return "uint8_t";

	case TYPE_UINT16_T:
		return "uint16_t";

	case TYPE_UINT32_T:
		return "uint32_t";

	case TYPE_FLOAT:
		return "float";

	case TYPE_DOUBLE:
		return "double";

	default:
		return "";
	}
}

/**
converts local path within Fennec Fox library to a complete path 
within the system

\param ffsrc_relative string with local path

\return tmp complete path

*/
char *get_full_path(char *ffsrc_relative) {
	char *fennec_fox_lib = getenv("FENNEC_FOX_LIB");
	char *tmp;

	if (fennec_fox_lib == NULL) {
		fprintf(stderr, "\n\nFENNEC_FOX_LIB is not set!\n");
		exit(1);
	}

	tmp = malloc(strlen(ffsrc_relative) + strlen(fennec_fox_lib) + 2);
	sprintf(tmp, "%s/%s", fennec_fox_lib, ffsrc_relative);
	return tmp;
}

/** 
creates a path to a file located within Fennec Fox source directory

\param file1 string with relative path withing Fennec Fox

\param file2 name of a file

\return tmp a complete path to file2
*/
char *get_sfc_path(char *file1, char *file2) {
	char *fennec_fox_lib = getenv("FENNEC_FOX_LIB");
	char *tmp;

	if (fennec_fox_lib == NULL) {
		fprintf(stderr, "\n\nFENNEC_FOX_LIB is not set!\n");
		exit(1);
	}

	tmp = malloc(strlen(fennec_fox_lib) + strlen("Fennec/sfc/") + 
					strlen(file1) + strlen(file2) + 2);
	sprintf(tmp, "%s/Fennec/sfc/%s%s", fennec_fox_lib, file1, file2);
	return tmp;
}


/**
creates directory within Fennec Fox

\param ffsrc_relative string with a path within Fennec Fox source directory

\return status reporting successful creation of a directory
*/
int create_dir(char *ffsrc_relative) {
	struct stat st;
	char *dirpath = get_full_path(ffsrc_relative);
	int status = 0;
	if (stat(dirpath, &st) != 0) {
		status = mkdir(dirpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	}
	free(dirpath);
	return status;
}

int get_policy_mask(struct policy *p) {
	struct conf_ids *cids = NULL;
	int mask = 0;
	for (cids = p->event_confs; cids != NULL; cids = cids->confs) {
		mask |= 1 << cids->conf->conf->id->value;
	}
	return mask;
}
