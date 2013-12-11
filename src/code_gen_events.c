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

/* Generate Events */
void generateEventC() {

	return;

}

void generateEventP(int event_counter) {

	return;

/*

        char *full_path = get_sfc_path("", "EventsP.nc");
        FILE *fp = fopen(full_path, "w");

	struct symtab *sp;

        if (fp == NULL) {
                fprintf(stderr, "You do not have a permission to write into file: %s\n", full_path);
                exit(1);
        }

	fprintf(fp, "\n#include <Fennec.h>\n\n");
	fprintf(fp, "module EventsP {\n\n");
	fprintf(fp, "  provides interface SplitControl;\n");
	fprintf(fp, "  uses interface EventCache;\n");

	for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
		if (sp->name && sp->type != NULL && !strcmp(sp->type, "event_id")) {
			fprintf(fp, "  uses interface Event as %sEvent%d;\n", 
							sp->lib->name, sp->value);
		}
	}

	fprintf(fp, "\n}\n\n");
	fprintf(fp, "implementation {\n\n");
	fprintf(fp, "  void turnEvents(bool flag);\n");
	fprintf(fp, "  void setEvent(uint8_t ev_num, bool flag);\n\n");
	fprintf(fp, "  command error_t SplitControl.start() {\n");
	fprintf(fp, "    turnEvents(ON);\n");
	fprintf(fp, "    dbg(\"Events\", \"Events started\\n\");\n");
	fprintf(fp, "    signal SplitControl.startDone(SUCCESS);\n");
	fprintf(fp, "    return SUCCESS;\n");
	fprintf(fp, "  }\n\n");
	fprintf(fp, "  command error_t SplitControl.stop() {\n");
	fprintf(fp, "    turnEvents(OFF);\n");
	fprintf(fp, "    dbg(\"Events\", \"Events stopped\\n\");\n");
	fprintf(fp, "    signal SplitControl.stopDone(SUCCESS);\n");
	fprintf(fp, "    return SUCCESS;\n");
	fprintf(fp, "  }\n\n");
	fprintf(fp, "  void setEvent(uint8_t ev_num, bool flag) {\n\n");
	fprintf(fp, "    switch(ev_num) {\n\n");
	fprintf(fp, "      case 0:\n");
	fprintf(fp, "        break;\n\n");

	for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
		if (sp->name && sp->type != NULL && !strcmp(sp->type, "event_id")) {
			fprintf(fp, "      case %d:\n", sp->value);
			fprintf(fp, "        flag ? call %sEvent%d.start(call EventCache.getEntry(%d)) : call %sEvent%d.stop();\n", 
				sp->lib->name, sp->value, sp->value, sp->lib->name, sp->value);
			fprintf(fp, "        break;\n\n");
		}
	}

	fprintf(fp, "      default:\n");
	fprintf(fp, "        dbg(\"Events\", \"Events: there is no event with number %s\\n\", ev_num);\n", "%d");
	fprintf(fp, "    }\n");
	fprintf(fp, "  }\n\n");

	for(sp = symtab; sp < &symtab[NSYMS]; sp++) {
		if (sp->name && sp->type != NULL && !strcmp(sp->type, "event_id")) {
			fprintf(fp, "  event void %sEvent%d.occured(bool oc) {\n", 
							sp->lib->name, sp->value);
			fprintf(fp, "    oc ? call EventCache.setBit(%d) : call EventCache.clearBit(%d);\n", 
							sp->value, sp->value);
			fprintf(fp, "  }\n\n");
		}
	}

	fprintf(fp, "\n}\n");
	fclose(fp);
*/
}

