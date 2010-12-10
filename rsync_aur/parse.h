#ifndef _PARSE_H
#define _PARSE_H

/* ***** BEGIN LICENSE BLOCK *****
 * 
 * BBN Address and AS Number PKI Database/repository software
 * Version 1.0
 * 
 * US government users are permitted unrestricted rights as
 * defined in the FAR.  
 *
 * This software is distributed on an "AS IS" basis, WITHOUT
 * WARRANTY OF ANY KIND, either express or implied.
 *
 * Copyright (C) Raytheon BBN Technologies Corp. 2007.  All Rights Reserved.
 *
 * Contributor(s):  Peiter "Mudge" Zatko
 *
 * ***** END LICENSE BLOCK ***** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>

/*
  $Id$
*/

char *getMessageFromString(char *, unsigned int, unsigned int *, char);
char *makeWarningStr(char *, unsigned int, unsigned int *);
char *makeInfoStr(char *, unsigned int, unsigned int *);
char *makeUpdateStr(char *, unsigned int, unsigned int *);
char *makeAddStr(char *, unsigned int, unsigned int *);
char *makeLinkStr(char *, unsigned int, unsigned int *);
char *makeRemoveStr(char *, unsigned int, unsigned int *);
char *makeFatalStr(char *, unsigned int, unsigned int *);
char *makeErrorStr(char *, unsigned int, unsigned int *);
char *makeGenericStr(char *, unsigned int, unsigned int *, char);
char *makeStartStr(unsigned int *);
char *makeEndStr(unsigned int *);
int looksOK(char *, unsigned int);
int has_I_Format(char *, unsigned int);
int has_Text_Value(char *, unsigned int);
int has_Correct_Extension(char *, unsigned int);
int has_newline(char *str, unsigned int len);

/* Lower level parsing utilities. */
int endswith(const char *s, const char *suffix);
int startswith(const char *s, const char *prefix);
int exists_non_delimiter(const char *s, const char *delimiters);
char *start_of_next_field(const char *s, const char *delimiters);
char *dirname(char *dest, int dest_len, const char *path);
char *this_field(char *dest, int dest_length, const char *src,
		 const char *delimiters);
int field_length(const char *s, const char *delimiters);
long next_dirblock(FILE *fp);
int is_manifest(const char *path);

#endif
