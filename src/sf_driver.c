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
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include "sf.h"

void print_help();
void print_version();

/** 
the entry point to the sfc compiler
*/
int
main(int argc, char *argv[] ) {

	char *pfile = NULL;
	char *lfile = NULL;
	int c;
	int errflag = 0;

	/** 
	argument checking 
	*/

	while ((c = getopt(argc, argv, "vhdp:l:")) != -1) {
		//fprintf(stdout, "gets %c\n", c);
		switch(c) {
		case 'v':
			print_version();
			exit(0);
		case 'h':
			print_help();
			exit(0);
		case 'd':
			sfc_debug = 1;
			break;
		case 'p':
			pfile = optarg;
			break;
		case 'l':
			lfile = optarg;
			break;
		case ':':
			fprintf(stderr, "Option -%c requires an operand\n", optopt);
			errflag++;
			break;
		case '?':
			fprintf(stderr, "Unrecognized option: '-%c'\n", optopt);
			errflag++;
			break;
		default:
			fprintf(stderr, "Incorrect argument\n");
			exit(1);
		}
	}

	while (optind < argc) {
		if (pfile == NULL) {
			pfile = argv[optind];
			optind++;
			break;
		}
		if (lfile == NULL) {
			lfile = argv[optind];
			optind++;
			break;
		}
		fprintf(stderr, "unrecognized additional parameters\n");
		fprintf(stderr, "compilation terminated\n");
		exit(1);
	}

	if (pfile == NULL) {
		fprintf(stderr, "no input files\n");
		fprintf(stderr, "compilation terminated.\n");
		exit(1);
	}

	if (errflag) {
		fprintf(stderr, "try %s -h  for help\n", argv[0]);
		exit(1);
	}

	/** 
	invoke the parser 
	*/
	return start_parser(pfile, lfile);
}

void print_help() {
	fprintf(stdout, "\nSwift Fox Compiler (sfc)\n");
	fprintf(stdout, "Compiles Swift Fox programs into Fennec Fox system.\n");
	fprintf(stdout, "\nUsage sfc [-vhd] [-p] <program file> [-l] <extra sfc library>\n");
	fprintf(stdout, "Options:\n");
	fprintf(stdout, "  %-10s %s\n", "-v", "Display compiler version");
	fprintf(stdout, "  %-10s %s\n", "-h", "Display compiler help information");
	fprintf(stdout, "  %-10s %s\n", "-d", "Compiler with debug statements");
	fprintf(stdout, "  %-10s %s\n", "-s <file>", "Specify Swift Fox program source file");
	fprintf(stdout, "  %-10s %s\n", "-l <file>", "Specify Swift Fox library file");
	fprintf(stdout, "\n\n");
}

void print_version() {
	fprintf(stdout, "\nSwift Fox Compiler version 0.9\n\n");
}
