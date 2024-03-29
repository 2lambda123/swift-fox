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


#ifndef __SF_TRAVERSE_H__
#define __SF_TRAVERSE_H__

#include "sf.h"

void traverse_program(struct program* p, int f, int policy_counter);
void traverse_variables(struct variables* sh, int f);
void traverse_variable(struct variable* sh, int f);
void traverse_processnodes(struct processnodes* c, int f);
void traverse_process(struct confnode* c, int f);
void traverse_statenodes(struct statenodes* s, int f);
void traverse_statenode(struct statenode* s, int f);
void traverse_policies(struct policies* p, int f);
void traverse_policy(struct policy* p, int f);
void traverse_initnode(struct initnode* i, int f);

#endif
