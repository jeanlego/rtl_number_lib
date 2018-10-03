%{
/*
Copyright (c) 2009 Peter Andrew Jamieson (jamieson.peter@gmail.com)

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "rtl_int.hpp"

#define PARSE {printf("here\n");}
extern int yylineno;
#ifndef YYLINENO
	#define YYLINENO yylineno
#endif

void yyerror(const char *str){	fprintf(stderr,"error in parsing: %s - on line number %d\n",str, yylineno);	exit(-1);}
int yywrap(){	return 1;}
int yyparse();
int yylex(void);


%}

%union{
	RTL_INT num_value;
	std::string node;
}

%token <num_value> vINTEGRAL vREAL
%token <num_value> vUNSIGNED_DECIMAL vUNSIGNED_OCTAL vUNSIGNED_HEXADECIMAL vUNSIGNED_BINARY
%token <num_value> vSIGNED_DECIMAL vSIGNED_OCTAL vSIGNED_HEXADECIMAL vSIGNED_BINARY
%token vAND vNAND vNOR vNOT vOR vXNOR vXOR voANDAND voXNOR voNAND voNO voOROR
%token voLTE voGTE voSLEFT voSSLEFT voSRIGHT voSSRIGHT voEQUAL voNOTEQUAL voCASEEQUAL voCASENOTEQUAL
%token '?' ':' '|' '^' '&' '<' '>' '+' '-' '*' '/' '%' '(' ')'

%right '?' ':'
%left voOROR
%left voANDAND
%left '|'
%left '^' voXNOR voNOR
%left '&' voNAND
%left voEQUAL voNOTEQUAL voCASEEQUAL voCASENOTEQUAL
%left voGTE voLTE '<' '>'
%left voSLEFT voSRIGHT voSSLEFT voSSRIGHT
%left '+' '-'
%left '*' '/' '%'
%right voPOWER
%left '~' '!'
%left UOR
%left UAND
%left UNOT
%left UNAND
%left UNOR
%left UXNOR
%left UXOR
%left ULNOT
%left UADD
%left UMINUS

%type <node> expression primary 

%%

// 1 Source Text
source_text:
	expression												{next_parsed_verilog_file($1);}
	;

expression:
	primary													{$$ = $1;}
	| '+' expression %prec UADD								{$$ = V_ADD		($2, yylineno);}
	| '-' expression %prec UMINUS							{$$ = V_MINUS	($2, yylineno);}
	| '~' expression %prec UNOT								{$$ = V_NEG		($2, yylineno);}
	| '&' expression %prec UAND								{$$ = V_AND		($2, yylineno);}
	| '|' expression %prec UOR								{$$ = V_OR		($2, yylineno);}
	| voNAND expression %prec UNAND							{$$ = V_NAND	($2, yylineno);}
	| voNOR expression %prec UNOR							{$$ = V_NOR		($2, yylineno);}
	| voXNOR  expression %prec UXNOR						{$$ = V_XNOR	($2, yylineno);}
	| '!' expression %prec ULNOT							{$$ = V_LOGICAL_NOT($2, yylineno);}
	| '^' expression %prec UXOR								{$$ = V_XOR		($2, yylineno);}
	| expression '^' expression								{$$ = V_XOR		($1, $3, yylineno);}
	| expression voPOWER expression							{$$ = V_POWER	($1, $3, yylineno);}
	| expression '*' expression								{$$ = V_MULTIPLY($1, $3, yylineno);}
	| expression '/' expression								{$$ = V_DIV		($1, $3, yylineno);}
	| expression '%' expression								{$$ = V_MOD		($1, $3, yylineno);}
	| expression '+' expression								{$$ = V_ADD		($1, $3, yylineno);}
	| expression '-' expression								{$$ = V_MINUS	($1, $3, yylineno);}
	| expression '&' expression								{$$ = V_AND		($1, $3, yylineno);}
	| expression '|' expression								{$$ = V_OR		($1, $3, yylineno);}
	| expression voNAND expression							{$$ = V_NAND	($1, $3, yylineno);}
	| expression voNOR expression							{$$ = V_NOR		($1, $3, yylineno);}
	| expression voXNOR expression							{$$ = V_XNOR	($1, $3, yylineno);}
	| expression '<' expression								{$$ = V_LT		($1, $3, yylineno);}
	| expression '>' expression								{$$ = V_GT		($1, $3, yylineno);}
	| expression voLTE expression							{$$ = V_LE		($1, $3, yylineno);}
	| expression voGTE expression							{$$ = V_GE		($1, $3, yylineno);}
	| expression voEQUAL expression							{$$ = V_EQUAL	($1, $3, yylineno);}
	| expression voNOTEQUAL expression						{$$ = V_NOT_EQUAL	($1, $3, yylineno);}
	| expression voOROR expression							{$$ = V_LOGICAL_OR	($1, $3, yylineno);}
	| expression voANDAND expression						{$$ = V_LOGICAL_AND	($1, $3, yylineno);}
	| expression voCASEEQUAL expression						{$$ = V_CASE_EQUAL	($1, $3, yylineno);}
	| expression voCASENOTEQUAL expression					{$$ = V_CASE_NOT_EQUAL		($1, $3, yylineno);}
	| expression voSRIGHT expression						{$$ = V_SHIFT_RIGHT			($1, $3, yylineno);}
	| expression voSLEFT expression							{$$ = V_SHIFT_LEFT			($1, $3, yylineno);}
	| expression voSSRIGHT expression						{$$ = V_SIGNED_SHIFT_RIGHT	($1, $3, yylineno);}
	| expression voSSLEFT expression						{$$ = V_SIGNED_SHIFT_LEFT	($1, $3, yylineno);}
	| expression '?' expression ':' expression				{$$ = V_TERNARY				($1, $3, $5, yylineno);}
	| '(' expression ')'									{$$ = $2;}
	;

primary:
	vINTEGRAL												{$$ = newNumberNode($1, LONG_LONG, SIGNED, yylineno);}
	| vREAL													{$$ = newNumberNode($1, REAL, SIGNED, yylineno);}
	| vUNSIGNED_DECIMAL										{$$ = newNumberNode($1, DEC, UNSIGNED, yylineno);}
	| vUNSIGNED_OCTAL										{$$ = newNumberNode($1, OCT, UNSIGNED, yylineno);}
	| vUNSIGNED_HEXADECIMAL									{$$ = newNumberNode($1, HEX, UNSIGNED, yylineno);}
	| vUNSIGNED_BINARY										{$$ = newNumberNode($1, BIN, UNSIGNED, yylineno);}
	| vSIGNED_DECIMAL										{$$ = newNumberNode($1, DEC, SIGNED, yylineno);}
	| vSIGNED_OCTAL											{$$ = newNumberNode($1, OCT, SIGNED, yylineno);}
	| vSIGNED_HEXADECIMAL									{$$ = newNumberNode($1, HEX, SIGNED, yylineno);}
	| vSIGNED_BINARY										{$$ = newNumberNode($1, BIN, SIGNED, yylineno);}
	;

%%
