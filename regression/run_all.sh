#!/bin/bash
# Swift Fox Compiler v0 .2
# Authors: Vasileios P. Kemerlis, Marcin Szczodrak
# Date: December 4, 2012

LEX_TEST="./test_lex"
PARSE_TEST="./test_parse"
SEM_TEST="./test_sem"

chmod +x $LEX_TEST
chmod +x $PARSE_TEST
chmod +x $SEM_TEST

$LEX_TEST
$PARSE_TEST
$SEM_TEST

