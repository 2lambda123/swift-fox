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

#ifndef _SF_UTILS_H_
#define _SF_UTILS_H_

#include "sf.h"

char *type_name(int type_value);
char *get_full_path(char *ffsrc_relative);
char *get_sfc_path(char *file1, char *file2);
int create_dir(char *ffsrc_relative);
int get_policy_mask(struct policy *p);

#endif
