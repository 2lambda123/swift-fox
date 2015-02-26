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

void initGlobalDataValues();
void initLocalDataValues();
void initSharedDataValues();

void initGlobalDataH();
void initLocalDataH();
void initSharedDataH();

void generateVariable(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen);
void addGlobalVariable(struct variable *sh);
void addLocalVariable(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen);

void setGlobalVariableValue(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen);
void setSharedVariableValue(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen);
void setLocalVariableValue(struct variable *sh, struct confnode* current_process_gen, struct modtab* current_module_gen);

void globalDataMsgH();

void finishGlobalDataH();
void finishLocalDataH();
void finishSharedDataH();

void finishGlobalDataValues();
void finishLocalDataValues();
void finishSharedDataValues();

void switchGlobalToLocalDataStorage();

void generateVariableConstants();

void finishCodeGeneration(int policy_counter);

void generateCaches(int event_counter, int policy_counter);

void generateModules();

void generateInitial(struct initnode *i);

void generateFennecEngineC();

void generateFennecEngineP();

void generateParams();
void generateGlobals();

void checkModuleParameters(struct modtab* mp);

void setProcessesLookupTable();

#endif
