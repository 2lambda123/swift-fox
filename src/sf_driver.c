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
#include "sf.h"

/** 
the entry point to the sfc compiler
*/
int
main(int argc, char *argv[] ) {

	/** 
	argument checking 
	*/
  	if (argc != ARGC_MAX) {

		(void)fprintf(stderr, "%s: no input files\n\n", argv[0]);
		(void)fprintf(stderr, "If your source program is located in");
		(void)fprintf(stderr, " sample.sfp and sample.sfl, then run:");
  		(void)fprintf(stderr, "\n\n$ %s sample\n\n", argv[0]);
                exit(1);
       	}


	/** 
	invoke the parser 
	*/
	return start_parser(argc, argv);
}
