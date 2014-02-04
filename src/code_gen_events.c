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

