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

#ifndef __CODE_GEN_H__
#define __CODE_GEN_H__
#define TEMP_CONF_FILE	"/tmp/tmp_conf_file_sfc"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "sf.h"
#include "traverse.h"

extern int conf_id_counter;
extern int module_id_counter;
extern int state_id_counter;

extern FILE* tmp_confs;

void setFiles();
void initCodeGeneration();

void setFennecExtra();

void startGlobalVariables();
void addGlobalVariable(struct variable *sh);
void endGlobalVariables();

void finishCodeGeneration(int policy_counter);

void generateConfiguration(struct confnode* c);

void generateCaches(int event_counter, int policy_counter);

void generateModules();

void generateDefaultParams();

void generateInitial(struct initnode *i);

void generateFennecEngineC();

void generateFennecEngineP();

void generateParams();
void generateGlobals();

#endif
