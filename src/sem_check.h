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

#ifndef __SEM_CHECK_H__
#define __SEM_CHECK_H__

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include "traverse.h"
#include "sf.h"

/* adapted from code_gen.h for semantic checking */

void init_sem_conf(void);
void init_sem_evt(void);
void checkConfiguration(struct confnode *c);
void checkConfigurationModules(struct confnode *c);
void checkPolicy(struct policy *p);
void updateStatesWithEvents(struct policy *p);
void checkInitial(struct initnode *i);
void checkControlState(void);
void checkState(struct statenode *s);
void addConfState(struct confnode *c);

#endif
