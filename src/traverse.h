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

#ifndef __SF_TRAVERSE_H__
#define __SF_TRAVERSE_H__

#include "sf.h"

void traverse_program(struct program* p, int f, int policy_counter);
void traverse_variables(struct variables* sh, int f);
void traverse_variable(struct variable* sh, int f);
void traverse_confnodes(struct confnodes* c, int f);
void traverse_confnode(struct confnode* c, int f);
void traverse_statenodes(struct statenodes* s, int f);
void traverse_statenode(struct statenode* s, int f);
void traverse_policies(struct policies* p, int f);
void traverse_policy(struct policy* p, int f);
void traverse_initnode(struct initnode* i, int f);

#endif
