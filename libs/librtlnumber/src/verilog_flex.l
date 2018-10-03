%{
/*
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


#include <stdio.h>
#include <string.h>
#include "types.h"
#include "globals.h"
#include "verilog_bison.h"
#include "vtr_util.h"
#include <string>
#include <algorithm>
#include "string.h"

/* the define below helps with watching the parser go token by token */
#define MP {if (to_view_parse) {printf("%d %s\n", yylineno, yytext);}}
char* standardize_number(const char* input);

%}

%s COMMENT

%option noyywrap
%option nounput
%option noinput
%option never-interactive
%option nounistd

%%

	/* Operators */
<INITIAL>"**"				{ MP; return voPOWER;}
<INITIAL>"&&"				{ MP; return voANDAND;}
<INITIAL>"||"				{ MP; return voOROR;}
<INITIAL>"<="				{ MP; return voLTE;}
<INITIAL>">="				{ MP; return voGTE;}
<INITIAL>"<<"				{ MP; return voSLEFT;}
<INITIAL>"<<<"				{ MP; return voSSLEFT;}
<INITIAL>">>"				{ MP; return voSRIGHT;}
<INITIAL>">>>"				{ MP; return voSSRIGHT;}
<INITIAL>"=="				{ MP; return voEQUAL;}
<INITIAL>"!="				{ MP; return voNOTEQUAL;}
<INITIAL>"==="				{ MP; return voCASEEQUAL;}
<INITIAL>"!=="				{ MP; return voCASENOTEQUAL;}
<INITIAL>"^~"				{ MP; return voXNOR;}
<INITIAL>"~^"				{ MP; return voXNOR;}
<INITIAL>"~&"				{ MP; return voNAND;}
<INITIAL>"~|"				{ MP; return voNOR;}
<INITIAL>"++"				{ MP; return voINCREMENT;}
<INITIAL>"--"				{ MP; return voDECREMENT;}

	/*	signed numbers	*/
<INITIAL>[0-9]+'[sS][bB][0-1_]*						{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_BINARY; }
<INITIAL>[0-9]+'[sS][hH][a-fA-F0-9_]*				{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_HEXADECIMAL; }
<INITIAL>[0-9]+'[sS][oO][0-7_]*						{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_OCTAL; }
<INITIAL>[0-9]+'[sS][dD][0-9_]*						{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_DECIMAL; }
<INITIAL>'[sS][bB][0-1_]*							{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_BINARY; }
<INITIAL>'[sS][hH][a-fA-F0-9_]*						{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_HEXADECIMAL; }
<INITIAL>'[sS][oO][0-7_]*							{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_OCTAL; }
<INITIAL>'[sS][dD][0-9_]*							{ MP; yylval.num_value = standardize_input(yytext); return vSIGNED_DECIMAL; }

	/*	unsigned Numbers	*/
<INITIAL>[0-9]+'[bB][0-1_]*							{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_BINARY; }
<INITIAL>[0-9]+'[hH][a-fA-F0-9_]*					{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_HEXADECIMAL; }
<INITIAL>[0-9]+'[oO][0-7_]*							{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_OCTAL; }
<INITIAL>[0-9]+'[dD][0-9_]*							{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_DECIMAL; }
<INITIAL>'[bB][0-1_]*								{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_BINARY; }
<INITIAL>'[hH][a-fA-F0-9_]*							{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_HEXADECIMAL; }
<INITIAL>'[oO][0-7_]*								{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_OCTAL; }
<INITIAL>'[dD][0-9_]*								{ MP; yylval.num_value = standardize_input(yytext); return vUNSIGNED_DECIMAL; }

<INITIAL>[0-9_]+									{ MP; yylval.num_value = standardize_input(yytext); return vINTEGRAL; }
<INITIAL>[0-9_]+.[0-9_]+							{ MP; yylval.num_value = standardize_input(yytext); return vREAL; }

	/* return operators */
<INITIAL>[}{;:\[\],()#=.@&!?<>%|^~+*/-]				{ MP; return yytext[0]; }

	/* general stuff */
<INITIAL>[\f\r\t\b ]+								{ continue;}					/* ignore spaces */
<INITIAL,COMMENT>"\n"								{ yylineno++; continue;}		/* catch lines */
<INITIAL>`timescale.*$              				{ MP; continue;}

	/* Get the comments */
<INITIAL>"/*"										{ MP; BEGIN COMMENT; continue;} /* begin state where we eat everything for comments*/
<COMMENT>.											{ continue;}
<COMMENT>"*/"										{ BEGIN 0; continue;}
<INITIAL>"//"[^\n]*									{ MP; continue;}


<*>.|\n		{ printf("Missing verilog.l rule: Default rule invoked in state %d: \"%s\"\n", YY_START, yytext);printf("Line %d: %s\n", yylineno, yytext); }

%%

char* standardize_number(const char* input)
{
	std::string input_number(input);
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());
	return vtr::strdup(input_number.c_str());
}
