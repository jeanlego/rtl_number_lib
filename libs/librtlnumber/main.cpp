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

#define bad_ops(test) _bad_ops(test, __func__, __LINE__)
inline static RTL_INT _bad_ops(std::string test, const char *FUNCT, int LINE)	
{	
	std::cerr << "INVALID INPUT OPS: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;		
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
							(op == "&")		?		V_AND(a):
							(op == "|")		?		V_OR(a):
							(op == "^")		?		V_XOR(a):
							(op == "~&")	?		V_NAND(a):
							(op == "~|")	?		V_NOR(a):
							(op == "~^"	
							|| op == "^~")	?		V_XNOR(a):
							(op == "!")		?		V_LOGICAL_NOT(a):
													bad_ops(op));

}

std::string arithmetic(std::string a_in, std::string op, std::string b_in)
{
	std::vector<std::string> a = standardize_input(a_in);
	std::vector<std::string> b = standardize_input(b_in);
	
	/* return Process Operator via ternary */
	return	v_bin_string(	/*	Reduction Ops	*/
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

int get_next_token_len(std::string& input)
{
	std::string tok_3 = input.substr(0,3);
	std::string tok_2 = input.substr(0,2);
	std::string tok_1 = input.substr(0,1);

	if		(tok_1 == "(")			return 1;
	else if	(tok_1 == ")")			return 1;
	else if	(tok_1 == "?")			return 1;
	else if	(tok_1 == ":")			return 1;
	else if	(tok_3 == "===")		return 3;
	else if	(tok_3 == "!==")		return 3;
	else if	(tok_3 == "<<<")		return 3;
	else if	(tok_3 == ">>>")		return 3;
	else if	(tok_2 == "==")			return 2;
	else if	(tok_2 == "!=")			return 2;
	else if	(tok_2 == "<<")			return 2;
	else if	(tok_2 == ">>")			return 2;
	else if	(tok_2 == "&&")			return 2;
	else if	(tok_2 == "||")			return 2;
	else if	(tok_2 == "~^")			return 2;
	else if	(tok_2 == "~|")			return 2;
	else if	(tok_2 == "~&")			return 2;
	else if	(tok_2 == "^~")			return 2;
	else if	(tok_2 == "<=")			return 2;
	else if	(tok_2 == ">=")			return 2;
	else if	(tok_2 == "**")			return 2;
	else if	(tok_2 == "++")			return 2;
	else if	(tok_2 == "--")			return 2;
	else if	(tok_1 == "&")			return 1;
	else if	(tok_1 == "|")			return 1;
	else if	(tok_1 == "^")			return 1;
	else if	(tok_1 == "<")			return 1;
	else if	(tok_1 == ">")			return 1;
	else if	(tok_1 == "+")			return 1;
	else if	(tok_1 == "-")			return 1;
	else if	(tok_1 == "*")			return 1;
	else if	(tok_1 == "/")			return 1;
	else if	(tok_1 == "%")			return 1;
	else							return 0;
}

std::vector<std::string> process_input(std::string& input)
{
	//get math operation
	int bracket_current_count =0;
	std::vector<std::string> current_math;
	while(input.length())
	{
		//fetch operator here
		std::string to_return = "";
		bool is_operator = false;
		int len = get_next_token_len(input);
		if(len)
		{
			if(len == 1 && input[0] == '(')
				bracket_current_count++;
			else if(len == 1 && input[0] == ')')
				bracket_current_count--;		
			
			is_operator = true;
			to_return = input.substr(0,len);
			input.erase(0,len);
		}
		//it is a number
		else
		{
			while(input.length())
			{
				int len = get_next_token_len(input);
				if(len)
					break;

				to_return += input.substr(0,1);
				input.erase(0,1);
			}
		}
		current_math.push_back(to_return);
	}
	if(bracket_current_count)
		std::cout << "bracket are not all matched !";
	return current_math;
}

int main(int argc, char** argv) 
{ 
	//concatenate strings
	std::string args;
	for(int i=1; i < argc; i++)		args.append(argv[i]);
	args.erase(std::remove(args.begin(), args.end(), ' '), args.end());
	std::vector<std::string> input_tokenized = process_input(args);
	
	int i=0;
	while(input_tokenized.size() > 1)
	{
		//unary operation
		if(get_next_token_len(input_tokenized[0]) 
		&& 1 < input_tokenized.size() && !get_next_token_len(input_tokenized[1]) )
		{
			input_tokenized[0] = arithmetic(input_tokenized[0], input_tokenized[1]);
			input_tokenized.erase(input_tokenized.begin()+1);
		}
		//binary operation
		else if(!get_next_token_len(input_tokenized[0])
		&& 1 < input_tokenized.size() && get_next_token_len(input_tokenized[1]) 
		&& 2 < input_tokenized.size() && !get_next_token_len(input_tokenized[2]) 
		&& input_tokenized[1] != "?")
		{
			input_tokenized[0] = arithmetic(input_tokenized[0], input_tokenized[1], input_tokenized[2]);
			input_tokenized.erase(input_tokenized.begin()+1);
			input_tokenized.erase(input_tokenized.begin()+2);
		}
		//ternary operation
		else if(!get_next_token_len(input_tokenized[0])
		&& 1 < input_tokenized.size() && get_next_token_len(input_tokenized[1]) 
		&& 2 < input_tokenized.size() && !get_next_token_len(input_tokenized[2]) 
		&& 3 < input_tokenized.size() && get_next_token_len(input_tokenized[3]) 
		&& 4 < input_tokenized.size() && !get_next_token_len(input_tokenized[4]) )
		{
			input_tokenized[0] = arithmetic(input_tokenized[0], input_tokenized[1], input_tokenized[2], input_tokenized[3], input_tokenized[4]);
			input_tokenized.erase(input_tokenized.begin()+1);
			input_tokenized.erase(input_tokenized.begin()+2);
			input_tokenized.erase(input_tokenized.begin()+3);
			input_tokenized.erase(input_tokenized.begin()+4);
		}
		else
		{
			std::cout << "error parsing your line";
			break;
		}
	}
	std::string output = "";
	for (auto const& s : input_tokenized) { output += s; }
	std::cout << output;

	return 0;
}
