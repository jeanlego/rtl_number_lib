#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

#include "rtl_int.h"
#include "rtl_utils.h"

/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *           Dr. Kenneth B. Kent (ken@unb.ca)
 *           for the Reconfigurable Computing Research Lab at the
 *           Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

void start_odin_ii();

#define bad_ops(test) _bad_ops(test, __func__, __LINE__)
inline static std::string _bad_ops(std::string test, const char *FUNCT, int LINE)	
{	
	std::cerr << "INVALID INPUT OPS: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;		
	return "x"; 
}


/***
 *     __   __       ___  __   __           ___       __       
 *    /  ` /  \ |\ |  |  |__) /  \ |       |__  |    /  \ |  | 
 *    \__, \__/ | \|  |  |  \ \__/ |___    |    |___ \__/ |/\| 
 *                                                             
 * 	This is used for testing purposes only, unused in ODIN as the input is already preprocessed
 */

static std::string arithmetic(std::string a_in, std::string op, std::string b_in)
{
	std::string a = verilog_string_to_bitstring(a_in);
	std::string b = verilog_string_to_bitstring(b_in);
	
	/* return Process Operator via ternary */
	return	bitstring_to_verilog_string(	/*	Reduction Ops	*/
							(op == "&")		?		V_AND(a,b):
							(op == "|")		?		V_OR(a,b):
							(op == "^")		?		V_XOR(a,b):
							(op == "~&")	?		V_NAND(a,b):
							(op == "~|")	?		V_NOR(a,b):
							(op == "~^"	
							|| op == "~^")	?		V_XNOR(a,b):
							/*	Case test	*/
							(op == "===" )	?		V_CASE_EQUAL(a,b):
							(op == "!==")	?		V_CASE_NOT_EQUAL(a,b):
							/*	Shift Operator	*/
							(op == "<<")	?		V_SHIFT_LEFT(a,b):
							(op == "<<<")	?		V_SIGNED_SHIFT_LEFT(a,b):
							(op == ">>")	?		V_SHIFT_RIGHT(a,b):
							(op == ">>>")	?		V_SIGNED_SHIFT_RIGHT(a,b):
							/* Logical Operators */
							(op == "&&")	?		V_LOGICAL_AND(a,b):
							(op == "||")	?		V_LOGICAL_OR(a,b):
							(op == "<")		?		V_LT(a,b):																																													
							(op == ">")		?		V_GT(a,b):
							(op == "<=")	?		V_LE(a,b):
							(op == ">=")	?		V_GE(a,b):
							(op == "==")	?		V_EQUAL(a,b):
							(op == "!=")	?		V_NOT_EQUAL(a,b):
							/* arithmetic Operators */																
							(op == "+")		?		V_ADD(a,b):
							(op == "-")		?		V_MINUS(a,b):
							(op == "*")		?		V_MULTIPLY(a,b):
							(op == "**")	?		V_POWER(a,b):
							/* cannot div by 0 */
							(op == "/")		?		V_DIV(a,b):
							(op == "%")		?		V_MOD(a,b):																																																																																																																																																																																																															
													bad_ops(op));
}

void
start_odin_ii()
{
	printf("ODIN_II: main.cpp: start_odin_ii(): Init:\n");
	printf("ODIN_II: main.cpp: start_odin_ii(): std::string result = arithmetic(\"1\", \"+\", \"1\");\n");

	std::string result = arithmetic("1", "+", "1");

	printf("ODIN_II: main.cpp: start_odin_ii(): result: %s\n", result.c_str());
	printf("ODIN_II: main.cpp: start_odin_ii(): Done.\n");
	return;
}

int
main()
{
	// vtr::ScopedFinishTimer t("Odin II");
	start_odin_ii();
	// terminate_odin_ii();
	return 0;

}
