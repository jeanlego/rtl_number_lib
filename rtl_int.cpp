#include <rtl_int.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>

// this sets the default bit width
#define DEFAULT_BIT_WIDTH 0

std::vector<std::string> list_of_file_names;

/***
 *     ___  __   __   __   __      __   ___  __   __   __  ___         __  
 *    |__  |__) |__) /  \ |__)    |__) |__  |__) /  \ |__)  |  | |\ | / _` 
 *    |___ |  \ |  \ \__/ |  \    |  \ |___ |    \__/ |  \  |  | | \| \__> 
 *                                                                         
 */

#define oassert(condition) { \
	if(!(condition)) { \
		std::clog << "ASSERT FAILED: " << #condition << " \n\t@" << __FILE__<< "::" << __LINE__ << std::endl; std::abort(); \
} 	}

/*---------------------------------------------------------------------------------------------
* (function: error_message)
*-------------------------------------------------------------------------------------------*/
static void error_message(int line_number, int file, const char *message, ...)
{
	va_list ap;

	std::clog << "ERROR ";

	if (file >= 0 && file < list_of_file_names.size())
		std::clog << "@" << list_of_file_names[file];

	if (line_number > 0)
		std::clog << "::" << std::to_string(line_number);

	std::clog << " ";

	if (message)
	{
		char buf[1024];
		va_start(ap, message);
		vsprintf(buf,message, ap);
		std::clog << buf;
		va_end(ap);
	}
	std::clog << "\n";
	std::abort();
}

/*---------------------------------------------------------------------------------------------
* (function: warning_message)
*-------------------------------------------------------------------------------------------*/
static void warning_message(int line_number, int file, const char *message, ...)
{
	va_list ap;

	std::clog << "WARNING ";

	if (file >= 0 && file < list_of_file_names.size())
		std::clog << "@" << list_of_file_names[file];

	if (line_number > 0)
		std::clog << "::" << std::to_string(line_number);

	std::clog << " ";

	if (message)
	{
		char buf[1024];
		va_start(ap, message);
		vsprintf(buf,message, ap);
		std::clog << buf;
		va_end(ap);
	}
	std::clog << "\n";
}

/***
 *     __    ___     __  ___  __          __                     __        ___  __   __  
 *    |__) |  |     /__`  |  |__) | |\ | / _`    |__|  /\  |\ | |  \ |    |__  |__) /__` 
 *    |__) |  |     .__/  |  |  \ | | \| \__>    |  | /~~\ | \| |__/ |___ |___ |  \ .__/ 
 *                                                                                       
 */

#define containing_only_character_from_list(input, char_list) 	( input.find_first_not_of(char_list) == std::string::npos )
#define containing_character_from_list(input, char_list)	( input.find(char_list) == std::string::npos )
#define is_dont_care_string(input)								( containing_character_from_list(input,"XxZz") )
#define is_string_of_radix(input, radix) 		(radix == 16)?	( containing_only_character_from_list(input,"0123456789abcdefABCDEFXxZz") ) \
												:(radix == 10)?	( containing_only_character_from_list(input,"0123456789") ) \
												:(radix == 8)?	( containing_only_character_from_list(input,"01234567xXzZ") ) \
												:(radix == 2)?	( containing_only_character_from_list(input,"01xXzZ") ) \
												:false

std::string resize(std::string binary_number, int binary_limit, bool is_signed)
{
	binary_limit = binary_limit;
	//remove unecessary bits
	if(binary_limit <= 0)
	{
		while( binary_number.length() > 1 
		&& ((is_signed && binary_number[0] == binary_number[1]) 
			|| ( binary_number[0] == '0' ) 
			|| containing_only_character_from_list(binary_number,"xX")))
			binary_number.erase(0,1);
	}
	//expand to length
	else if(binary_limit > binary_number.length() )
		binary_number.insert(0,std::string(binary_limit-binary_number.length(), (!is_signed && tolower(binary_number[0]) !='x')? '0': binary_number[0]));
	
	//truncate to length
	else
		binary_number.substr(binary_number.length() - binary_limit);
}

unsigned long long v_strtoull(std::string orig_number, int radix)
{
	if (!is_string_of_radix(orig_number, radix))
		error_message(-1, -1, "Invalid base %d number: %s.\n", radix, orig_number);

	if (is_dont_care_string(orig_number))
		error_message(-1, -1, "Invalid number for conversion, contains don't care values: %s.\n", orig_number.c_str());

	if( orig_number.length() >= (sizeof(unsigned long long)*8) )
		error_message(-1, -1, "This base %d number (%s) is too long for Odin\n", radix, orig_number.c_str());

	return std::strtoull(orig_number.c_str(), NULL, radix);
}


std::string to_bitstring(std::string orig_number, int radix, int num_bits, bool is_signed);
std::string to_bitstring(std::string orig_string, int radix, int num_bits, bool is_signed)
{
	std::string result = "";	
	oassert(is_string_of_radix(orig_string,radix));

	while(orig_string != "")
	{
		switch(radix)
		{
			case 10:
			{
				std::string new_number = "";
				char prev = '0';
				short flag = 0;
				for(int i=0; i<orig_string.length(); i++)
				{
					short new_pair = ((prev-'0')%2)*10 + (orig_string[i]-'0');
					std::string div(std::to_string(new_pair/2));
					std::string rem(std::to_string(new_pair%2));

					prev = orig_string[i];
					if(div != "0" && !flag)
						flag = 1;

					if(flag)
						new_number.append(div);
					
					if(i == orig_string.length()-1)
					{
						result.insert(0, rem);
						orig_string = new_number;
					}
				}
				break;
			}
			case 2:
			{
				switch(tolower(orig_string[0]))
				{
					case '0': result.append("0");	break;
					case '1': result.append("1");	break;
					case 'x': result.append("x");	break;
					case 'z': result.append("z");	break;
					default:  oassert(0);			break;
				}
				orig_string.erase(0,1);
				break;
			}
			case 8:
			{
				switch(tolower(orig_string[0]))
				{
					case '0': result.append("000");	break;
					case '1': result.append("001");	break;
					case '2': result.append("010");	break;
					case '3': result.append("011");	break;
					case '4': result.append("100");	break;
					case '5': result.append("101");	break;
					case '6': result.append("110");	break;
					case '7': result.append("111");	break;
					case 'x': result.append("xxx");	break;
					case 'z': result.append("zzz");	break;
					default:  oassert(0);			break;
				}
				orig_string.erase(0,1);
				break;
			}
			case 16:
			{
				switch(tolower(orig_string[0]))
				{
					case '0': result.append("0000");	break;
					case '1': result.append("0001");	break;
					case '2': result.append("0010");	break;
					case '3': result.append("0011");	break;
					case '4': result.append("0100");	break;
					case '5': result.append("0101");	break;
					case '6': result.append("0110");	break;
					case '7': result.append("0111");	break;
					case '8': result.append("1000");	break;
					case '9': result.append("1001");	break;
					case 'a': result.append("1010");	break;
					case 'b': result.append("1011");	break;
					case 'c': result.append("1100");	break;
					case 'd': result.append("1101");	break;
					case 'e': result.append("1110");	break;
					case 'f': result.append("1111");	break;
					case 'x': result.append("xxxx");	break;
					case 'z': result.append("zzzz");	break;
					default:  oassert(0);				break;
				}
				orig_string.erase(0,1);
				break;
			}
			default:
			{
				error_message(-1, -1, "Invalid base %d number: %s.\n", radix, orig_string);
				break;
			}
		}
	}

	return resize(result, num_bits, false);
}

std::string cleanup_input (std::string input_number)
{	
	//remove underscores
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());
	
	auto loc = input_number.find("\'");
	if(loc == std::string::npos)
	{
		input_number.insert(0, "\'sd");
		loc = 0;
	}

	int bin_length = (loc == 0)? DEFAULT_BIT_WIDTH: std::strtoul(input_number.substr(0,loc).c_str(),NULL,10);
	std::string appendage("b'");
	if(bin_length)
		appendage.insert(0,std::to_string(bin_length));

	bool is_signed = false;
	if(std::tolower(input_number[loc+1]) == 's')
	{
		input_number.erase (input_number.begin()+loc+1);
		is_signed = true;
	}

	switch(std::tolower(input_number[loc+1]))
	{
		case 'b':	return appendage + to_bitstring(input_number.substr(loc+2), 2, bin_length, is_signed);
		case 'o':	return appendage + to_bitstring(input_number.substr(loc+2), 8, bin_length, is_signed);
		case 'd':	return appendage + to_bitstring(input_number.substr(loc+2), 10, bin_length, is_signed);
		case 'h':	return appendage + to_bitstring(input_number.substr(loc+2), 16, bin_length, is_signed);
		default:	error_message(-1, -1, "Invalid base (%c) number: %s.\n", input_number[loc+1], input_number);;
	}
}

/***
 *     __   __            ___         ___     __   __   ___  __       ___    __       
 *    |__) |__) |  |\/| |  |  | \  / |__     /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    |    |  \ |  |  | |  |  |  \/  |___    \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                                    
 *                                                                
 *                                                                  
 * these are taken from the raw verilog truth tables so that the evaluation are correct.
 * only use this to evaluate any expression for the number_t binary digits.
 * reference: http://staff.ustc.edu.cn/~songch/download/IEEE.1364-2005.pdf
 * 
 *******************************************************/ 

inline static const char bad_value(char test, char *FILE, int LINE)	
{	
	std::clog << "INVALID INPUT: (" << test << ")@" << __FILE__<< "::" << __LINE__ << std::endl;	
	std::abort;		
	return 0; 
}

inline static const char v_switch(	const char a, 
										const char _0, const char _1, const char  _x, const char _z,
										char *FILE, int LINE) 
{
	return 	((a) == '0')				?	_0 :
			((a) == '1')				?	_1 :
			(std::tolower(a) == 'x')	?	_x : 
			(std::tolower(a) == 'z')	?	_z :
											bad_value(a, FILE,LINE)
			;
}

inline static const char v_switch(	const char a, const char b, 
										const char _00, const char _01, const char _0x, const char _0z, 
										const char _10, const char _11, const char _1x, const char _1z, 
										const char _x0, const char _x1, const char _xx, const char _xz, 
										const char _z0, const char _z1, const char _zx, const char _zz,
										char *FILE, int LINE) 
{
	return 	((a) == '0')				? 	v_switch(b, _00, _01, _0x, _0z, FILE, LINE)	:
			((a) == '1')				? 	v_switch(b, _10, _11, _1x, _1z, FILE, LINE) 	:
			(std::tolower(a) == 'x')	?	v_switch(b, _x0, _x1, _xx, _xz, FILE, LINE)	: 
			(std::tolower(a) == 'z')	?	v_switch(b, _z0, _z1, _zx, _zz, FILE, LINE) 	:
											bad_value(a, FILE, LINE)
			;
}

/* P:111 */
#define v_buf(a)				v_switch(a,\
	/*	 	 0   1   x   z  <- a*/	\
			'0','1','x','x',		\
	__FILE__,__LINE__)

#define v_not(a)				v_switch(a,\
	/*   	 0   1   x   z 	<- a */	\
			'1','0','x','x',			\
	__FILE__,__LINE__)

#define v_and(a,b)				v_switch(a,b,\
	/* a  /	 0   1   x   z 	<-b */	\
	/* 0 */	'0','0','0','0',		\
	/* 1 */	'0','1','x','x',		\
	/* x */	'0','x','x','x',		\
	/* z */	'0','x','x','x',		\
	__FILE__,__LINE__)

#define v_or(a,b)				v_switch(a,b,\
	/* a  /	 0   1   x   z 	<-b */	\
	/* 0 */	'0','1','x','x',		\
	/* 1 */	'1','1','1','1',		\
	/* x */	'x','1','x','x',		\
	/* z */	'x','1','x','x',		\
	__FILE__,__LINE__)

#define v_xor(a,b)				v_switch(a,b,\
	/* a  /	 0   1   x   z 	<-b */	\
	/* 0 */	'0','1','x','x',		\
	/* 1 */	'1','0','x','x',		\
	/* x */	'x','x','x','x',		\
	/* z */	'x','x','x','x',		\
	__FILE__,__LINE__)

#define v_nand(a,b)				v_switch(a,b,\
	/* a  /	 0   1   x   z 	<-b */	\
	/* 0 */	'1','1','1','1',		\
	/* 1 */	'1','0','x','x',		\
	/* x */	'1','x','x','x',		\
	/* z */	'1','x','x','x',		\
	__FILE__,__LINE__)

#define v_nor(a,b)				v_switch(a,b,\
	/* a  /	 0   1   x   z 	<-b */	\
	/* 0 */	'1','0','x','x',		\
	/* 1 */	'0','0','0','0',		\
	/* x */	'x','0','x','x',		\
	/* z */	'x','0','x','x',		\
	__FILE__,__LINE__)

#define v_xnor(a,b)				v_switch(a,b,\
	/* a  /	 0   1   x   z 	<-b */	\
	/* 0 */	'1','0','x','x',		\
	/* 1 */	'0','1','x','x',		\
	/* x */	'x','x','x','x',		\
	/* z */	'x','x','x','x',		\
	__FILE__,__LINE__)

#define v_notif1(in,control)	v_switch(in,control,\
	/* in /	 0   1   x   z 	<-control */	\
	/* 0 */	'z','1','H','H',	\
	/* 1 */	'z','0','L','L',	\
	/* x */	'z','x','x','x',	\
	/* z */	'z','x','x','x',	\
	__FILE__,__LINE__)

#define v_notif0(in,control)	v_switch(in,control,\
	/* in /	 0   1   x   z 	<-control */	\
	/* 0 */	'1','z','H','H',	\
	/* 1 */	'0','z','L','L',	\
	/* x */	'x','z','x','x',	\
	/* z */	'x','z','x','x',	\
	__FILE__,__LINE__)

#define v_bufif1(in,control)	v_switch(in,control,\
	/* in /	 0   1   x   z 	<-control */	\	
	/* 0 */	'z','0','H','H',	\
	/* 1 */	'z','1','L','L',	\
	/* x */	'z','x','x','x',	\
	/* z */	'z','x','x','x',	\
	__FILE__,__LINE__)

#define v_bufif0(in,control)	v_switch(in,control,\
	/* in /	 0   1   x   z 	<-control */	\
	/* 0 */	'0','z','H','H',	\
	/* 1 */	'1','z','L','L',	\
	/* x */	'x','z','x','x',	\
	/* z */	'x','z','x','x',	\
	__FILE__,__LINE__)

/***
 * these are extended defines to simplify our lives
 */
#define add_func(a, b, carry)	v_xor(v_xor(a,b),carry)
#define carry_func(a, b, carry)	v_or(v_and(a,b),v_and(v_xor(a,b),carry))

#define OPTIMIZED 0
/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */
static std::string arithmetic(std::string op, std::string a, size_t a_length, bool a_signed)
{
	std::string result = "";
	size_t result_length = 0;
	bool result_sign = a_signed;

    if (op == "~")		// negation
	{	
		for(int i=0; i<a.length(); i++)
			result[i] = v_not(a[i]);
			}
    else if(op == "&")	// reduction AND
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length() && (!OPTIMIZED || result[0] == '1'); i++)
			result[0] = v_and(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "|")	// reduction OR
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length() && (!OPTIMIZED || result[0] != '1'); i++)
			result[0] = v_or(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "^")	// reduction XOR
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length(); i++)
			result[0] = v_xor(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "~&")	// reduction NAND
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length(); i++)
			result[0] = v_nand(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "~|")	// reduction NOR
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length() && (!OPTIMIZED || result[0] == '1'); i++)
			result[0] = v_nor(a[i], result[0]);
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "~^")	// reduction XNOR
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length(); i++)
			result[0] = v_xnor(a[i], result[0]);
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "!")	// logical NOT
	{
		result = std::string(a[0],1);
		for(int i=1; i<a.length() && (!OPTIMIZED || result[0] != '1'); i++)
			result[0] = v_or(a[i], result[0]);
		
		result[0] = v_not(result[0]);
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "-")	// minus
	{
		result = arithmetic("~", a, a.length(), true);
		result = arithmetic("++", result, result.length()+1, true);
		result_sign = true; //verify this overide
	}
    else if(op == "+")	// plus
	{
		result = a;
		result_length = a_length;
		result_sign = true;
	}
	// these are not standard verilog but simplify our lives where possible and speed up
	else if(op == "++")
	{
		char previous_carry = '1';
		for(int i=a.length()-1; i >= 0  && previous_carry == '1'; i++)
		{
			result[i+1] = v_xor(a[i], previous_carry);
			previous_carry = v_and(a[i], previous_carry);
		}
		result[0] = previous_carry;
	
		result_length = a.length()+1;
	}
	else
	{
		error_message(-1,-1,"Operator %s is invalid for unary operation", op);
    }
	return result;
}

/***
 *     __               __          __   __   ___  __       ___    __       
 *    |__) | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    |__) | | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                          
 */
static std::string arithmetic(std::string a, size_t a_length, short a_signed, std::string op, std::string b, size_t b_length, short b_signed)
{
	size_t length = std::max(a_length, b_length);
	std::string adj_a = resize(a, length, a_signed);
	std::string adj_b = resize(b, length, b_signed);
	

	std::string result = "x";
	bool result_sign = a_signed && b_signed;
	size_t result_length = length;

	if( op == "^" )
	{
		result = "0";
		for(int i=0; i<=length; i++)
			result[i] = v_xor(adj_a[i], adj_b[i]);
	}
	else if( op == "~^" || op == "^~" )
	{
		result = "0";
		for(int i=0; i<=length; i++)
			result[i] = v_xnor(adj_a[i], adj_b[i]);
	}
	else if( op == "&" )
	{
		result = "0";
		for(int i=0; i<=length; i++)
			result[i] = v_and(adj_a[i], adj_b[i]);
	}
	else if( op == "~&" )
	{
		result = "0";
		for(int i=0; i<=length; i++)
			result[i] = v_nand(adj_a[i], adj_b[i]);
	}
	else if( op == "|" )
	{
		result = "0";
		for(int i=0; i<=length; i++)
			result[i] = v_or(adj_a[i], adj_b[i]);
	}
	else if( op == "~|" )
	{
		result = "0";
		for(int i=0; i<=length; i++)
			result[i] = v_nor(adj_a[i], adj_b[i]);
	}
	else if( op == "&&" )
	{
		std::string res_a = arithmetic("|", a, a_length, a_signed);
		std::string res_b = arithmetic("|", b, b_length, b_signed);
		result = arithmetic(res_a, 1, false, "&", res_b, 1, false);
		result_length = 1;
		result_sign = false;
	}
	else if( op == "||" )
	{
		std::string res_a = arithmetic("|", a, a_length, a_signed);
		std::string res_b = arithmetic("|", b, b_length, b_signed);
		result = arithmetic(res_a, 1, false, "|", res_b, 1, false);
		result_length = 1;
		result_sign = false;
	}
	else if( op == "===" )
	{
		result = "1";
		for(int i=0; i < length && result[0] == '1'; i++)
			result[0] = (adj_a[i] == adj_b[i])? '1': '0';
		result_length = 1;
		result_sign = false;
	}
	else if( op == "!==" )
	{
		result = "0";
		for(int i=0; i < length && result[0] == '0'; i++)
			result[0] = (adj_a[i] != adj_b[i])? '1': '0';
		result_length = 1;
		result_sign = false;
	}

	else if( op == "<<" ||  op == "<<<" || op == ">>" || op == ">>>" )
	{
		if(!is_dont_care_string(adj_b))
		{
			// flip the shift if signed integer
			if(b_signed && b[b.length()-1] == '1')
			{
				std::string two_comp = arithmetic("-", b, b_length, b_signed);
				op =	(op == "<<<")	?	">>>"	:
						(op == "<<")	?	">>"	:
						(op == ">>>")	?	"<<<"	:
						/*(op == ">>")	?*/	"<<"	;
			}

			//these operation are limited to shift by the amount dictated by a long long 2^64 (doubt we will ever reach this!) 
			unsigned long long shift_by = v_strtoull(b,2);
			if( op == "<<" || op == "<<<")
			{
				// from 0 ... to n we are in big endian here
				result = a + std::string('0',shift_by); 
				result_length += shift_by;
			}
			else if(op == ">>")
			{
				result = std::string('0', shift_by) + a; 
				result_length += shift_by;
			}
			else /*if(op == ">>>" )*/
			{
				result = std::string(a[0], shift_by) + a; 
				result_length += shift_by;
			}
		}
	}
	else
	{
		/* dont care sensitive ops */
		if(!is_dont_care_string(adj_a) && !is_dont_care_string(adj_b))
		{
			// todo signed int!!
			if( op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" )
			{
				// -1 less, 0 equal, 1 greater
				short condition_code = -1; 
				for(int i=0; i < length && condition_code == 0; i++)
				{
					condition_code = 	(adj_a[i] == adj_b[i])	?	0	:
										(adj_a[i] == '0')		?	-1	:
																	1	;
				}
				result_length = 1;
				result_sign = false;

				result[0] =	( op == "<" )	?	(condition_code == -1) 	? '1': '0'	:
							( op == ">" )	?	(condition_code == 1)	? '1': '0'	:
							( op == "<=" )	?	(condition_code != 1)	? '1': '0'	:
							( op == ">=" )	?	(condition_code != -1)	? '1': '0'	:
							( op == "==" )	?	(condition_code == 0)	? '1': '0'	:
							/*( op == "!=" )?*/	(condition_code != 0)	? '1': '0'	;
			}

			if( op == "+" )
			{
				char previous_carry = '0';
				for(int i=length-1; i >= 0 ; i++)
				{
					result[i+1] = add_func(adj_a[i], adj_b[i], previous_carry);
					previous_carry = carry_func(adj_a[i], adj_b[i], previous_carry);
				}
				result[0] = previous_carry;
			
				result_length = length+1;
				result_sign = false; //TODO
			}
			else if( op == "-" )
			{
				std::string neg_b = arithmetic("-", adj_b, length, true);
				return arithmetic(adj_a, length, a_signed, "+", neg_b, length, true);
			}
			else if( op == "*" )
			{
				result = "0";
				for(int i=length-1; i>=0; i++)
				{
					if(adj_a[i] == '1')
					{
						std::string shift_by = to_bitstring(std::to_string(i),10,sizeof(i)*8,false);
						std::string shifted_result = arithmetic(adj_b, length, b_signed, "<<<", shift_by, DEFAULT_BIT_WIDTH, b_signed);
						result = arithmetic(result, 2*(length+1), a_signed&&b_signed, "+", shifted_result, 2*(length+1), b_signed);
					}
				}

			}
			else if( op == "**" )
			{
				std::string result = "1";
				while(arithmetic("|",adj_b, length, b_sign)
				{
					adj_b = arithmetic( adj_b, length, b_signed, "-", "1", 2, false);
					result = arithmetic( result, length, true, "*", adj_a, length, true);
				}
			}
			//sensitive to division by zero
			else if( '0' != arithmetic("|", adj_b, length, b_signed))
			{

				if( op == "/" || op == "%" )
				{
					result = "0";
					//TODO signed division!
					while(arithmetic(adj_b, length, b_signed, "<", adj_a, length, a_signed) ) )
					{
						std::string count = "1";
						std::string sub_with = adj_b;
						for( std::string tmp = sub_with; arithmetic(tmp, length, true, "<", adj_a, length, a_signed); arithmetic(tmp, length, true, "<<", "1", 2, true))
						{
							count = arithmetic( count, length, true, "<<", "1", 2, true);
							sub_with = tmp;
						}
						adj_a = arithmetic( adj_a, length, a_signed, "-", sub_with, length, true);
						result = arithmetic ( result, length, true, "+", count, length, true);
					}
					if( op == "%" )
					{
						result = adj_a;
					}
				}
				else
				{
					error_message(-1,-1,"Operator %s is invalid for binary operation", op);
				}
			}
		}
	}
	return "";
}

/***
 *    ___  ___  __             __          __   __   ___  __       ___    __       
 *     |  |__  |__) |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *     |  |___ |  \ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                                 
 */

/**
 * The  evaluation  of  a  conditional  operator  shall  begin  with  a  logical  equality  comparison  (see  
5.1.8
)  of
expression1 with zero, termed the 
condition
. If the condition evaluates to fa
lse (0), then expression3 shall be
evaluated  and  used  as  the  result  of  the  conditional  e
xpression.  If  the  condition  evaluates  to  true  (1),  then
expression2 is evaluated and used as the result. 
If the condition evaluates to an ambiguous value (
x
 or 
z
),
then  both  expression2  and  expression3  shall  be  evaluated;  and  their  results  shall  be  combined,  bit  by  bit,
using 
Table 5-21
  to  calculate  the  final  result  unless  expressi
on2  or  expression3  is  real,  in  which  case  the
result  shall  be  
0
.  If  the  lengths  of  expression2  and  expressi
on3  are  different,  the  shorter  operand  shall  be
lengthened to match the long
er and zero-filled from the le
ft (the high-order end)
*/

