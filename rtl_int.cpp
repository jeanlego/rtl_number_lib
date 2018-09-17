#include <rtl_int.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>

std::vector<std::string> list_of_file_names;

/***
 *      ______                       _      _ _     
 *     |  ____|                     | |    (_) |    
 *     | |__   _ __ _ __ ___  _ __  | |     _| |__  
 *     |  __| | '__| '__/ _ \| '__| | |    | | '_ \ 
 *     | |____| |  | | | (_) | |    | |____| | |_) |
 *     |______|_|  |_|  \___/|_|    |______|_|_.__/ 
 *                                                  
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
 *      _____       _ _   
 *     |_   _|     (_) |  
 *       | |  _ __  _| |_ 
 *       | | | '_ \| | __|
 *      _| |_| | | | | |_ 
 *     |_____|_| |_|_|\__|
 *                        
 *                        
 */

void init_number(const char* orig_number, short radix, int num_bits)
{
	original_number = orig_number;
	binary_limit = num_bits;
	binary_number = to_bitstring(orig_number, radix, num_bits);
	is_full=0;
}

void init_number(const char* orig_number, short radix)
{

}

void init_number(const char* orig_number)
{

}


/*******************************************************
 *      _____      _           _ _   _             ______          _ 
 *     |  __ \    (_)         (_) | (_)           |  ____|        | |
 *     | |__) | __ _ _ __ ___  _| |_ ___   _____  | |____   ____ _| |
 *     |  ___/ '__| | '_ ` _ \| | __| \ \ / / _ \ |  __\ \ / / _` | |
 *     | |   | |  | | | | | | | | |_| |\ V /  __/ | |___\ V / (_| | |
 *     |_|   |_|  |_|_| |_| |_|_|\__|_| \_/ \___| |______\_/ \__,_|_|
 *                                                                   
 *                                                                  
 * these are taken from the raw verilog truth tables so that the evaluation are correct.
 * only use this to evaluate any expression for the number_t binary digits.
 * reference: http://staff.ustc.edu.cn/~songch/download/IEEE.1364-2005.pdf
 *******************************************************/ 

#define v_1d_switch(a, _0, _1, _x, _z) \
						((a) == '0')? 			_0 \
						:((a) == '1')? 			_1 \
						:(std::tolower(a) == 'x')?	_x \
						:(std::tolower(a) == 'z')?	_z \
						: 0

#define v_2d_switch(a, b, _00, _01, _0x, _0z, _10, _11, _1x, _1z, _x0, _x1, _xx, _xz, _z0, _z1, _zx, _zz) \
						((a) == '0')? 			v_1d_switch(b, _00, _01, _0x, _0z) \
						:((a) == '1')? 			v_1d_switch(b, _10, _11, _1x, _1z) \
						:(std::tolower(a) == 'x')? 	v_1d_switch(b, _x0, _x1, _xx, _xz) \
						:(std::tolower(a) == 'z')? 	v_1d_switch(b, _z0, _z1, _zx, _zz) \
						: 0


static char v_buf(const char a)
{
	char output = v_1d_switch(a,
		/*   0   1   x   z 	<-b */	
			'0','1','x','x'
	);
	oassert(output);
	return output;
};

static char v_not(const char a)
{
	char output = v_1d_switch(a,
		/*   0   1   x   z 	<-b */	
			'1','0','x','x'
	);
	oassert(output);
	return output;
};

static char v_and(const char a, const char b)
{
	char output = v_2d_switch(a,b,
		/* a  /	 0   1   x   z 	<-b */	
		/* 0 */	'0','0','0','0',
		/* 1 */	'0','1','x','x',
		/* x */	'0','x','x','x',
		/* z */	'0','x','x','x'
	);
	oassert(output);
	return output;
};

static char v_or(const char a, const char b)
{
	char output = v_2d_switch(a,b,
		/* a  /	 0   1   x   z 	<-b */	
		/* 0 */	'0','1','x','x',
		/* 1 */	'1','1','1','1',
		/* x */	'x','1','x','x',
		/* z */	'x','1','x','x'
	);
	oassert(output);
	return output;
};


static char v_xor(const char a, const char b)
{
	char output = v_2d_switch(a,b,
		/* a  /	 0   1   x   z 	<-b */	
		/* 0 */	'0','1','x','x',
		/* 1 */	'1','0','x','x',
		/* x */	'x','x','x','x',
		/* z */	'x','x','x','x'
	);
	oassert(output);
	return output;
};

static char v_nand(const char a, const char b)
{
	char output = v_2d_switch(a,b,
		/* a  /	 0   1   x   z 	<-b */	
		/* 0 */	'1','1','1','1',
		/* 1 */	'1','0','x','x',
		/* x */	'1','x','x','x',
		/* z */	'1','x','x','x'
	);
	oassert(output);
	return output;
};

static char v_nor(const char a, const char b)
{
	char output = v_2d_switch(a,b,
		/* a  /	 0   1   x   z 	<-b */	
		/* 0 */	'1','0','x','x',
		/* 1 */	'0','0','0','0',
		/* x */	'x','0','x','x',
		/* z */	'x','0','x','x'
	);
	oassert(output);
	return output;
};

static char v_xnor(const char a, const char b)
{
	char output = v_2d_switch(a,b,
		/* a  /	 0   1   x   z 	<-b */	
		/* 0 */	'1','0','x','x',
		/* 1 */	'0','1','x','x',
		/* x */	'x','x','x','x',
		/* z */	'x','x','x','x'
	);
	oassert(output);
	return output;
};

/**
 *  the next 4 implementation replaces High and Low impedance values
 * although it is not EXACT implementation it should still be correct
 * I.E if control signal is not 0 or 1, the output is x
 */ 

static char v_notif1(const char control_signal, const char input)
{
	char output = v_2d_switch(control_signal, input,
		/* cs /	 0   1   x   z 	<-input */	
		/* 0 */	'1','0','x','x',
		/* 1 */	'z','z','z','z',
		/* x */	'x','z','x','x',
		/* z */	'x','z','x','x'
	);
	oassert(output);
	return output;
};

static char g_notif0(const char control_signal, const char input)
{
	char output = v_2d_switch(control_signal, input,
		/* cs /	 0   1   x   z 	<-input */	
		/* 0 */	'z','z','z','z',
		/* 1 */	'1','0','x','x',
		/* x */	'x','x','x','x',
		/* z */	'x','x','x','x'
	);
	oassert(output);
	return output;
};

static char v_bufif1(const char control_signal, const char input)
{
	char output = v_2d_switch(control_signal, input,
		/* cs /	 0   1   x   z 	<-input */	
		/* 0 */	'0','1','x','x',
		/* 1 */	'z','z','z','z',
		/* x */	'x','z','x','x',
		/* z */	'x','z','x','x'
	);
	oassert(output);
	return output;
};

static char g_bufif0(const char control_signal, const char input)
{
	char output = v_2d_switch(control_signal, input,
		/* cs /	 0   1   x   z 	<-input */	
		/* 0 */	'z','z','z','z',
		/* 1 */	'0','1','x','x',
		/* x */	'x','x','x','x',
		/* z */	'x','x','x','x'
	);
	oassert(output);
	return output;
};

/* END of Primitive Gate Evaluators */

#define containing_only_character_from_list(input, char_list) 	( input.find_first_not_of(char_list) == std::string::npos )
#define not_containing_character_from_list(input, char_list)	( input.find(char_list) == std::string::npos )
#define is_dont_care_string(input)								( not_containing_character_from_list(input,"XxZz") )
#define is_string_of_radix(input, radix) 		(radix == 16)?	( containing_only_character_from_list(input,"0123456789abcdefABCDEFXxZz") ) \
												:(radix == 10)?	( containing_only_character_from_list(input,"0123456789") ) \
												:(radix == 8)?	( containing_only_character_from_list(input,"01234567xXzZ") ) \
												:(radix == 2)?	( containing_only_character_from_list(input,"01xXzZ") ) \
												:false

static std::string bitstring_adjust_len(std::string bitstring, int target_len, bool is_signed)
{
	std::string result = bitstring;

	//remove unecessary bits
	if(target_len <= 0)
	{
		while( result.length() > 1 && ((is_signed && result[0] == result[1]) || ( result[0] != '1' && result[0] == result[1])))
			result.erase(0,1);
	}
	//expand to length
	else if(target_len > bitstring.length())
		result.insert(0,std::string(target_len-result.length(), (is_signed)? result[0]:'0'));
	
	//truncate to length
	else
		result.substr(result.length() - target_len);
	
	return result;
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

/***
 *                    _ _   _                    _   _      
 *         /\        (_) | | |                  | | (_)     
 *        /  \   _ __ _| |_| |__  _ __ ___   ___| |_ _  ___ 
 *       / /\ \ | '__| | __| '_ \| '_ ` _ \ / _ \ __| |/ __|
 *      / ____ \| |  | | |_| | | | | | | | |  __/ |_| | (__ 
 *     /_/    \_\_|  |_|\__|_| |_|_| |_| |_|\___|\__|_|\___|
 *                                                          
 *                                                          
 */

/*---------------------------------------------------------------------------------------------
 * define Verilog unary operations
 *-------------------------------------------------------------------------------------------*/
static std::string arithmetic(std::string op, std::string a, size_t a_length, bool a_signed)
{
	
	std::string result = "";
	size_t result_length = 0;
	bool result_sign = false;

    if (op == "~")		// negation
	{	
		for(int i=0; i<=a_length; i++)
			result[i] = v_not(a[i]);
		
		result_length = a_length;
		result_sign = false;
	}
    else if(op == "&")	// reduction AND
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_and(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "|")	// reduction OR
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_or(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "^")	// reduction XOR
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_xor(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "~&")	// reduction NAND
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_nand(a[i], result[0]);		
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "~|")	// reduction NOR
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_nor(a[i], result[0]);
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "~^")	// reduction XNOR
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_xnor(a[i], result[0]);
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "!")	// logical NOT
	{
		result = std::string(a[0],1);
		for(int i=1; i<=a_length; i++)
			result[0] = v_or(a[i], result[0]);
		
		result[0] = v_not(result[0]);
		
		result_length = 1;
		result_sign = false;
	}
    else if(op == "-")	// minus
	{
		std::string zero = "0";
		size_t zero_length = 1;
		bool zero_signed = true;
		result = arithmetic(zero, zero_length, zero_signed, "-", a, a_length, true);

		result_length = a_length;
		result_sign = true;
	}
    else if(op == "+")	// plus
	{
		result = a;
		result_length = a_length;
		result_sign = true;
	}
	else
	{
		error_message(-1,-1,"Operator %s is invalid for unary operation", op);
    }
	return result;
}

/*---------------------------------------------------------------------------------------------
 * (function: calculate_binary)
 *-------------------------------------------------------------------------------------------*/
static std::string arithmetic(std::string a, size_t a_length, short a_signed, std::string op, std::string b, size_t b_length, short b_signed)
{
	size_t length = std::max(a_length, b_length);
	std::string adj_a = bitstring_adjust_len(a, length, a_signed);
	std::string adj_b = bitstring_adjust_len(b, length, b_signed);
	

	std::string result = "";
	bool result_sign = false;
	size_t result_length = 0;

		if( op == "^" )
		{
			for(int i=0; i<=length; i++)
				result[i] = v_xor(adj_a[i], adj_b[i]);
		
			result_length = length;
			result_sign = false;
		}
		else if( op == "~^" || op == "^~" )
		{
			for(int i=0; i<=length; i++)
				result[i] = v_xnor(adj_a[i], adj_b[i]);
		
			result_length = length;
			result_sign = false;
		}
		else if( op == "&" )
		{
			for(int i=0; i<=length; i++)
				result[i] = v_and(adj_a[i], adj_b[i]);
		
			result_length = length;
			result_sign = false;
		}
		else if( op == "~&" )
		{
			for(int i=0; i<=length; i++)
				result[i] = v_nand(adj_a[i], adj_b[i]);
		
			result_length = length;
			result_sign = false;
		}
		else if( op == "|" )
		{
			for(int i=0; i<=length; i++)
				result[i] = v_or(adj_a[i], adj_b[i]);
		
			result_length = length;
			result_sign = false;
		}
		else if( op == "~|" )
		{
			for(int i=0; i<=length; i++)
				result[i] = v_nor(adj_a[i], adj_b[i]);
		
			result_length = length;
			result_sign = false;
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
			result = arithmetic(res_a, 1, false, "&", res_b, 1, false);
			result_length = 1;
			result_sign = false;
		}
		else if( op == "==" )
		{
			if(is_dont_care_string(adj_a) || is_dont_care_string(adj_b))
			{
				result = "x";
			}
			else
			{
				result = "1";
				for(int i=0; i <= length && result[0] == '1'; i++)
					result[0] = (adj_a[i] == adj_b[i])? '1': '0';
			}
			result_length = 1;
			result_sign = false;
		}
		else if( op == "!=" )
		{
			result = arithmetic("!", arithmetic(a, a_length, a_signed, "==", b, b_length, b_signed),1,false);
			result_length = 1;
			result_sign = false;
		}
		else if( op == "===" )
		{
			result = "1";
			for(int i=0; i <= length && result[0] != '0'; i++)
				result[0] = (adj_a[i] == adj_b[i])? '1': '0';
		
			result_length = 1;
			result_sign = false;
		}
		else if( op == "!==" )
		{
			result = arithmetic("!", arithmetic(a, a_length, a_signed, "===", b, b_length, b_signed),1,false);
			result_length = 1;
			result_sign = false;
		}
		else if( op == "<<" ||  op == "<<<" )
		{
			if(b_signed && b[0] == '1')
			{
				std::string two_comp = arithmetic("-", b, b_length, b_signed);
				if( op == "<<<" )
					result = arithmetic(a, a_length, a_signed, ">>>", two_comp, b_length, b_signed);
				else
					result = arithmetic(a, a_length, a_signed, ">>", two_comp, b_length, b_signed);
			}
			else
			{

			}
		}
		else if( op == ">>" || op == ">>>" )
		{
			if(b_signed && b[0] == '1')
			{
				std::string two_comp = arithmetic("-", b, b_length, b_signed);
				result = arithmetic(a, a_length, a_signed, "<<", two_comp, b_length, b_signed);
			}
			else
			{
				if( b_signed && op == ">>>" )
				{
					int shift_by = v_strtoull(b, 2);

				}
				else
				{

				}
			}
		}
		else if( op == "<" )
		{

		}
		else if( op == ">" )
		{

		}
		else if( op == "<=" )
		{

		}
		else if( op == ">=" )
		{

		}
		else
		{

			/* Arithmetic operator */
			if(is_dont_care_string(adj_a) || is_dont_care_string(adj_b))
			{
				result = "x";
				result_length = 1;
				result_sign = false;
			}
			else
			{
				if( op == "+" )
				{
					
				}
				else if( op == "-" )
				{
					
				}
				else if( op == "*" )
				{

				}
				else if( op == "**" )
				{

				}
				else if( op == "/" )
				{
					if( "0" == arithmetic("|", adj_b, length, b_signed))
					{
						result = "x";
						result_length = 1;
						result_sign = false;
					}
					else
					{

					}

				}
				else if( op == "%" )
				{
					if( "0" == arithmetic("|", adj_b, length, b_signed))
					{
						result = "x";
						result_length = 1;
						result_sign = false;
					}
					else
					{

					}
				}
				else
				{
					error_message(-1,-1,"Operator %s is invalid for binary operation", op);
				}
			}
		}
	}
	return NULL;
}

/*******************************************************************
 *  convert to bitstring a number input and truncate to size
 * input
 *******************************************************************/
std::string to_bitstring(unsigned long long orig_number) 										{	return to_bitstring(std::to_string(orig_number), 10, 0, true);	}
std::string to_bitstring(unsigned long long orig_number, int num_bits)							{	return to_bitstring(std::to_string(orig_number), 10, num_bits, true);	}
std::string to_bitstring(std::string orig_number, int radix)									{	return to_bitstring(orig_number, radix, 0, false);	}
std::string to_bitstring(std::string orig_number, int radix, int num_bits)						{	return to_bitstring(orig_number, radix, 0, false);	}
std::string to_bitstring(std::string orig_number, int radix, bool is_signed)					{	return to_bitstring(orig_number, radix, 0, is_signed);	}
std::string to_bitstring(std::string orig_number, int radix, int num_bits, bool is_signed)
{
	std::string result = "";	
	oassert(is_string_of_radix(orig_number,radix));

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
				error_message(PARSE_ERROR, -1, -1, "Invalid base %d number: %s.\n", radix, orig_string);
				break;
			}
		}
	}

	return bitstring_adjust_len(result, num_bits, false);
}

std::string verilog_input_to_binary_string(std::string input_number)
{	
	//remove underscores
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());
	
	//prepare output and input
	auto loc = input_number.find("\'");
	if(loc == std::string::npos)
	{
		input_number.insert(0, "\'sd");
		loc = 0;
	}

	int bin_length = (loc == 0)? 0: std::strtoul(input_number.substr(0,loc).c_str(),NULL,10);

	bool signed = false;
	if(std::tolower(input_number[loc+1]) == 's')
	{
		input_number.erase (input_number.begin()+loc+1);
		signed = true;
	}

	switch(std::tolower(input_number[loc+1]))
	{
		case 'b':	return to_bitstring(input_number.substr(loc+2), 2, bin_length, signed);
		case 'o':	return to_bitstring(input_number.substr(loc+2), 8, bin_length, signed);
		case 'd':	return to_bitstring(input_number.substr(loc+2), 10, bin_length, signed);
		case 'h':	return to_bitstring(input_number.substr(loc+2), 16, bin_length, signed);
		default:	return to_bitstring(input_number.substr(loc+2), 0, bin_length, signed);
	}
}

std::string binary_string_to_verilog(orig_string, num_bits, sign)	
{
	return std::string((num_bits > 0) ? std::to_string(num_bits): "" + "\'b" + bitstring_adjust_len(orig_string, num_bits, false));
}