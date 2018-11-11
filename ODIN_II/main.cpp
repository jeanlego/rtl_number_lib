/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "v_number.h"
#include "rtl_int.h"
#include "rtl_utils.h"

void start_odin_ii();

#define bad_ops(test) _bad_ops(test, __func__, __LINE__)
inline static std::string _bad_ops(std::string test, const char *FUNCT, int LINE)	
{	
	std::cerr << "INVALID INPUT OPS: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
	std::abort();
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
	v_number v_number_a(a_in);
	v_number v_number_b(b_in);
	
	/* return Process Operator via ternary */
	v_number v_number_result/* TODO: Get return as v_number itself */(	/*	Reduction Ops	*/
							(op == "&")		?		V_BITWISE_AND(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "|")		?		V_BITWISE_OR(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "^")		?		V_BITWISE_XOR(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "~&")	?		V_BITWISE_NAND(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "~|")	?		V_BITWISE_NOR(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "~^"	
							|| op == "~^")	?		V_BITWISE_XNOR(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							/*	Case test	*/
							(op == "===" )	?		V_CASE_EQUAL(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "!==")	?		V_CASE_NOT_EQUAL(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							/*	Shift Operator	*/
							(op == "<<")	?		V_SHIFT_LEFT(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "<<<")	?		V_SIGNED_SHIFT_LEFT(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == ">>")	?		V_SHIFT_RIGHT(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == ">>>")	?		V_SIGNED_SHIFT_RIGHT(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							/* Logical Operators */
							(op == "&&")	?		V_LOGICAL_AND(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "||")	?		V_LOGICAL_OR(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "<")		?		V_LT(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):																																													
							(op == ">")		?		V_GT(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "<=")	?		V_LE(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == ">=")	?		V_GE(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "==")	?		V_EQUAL(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "!=")	?		V_NOT_EQUAL(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							/* arithmetic Operators */																
							(op == "+")		?		V_ADD(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "-")		?		V_MINUS(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "*")		?		V_MULTIPLY(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "**")	?		V_POWER(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							/* cannot div by 0 */
							(op == "/")		?		V_DIV(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):
							(op == "%")		?		V_MOD(/* TODO: Pass in v_number itself */v_number_a.to_bitstring(), /* TODO: Pass in v_number itself */v_number_b.to_bitstring()):																																																																																																																																																																																																															
													bad_ops(op));
	return v_number_result.to_bitstring();
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
