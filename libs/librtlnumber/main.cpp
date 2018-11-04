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

#include "rtl_int.h"
#include "rtl_utils.h"

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

static std::string arithmetic(std::string op, std::string a_in)
{
	std::string a = verilog_string_to_bitstring(a_in);
	
	/* return Process Operator via ternary */
	return	bitstring_to_verilog_string(
							(op == "~")		?		V_BITWISE_NOT(a):
							(op == "-")		?		V_MINUS(a):
							(op == "+")		?		V_ADD(a):
							(op == "&")		?		V_BITWISE_AND(a):
							(op == "|")		?		V_BITWISE_OR(a):
							(op == "^")		?		V_BITWISE_XOR(a):
							(op == "~&")	?		V_BITWISE_NAND(a):
							(op == "~|")	?		V_BITWISE_NOR(a):
							(op == "~^"	
							|| op == "^~")	?		V_BITWISE_XNOR(a):
							(op == "!")		?		V_LOGICAL_NOT(a):
													bad_ops(op));

}

static std::string arithmetic(std::string a_in, std::string op, std::string b_in)
{
	std::string a = verilog_string_to_bitstring(a_in);
	std::string b = verilog_string_to_bitstring(b_in);
	
	/* return Process Operator via ternary */
	return	bitstring_to_verilog_string(	/*	Reduction Ops	*/
							(op == "&")		?		V_BITWISE_AND(a,b):
							(op == "|")		?		V_BITWISE_OR(a,b):
							(op == "^")		?		V_BITWISE_XOR(a,b):
							(op == "~&")	?		V_BITWISE_NAND(a,b):
							(op == "~|")	?		V_BITWISE_NOR(a,b):
							(op == "~^"	
							|| op == "~^")	?		V_BITWISE_XNOR(a,b):
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

static std::string arithmetic(std::string a_in, std::string op1 ,std::string b_in, std::string op2, std::string c_in)
{
	std::string a = verilog_string_to_bitstring(a_in);
	std::string b = verilog_string_to_bitstring(b_in);
	std::string c = verilog_string_to_bitstring(c_in);
	
	 /* return Process Operator via ternary */
	return	bitstring_to_verilog_string(	(op1 != "?")	?	bad_ops(op1):
							(op2 != ":")	?	bad_ops(op2):
												V_TERNARY(a,b,c));
}

int main(int argc, char** argv) 
{ 
	std::vector<std::string> input;
	for(int i=0; i < argc; i++)		input.push_back(argv[i]);

	if(argc == 3 && input[1] == "is_true")	std::cout << (V_TRUE(verilog_string_to_bitstring(input[2]))?"pass":"fail") << std::endl;
	else if		(argc == 3)	std::cout << arithmetic(input[1], input[2]) << std::endl;
	else if	(argc == 4)	std::cout << arithmetic(input[1], input[2], input[3]) << std::endl;
	else if (argc == 6)	std::cout << arithmetic(input[1], input[2], input[3], input[4], input[5]) << std::endl;
	else				
	{
		std::cout << "ERROR: Too Many Arguments: " << std::to_string(argc - 1) << "!" << std::endl;
		return -1;
	}

	return 0;
}
