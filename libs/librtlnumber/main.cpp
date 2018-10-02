#include <iostream>
#include <vector>
#include <string>

#include "rtl_int.h"

#define bad_ops(test) _bad_ops(test, __func__, __LINE__)
inline static RTL_INT _bad_ops(std::string test, const char *FUNCT, int LINE)	
{	
	std::cout << "INVALID INPUT OPS: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;		
	return standardize_input("1'bx"); 
}


/***
 *     __   __       ___  __   __           ___       __       
 *    /  ` /  \ |\ |  |  |__) /  \ |       |__  |    /  \ |  | 
 *    \__, \__/ | \|  |  |  \ \__/ |___    |    |___ \__/ |/\| 
 *                                                             
 * 	This is used for testing purposes only, unused in ODIN as the input is already preprocessed
 */

std::string arithmetic(std::string op, std::string a_in);
std::string arithmetic(std::string a_in, std::string op, std::string b_in);
std::string arithmetic(std::string a_in, std::string op1 ,std::string b_in, std::string op2, std::string c_in);

std::string arithmetic(std::string op, std::string a_in)
{
	std::vector<std::string> a = standardize_input(a_in);

	/* return Process Operator via ternary */
	return	v_bin_string(	(op == "~")		?		V_NEG(a):
							(op == "-")		?		V_MINUS(a):
							(op == "+")		?		V_ADD(a):
							(op == "++")	?		V_PLUS_PLUS(a):
							(op == "--")	?		V_MINUS_MINUS(a):
							(op == "&")		?		V_REDUCTION_AND(a):
							(op == "|")		?		V_REDUCTION_OR(a):
							(op == "^")		?		V_REDUCTION_XOR(a):
							(op == "~&")	?		V_REDUCTION_NAND(a):
							(op == "~|")	?		V_REDUCTION_NOR(a):
							(op == "~^"	
							|| op == "~^")	?		V_REDUCTION_XNOR(a):
							(op == "!")		?		V_LOGICAL_NOT(a):
													bad_ops(op));

}

std::string arithmetic(std::string a_in, std::string op, std::string b_in)
{
	std::vector<std::string> a = standardize_input(a_in);
	std::vector<std::string> b = standardize_input(b_in);
	
	/* return Process Operator via ternary */
	return	v_bin_string(	/*	Reduction Ops	*/
							(op == "&")		?		V_REDUCTION_AND(a,b):
							(op == "|")		?		V_REDUCTION_OR(a,b):
							(op == "^")		?		V_REDUCTION_XOR(a,b):
							(op == "~&")	?		V_REDUCTION_NAND(a,b):
							(op == "~|")	?		V_REDUCTION_NOR(a,b):
							(op == "~^"	
							|| op == "~^")	?		V_REDUCTION_XNOR(a,b):
							/*	Case test	*/
							(op == "===" )	?		V_CASE_EQUAL(a,b):
							(op == "!==")	?		V_CASE_NOT_EQUAL(a,b):
							/*	Shift Operator	*/
							(op == "<<")	?		V_SHIFT_LEFT(a,b):
							(op == "<<<")	?		V_SIGNED_SHIFT_LEFT(a,b):
							(op == ">>")	?		V_SHIFT_RIGHT(a,b):
							(op == ">>>")	?		V_SIGNED_SHIFT_LEFT(a,b):
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

std::string arithmetic(std::string a_in, std::string op1 ,std::string b_in, std::string op2, std::string c_in)
{
	std::vector<std::string> a = standardize_input(a_in);
	std::vector<std::string> b = standardize_input(b_in);
	std::vector<std::string> c = standardize_input(c_in);
	
	 /* return Process Operator via ternary */
	return	v_bin_string(	(op1 != "?")	?	bad_ops(op1):
							(op2 != ":")	?	bad_ops(op2):
												V_TERNARY(a,b,c));
}

int main(int argc, char** argv) 
{ 

	if		(argc == 3)	std::cout << arithmetic(argv[1], argv[2]) << std::endl;
	else if	(argc == 4)	std::cout << arithmetic(argv[1], argv[2], argv[3]) << std::endl;
	else if (argc == 6)	std::cout << arithmetic(argv[1], argv[2], argv[3], argv[4], argv[5]) << std::endl;
	else				std::cout << "ERROR: Too Many Arguments: " << std::to_string(argc - 1) << "!" << std::endl;

	return 0;
}
