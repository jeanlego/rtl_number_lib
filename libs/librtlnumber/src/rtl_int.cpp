#include <cstdarg>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>

#include "rtl_int.h"

/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *           Dr. Kenneth B. Kent (ken@unb.ca)
 *           for the Reconfigurable Computing Research Lab at the
 *           Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

// this sets the default bit width
#define DEFAULT_BIT_WIDTH 32
#define BRK brk_pt();
static void brk_pt(){
	return;
}

/***
 *     ___  __   __   __   __      __   ___  __   __   __  ___         __  
 *    |__  |__) |__) /  \ |__)    |__) |__  |__) /  \ |__)  |  | |\ | / _` 
 *    |___ |  \ |  \ \__/ |  \    |  \ |___ |    \__/ |  \  |  | | \| \__> 
 *                                                                         
 */
inline static RTL_INT return_internal_representation(bool sign, std::size_t length, std::string bitstring)
{
	RTL_INT to_return;
	to_return.push_back((sign)?"1":"0");
	to_return.push_back(std::to_string(length));
	to_return.push_back(bitstring);
	return to_return;
}

#define bad_value(test) _bad_value(static_cast<char>(std::tolower(test)), __func__, __LINE__); std::abort()
inline static char _bad_value(char test, const char *FUNCT, int LINE)	
{	
	std::cout << "INVALID BIT INPUT: (" << std::string(1,test) << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
	return 'x'; 
}

/***
 *     __    ___     __  ___  __          __                     __        ___  __   __  
 *    |__) |  |     /__`  |  |__) | |\ | / _`    |__|  /\  |\ | |  \ |    |__  |__) /__` 
 *    |__) |  |     .__/  |  |  \ | | \| \__>    |  | /~~\ | \| |__/ |___ |___ |  \ .__/ 
 *                                                                                       
 */

#define MSB 0
#define get_bitstring(bits)			bits[2]
#define PUSH_MS_BITSTRING(str,chr)	str.insert(MSB,chr)
#define PUSH_MSB(str,chr)			str.insert(MSB,std::string(1,chr))

#define size_max(a,b) ((a) > (b)) ? (a): (b)
#define V_ZERO return_internal_representation(false,1,"0")
#define V_ONE return_internal_representation(false,1,"1")
#define V_UNK return_internal_representation(false,1,"x")
#define V_BAD return_internal_representation(true,0,"xx0x00x0x0x0x0x0x0x0x0x")

#define is_dont_care_string(input) (input.find("xXzZ") != std::string::npos)

#define assert_string_of_radix(input, radix) _assert_string_of_radix(input, radix, __func__,__LINE__)
inline static void _assert_string_of_radix(std::string input, std::size_t radix, const char *FUNCT, int LINE)
{
	BRK
	bool pass = false;
	switch(radix)
	{
		case 2:		pass = (input.find_first_not_of("xXzZ01") == std::string::npos);						break;
		case 8:		pass = (input.find_first_not_of("xXzZ01234567") == std::string::npos);					break;
		case 10:	pass = (input.find_first_not_of("0123456789") == std::string::npos);					break;
		case 16:	pass = (input.find_first_not_of("xZzZ0123456789aAbBcCdDeEfF") == std::string::npos);	break;
		default:	pass = false;	break;	
	}
	if(!pass)
	{
		std::cout << "Invalid bitstring of radix input " << std::to_string(radix) << " number: " << input << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

#define v_strtoul(num,radix) _v_strtoul(num,radix,__func__, __LINE__)
inline static std::size_t _v_strtoul(std::string orig_number, std::size_t radix, const char *FUNCT, int LINE)
{
	BRK
	assert_string_of_radix(orig_number, radix);
	if (is_dont_care_string(orig_number))
	{
		std::cout << "Invalid Number contains dont care values. number: " << orig_number << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
	else if( orig_number.length() >= (sizeof(long)*8) )
	{
		std::cout << "Invalid Number. Too large to be converted. number: " << orig_number << " upper limit: " << std::to_string((sizeof(long)*8)) << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
	
	return std::strtoul(orig_number.c_str(),NULL,static_cast<int>(radix));
}

#define get_len(internal_bit_struct) _get_len(internal_bit_struct, __func__, __LINE__)
inline static std::size_t _get_len(RTL_INT internal_bit_struct, const char *FUNCT, int LINE)
{
	BRK
	std::size_t defined_size = _v_strtoul(internal_bit_struct[1],10, FUNCT, LINE);
	std::size_t current_bit_width = internal_bit_struct[2].length();

	if(defined_size == 0)
		return current_bit_width;
	else
		return defined_size;
}

#define get_bit(int_bits, location) _get_bit(int_bits, location, __func__, __LINE__)
inline static char _get_bit(RTL_INT internal_bit_struct, std::size_t location, const char *FUNCT, int LINE)
{
	BRK
	if(location >= get_len(internal_bit_struct))
	{
		std::clog << "INVALID INPUT BITSTRING INDEX: (" << std::to_string(location) << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
		std::abort();	
	}
	return static_cast<char>(internal_bit_struct[2][location]);
}

#define is_signed(bits)	(bits[0] == "0")? 0 : 1
inline static RTL_INT set_sign(RTL_INT internal_binaries, bool new_sign)
{
	BRK
	return return_internal_representation(new_sign && is_signed(internal_binaries), v_strtoul(internal_binaries[1],10), internal_binaries[2]);
}

inline static RTL_INT resize(RTL_INT internal_bit_struct, std::size_t len)
{
	BRK
	//expand to length
	if(len > get_len(internal_bit_struct))
	{
		std::string padding_bits(len-get_len(internal_bit_struct), (!is_signed(internal_bit_struct) || get_bit(internal_bit_struct,MSB) =='x')? '0': get_bit(internal_bit_struct,MSB));
		internal_bit_struct[2].insert(MSB,padding_bits);
	}

	//truncate to length
	else if (len < get_len(internal_bit_struct))
		internal_bit_struct[2] = internal_bit_struct[2].substr(0,get_len(internal_bit_struct)-len);

	return internal_bit_struct;
}

//resize bitstring to its own length
inline static RTL_INT readjust_size(RTL_INT internal_bit_struct)
{
	BRK
	return resize(internal_bit_struct, get_len(internal_bit_struct));
}

/**********************
 * convert from and to internal representation bitstring
 */
inline static std::string to_bitstring(std::string orig_string, std::size_t radix)
{
	BRK
	std::string result = "";	
	assert_string_of_radix(orig_string,radix);
	while(orig_string != "")
	{
		switch(radix)
		{
			case 10:
			{
				std::string new_number = "";
				short prev = '0';
				short flag = 0;
				for(size_t i=orig_string.length()-1; i<orig_string.length(); i--)
				{
					short current_digit = (orig_string[i]-'0');
					short new_pair = (prev%2)*10 + current_digit;
					std::string div = std::to_string(new_pair/2);
					std::string rem = std::to_string(new_pair%2);

					prev = current_digit;
					if(div != "0" && !flag)
						flag = 1;

					if(flag)
						new_number.append(div);
					
					if(i == MSB)
					{
						PUSH_MS_BITSTRING(result, rem);
						orig_string = new_number;
					}
				}
				break;
			}
			case 2:		//fallthrough
			case 8:		//fallthrough
			case 16:
			{
				int lsb = std::tolower(orig_string.back()); 
				orig_string.pop_back();
				switch(radix)
				{
					case 2:
					{
						switch(lsb)
						{
							case '0': PUSH_MS_BITSTRING(result, "0");	break;
							case '1': PUSH_MS_BITSTRING(result, "1");	break;
							case 'x': PUSH_MS_BITSTRING(result, "x");	break;
							case 'z': PUSH_MS_BITSTRING(result, "z");	break;
							default:  bad_value(lsb);
						}
						break;
					}
					case 8:
					{
						switch(lsb)
						{
							case '0': PUSH_MS_BITSTRING(result, "000");	break;
							case '1': PUSH_MS_BITSTRING(result, "001");	break;
							case '2': PUSH_MS_BITSTRING(result, "010");	break;
							case '3': PUSH_MS_BITSTRING(result, "011");	break;
							case '4': PUSH_MS_BITSTRING(result, "100");	break;
							case '5': PUSH_MS_BITSTRING(result, "101");	break;
							case '6': PUSH_MS_BITSTRING(result, "110");	break;
							case '7': PUSH_MS_BITSTRING(result, "111");	break;
							case 'x': PUSH_MS_BITSTRING(result, "xxx");	break;
							case 'z': PUSH_MS_BITSTRING(result, "zzz");	break;
							default:  bad_value(lsb);
						}
						break;
					}
					case 16:
					{
						switch(lsb)
						{
							case '0': PUSH_MS_BITSTRING(result, "0000");	break;
							case '1': PUSH_MS_BITSTRING(result, "0001");	break;
							case '2': PUSH_MS_BITSTRING(result, "0010");	break;
							case '3': PUSH_MS_BITSTRING(result, "0011");	break;
							case '4': PUSH_MS_BITSTRING(result, "0100");	break;
							case '5': PUSH_MS_BITSTRING(result, "0101");	break;
							case '6': PUSH_MS_BITSTRING(result, "0110");	break;
							case '7': PUSH_MS_BITSTRING(result, "0111");	break;
							case '8': PUSH_MS_BITSTRING(result, "1000");	break;
							case '9': PUSH_MS_BITSTRING(result, "1001");	break;
							case 'a': PUSH_MS_BITSTRING(result, "1010");	break;
							case 'b': PUSH_MS_BITSTRING(result, "1011");	break;
							case 'c': PUSH_MS_BITSTRING(result, "1100");	break;
							case 'd': PUSH_MS_BITSTRING(result, "1101");	break;
							case 'e': PUSH_MS_BITSTRING(result, "1110");	break;
							case 'f': PUSH_MS_BITSTRING(result, "1111");	break;
							case 'x': PUSH_MS_BITSTRING(result, "xxxx");	break;
							case 'z': PUSH_MS_BITSTRING(result, "zzzz");	break;
							default:  bad_value(lsb);
						}
						break;
					}
					default:
					{
						std::cout << "Invalid base " << std::to_string(radix) << " number: " << orig_string << ".\n";
						std::abort();
					}
				}
				break;
			}
			default:
			{
				std::cout << "Invalid base " << std::to_string(radix) << " number: " << orig_string << ".\n";
				std::abort();
			}
		}
	}
	return result;
}

RTL_INT standardize_input(std::string input_number)
{	
	BRK
	//remove underscores
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());
	RTL_INT to_return = V_BAD;

	std::size_t loc = input_number.find("\'");
	if(loc == std::string::npos)
	{
		input_number.insert(0, "\'sd");
		loc = 0;
	}

	if(loc == 0)
	{
		std::string bit_len = std::to_string(DEFAULT_BIT_WIDTH);
		input_number.insert(0, bit_len);
		loc = bit_len.length();
	}

	to_return = resize(to_return, v_strtoul(input_number.substr(0,loc),10));

	if(std::tolower(input_number[loc+1]) == 's')
	{
		input_number.erase (input_number.begin()+static_cast<long>(loc)+1);
		to_return = set_sign(to_return,true);
	}
	else
	{
		to_return = set_sign(to_return,false);
	}
	
	switch(std::tolower(input_number[loc+1]))
	{
		case 'b':	to_return[2] = to_bitstring(input_number.substr(loc+2), 2); break;
		case 'o':	to_return[2] = to_bitstring(input_number.substr(loc+2), 8); break;
		case 'd':	to_return[2] = to_bitstring(input_number.substr(loc+2), 10); break;
		case 'h':	to_return[2] = to_bitstring(input_number.substr(loc+2), 16); break;
		default:
		{
			std::cout << "Invalid base " << std::string(1,input_number[loc+1]) << " number: " << input_number << ".\n";
			std::abort();
		}
	}

	return readjust_size(to_return);
}

// convert internal format to verilog
std::string v_bin_string(RTL_INT internal_binary_number)
{
	BRK
	//final resize
	readjust_size(internal_binary_number);

	std::string output = "";

	if(internal_binary_number[1] != "0")
		output += internal_binary_number[1];

	output += "\'";

	if(is_signed(internal_binary_number)) 
		output += "s";

	return output + "b" + internal_binary_number[2];
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
#define REF_0 0
#define REF_1 1
#define REF_x 2
#define REF_z 3

#define v_unary_op(a,op) _v_unary_op(a, op, __func__, __LINE__)
inline static char _v_unary_op(const char a, const char lut[4], const char *FUNCT, int LINE) 
{
	return 	((a) == '0')				?	lut[REF_0] :
			((a) == '1')				?	lut[REF_1] :
			(std::tolower(a) == 'x')	?	lut[REF_x] : 
			(std::tolower(a) == 'z')	?	lut[REF_z] :
											_bad_value(a, FUNCT,LINE)
			;
}

#define v_binary_op(a,b,op)	_v_binary_op(a, b, op, __func__, __LINE__)
inline static char _v_binary_op(const char a, const char b, const char lut[4][4], const char *FUNCT, int LINE) 
{
	return 	((a) == '0')				? 	_v_unary_op(b, lut[REF_0], FUNCT, LINE)	:
			((a) == '1')				? 	_v_unary_op(b, lut[REF_1], FUNCT, LINE) :
			(std::tolower(a) == 'x')	?	_v_unary_op(b, lut[REF_x], FUNCT, LINE)	: 
			(std::tolower(a) == 'z')	?	_v_unary_op(b, lut[REF_z], FUNCT, LINE) :
											_bad_value(a, FUNCT, LINE)
			;
}

static const char l_buf[4] = {
	/*	 0   1   x   z  <- a*/
		'0','1','x','x'
};

static const char l_not[4] = {
	/*   0   1   x   z 	<- a */
		'1','0','x','x'
};

static const char l_and[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{'0','0','0','0'},	
	/* 1 */	{'0','1','x','x'},	
	/* x */	{'0','x','x','x'},	
	/* z */	{'0','x','x','x'}
};

static const char l_or[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{'0','1','x','x'},	
	/* 1 */	{'1','1','1','1'},	
	/* x */	{'x','1','x','x'},	
	/* z */	{'x','1','x','x'}
};

static const char l_xor[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{'0','1','x','x'},	
	/* 1 */	{'1','0','x','x'},	
	/* x */	{'x','x','x','x'},	
	/* z */	{'x','x','x','x'}
};

static const char l_nand[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{'1','1','1','1'},	
	/* 1 */	{'1','0','x','x'},	
	/* x */	{'1','x','x','x'},	
	/* z */	{'1','x','x','x'}
};

static const char l_nor[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{'1','0','x','x'},	
	/* 1 */	{'0','0','0','0'},	
	/* x */	{'x','0','x','x'},	
	/* z */	{'x','0','x','x'}
};

static const char l_xnor[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{'1','0','x','x'},	
	/* 1 */	{'0','1','x','x'},	
	/* x */	{'x','x','x','x'},	
	/* z */	{'x','x','x','x'}
};

static const char l_notif1[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'z','1','H','H'},
	/* 1 */	{'z','0','L','L'},
	/* x */	{'z','x','x','x'},
	/* z */	{'z','x','x','x'}
};

static const char l_notif0[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'1','z','H','H'},
	/* 1 */	{'0','z','L','L'},
	/* x */	{'x','z','x','x'},
	/* z */	{'x','z','x','x'}
};

static const char l_bufif1[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'z','0','H','H'},
	/* 1 */	{'z','1','L','L'},
	/* x */	{'z','x','x','x'},
	/* z */	{'z','x','x','x'}
};

static const char l_bufif0[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'0','z','H','H'},
	/* 1 */	{'1','z','L','L'},
	/* x */	{'x','z','x','x'},
	/* z */	{'x','z','x','x'}
};

static const char l_rpmos[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'0','z','L','L'},
	/* 1 */	{'1','z','H','H'},
	/* x */	{'x','z','x','x'},
	/* z */	{'z','z','z','z'}
};

static const char l_rnmos[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'z','0','L','L'},
	/* 1 */	{'z','1','H','H'},
	/* x */	{'z','x','x','x'},
	/* z */	{'z','z','z','z'}
};

static const char l_nmos[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'z','0','L','L'},
	/* 1 */	{'z','1','H','H'},
	/* x */	{'z','x','x','x'},
	/* z */	{'z','z','z','z'}
};

// see table 5-21 p:54 IEEE 1364-2005
static const char l_ternary[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'0','x','x','x'},
	/* 1 */	{'x','1','x','x'},
	/* x */	{'x','x','x','x'},
	/* z */	{'x','x','x','x'}
};

/***
 * these are extended defines to simplify our lives
 */
#define v_buf(a)				v_unary_op(a, 				l_buf)
#define v_not(a)				v_unary_op(a, 				l_not)
#define v_and(a,b)				v_binary_op(a,b, 			l_and)
#define v_or(a,b)				v_binary_op(a,b, 			l_or)
#define v_xor(a,b)				v_binary_op(a,b, 			l_xor)
#define v_nand(a,b)				v_binary_op(a,b, 			l_nand)
#define v_nor(a,b)				v_binary_op(a,b, 			l_nor)
#define v_xnor(a,b)				v_binary_op(a,b, 			l_xnor)
#define v_notif1(in,control)	v_binary_op(in,control, 	l_notif1)
#define v_notif0(in,control)	v_binary_op(in,control, 	l_notif0)
#define v_bufif1(in,control)	v_binary_op(in,control, 	l_bufif1)
#define v_bufif0(in,control)	v_binary_op(in,control, 	l_bufif0)
#define v_rpmos(in, control)	v_binary_op(in,control, 	l_rpmos)
#define v_pmos(in,control)		v_binary_op(in,control, 	l_pmos)
#define v_rnmos(in, control)	v_binary_op(in,control, 	l_rnmos)
#define v_nmos(in,control)		v_binary_op(in,control, 	l_nmos)
#define v_ternary(in,control)	v_binary_op(in,control,	l_ternary)

#define add_func(a, b, carry)	v_xor(v_xor(a,b),carry)
#define carry_func(a, b, carry)	v_or(v_and(a,b),v_and(v_xor(a,b),carry))


//TODO: signed numbers!

EVAL_RESULT return_int_eval(RTL_INT a,RTL_INT b)
{
	std::size_t std_length = std::max(get_len(a),get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	auto bit_a = get_bitstring(a).begin();
	auto bit_b = get_bitstring(b).begin();
	for (; bit_a != get_bitstring(a).end() && bit_b != get_bitstring(b).end(); ++bit_a, ++bit_b)
	{
		if		(*bit_a == '1' && *bit_b == '0')	return GREATHER_THAN;
		else if	(*bit_a == '0' && *bit_b == '1')	return LESS_THAN;
		else if	(*bit_a == 'x' || *bit_b == 'x')	return UNKNOWN;
	}
	return EQUAL;
}
EVAL_RESULT return_int_eval(long a,RTL_INT b)
{
	return return_int_eval(standardize_input(std::to_string(a)),b);
}
EVAL_RESULT return_int_eval(RTL_INT a,long b)
{
	return return_int_eval(a,standardize_input(std::to_string(b)));
}


//TODO: is it LSB -> MSB or MSB -> LSB
inline static RTL_INT V_REDUX(RTL_INT a, const char lut[4][4])
{
	std::size_t end = get_len(a)-1;
	char result = get_bit(a, end);
	auto bit_a = get_bitstring(a).crbegin();
    for (; bit_a != get_bitstring(a).crend(); ++bit_a)
		result = v_binary_op(*bit_a, result, lut);

	return return_internal_representation(false, 1, std::string(1,result));
}

inline static RTL_INT V_REDUX(RTL_INT a, RTL_INT b, const char lut[4][4])
{
	std::string result = "";
	std::size_t std_length = size_max(get_len(a), get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	auto bit_a = get_bitstring(a).crbegin();
	auto bit_b = get_bitstring(b).crbegin();
	for (; bit_a != get_bitstring(a).crend() && bit_b != get_bitstring(b).crend(); ++bit_a, ++bit_b)
		PUSH_MSB(result, v_binary_op(*bit_a, *bit_b,lut));

	return return_internal_representation(false, std_length, result);
}

inline static RTL_INT V_INCREMENT(RTL_INT a, const char lut_adder[4][4], const char lut_carry[4][4])
{
	std::string result = "";
	char tmp_carry  = '0';
	std::size_t std_length = get_len(a);
	
	auto bit_a = get_bitstring(a).crbegin();
    for (; bit_a != get_bitstring(a).crend(); ++bit_a) {
		PUSH_MSB(result, v_binary_op(*bit_a,tmp_carry, lut_adder));
		tmp_carry = v_binary_op(*bit_a, tmp_carry, lut_carry);
	}
	return return_internal_representation(is_signed(a), result.size(), result);
}

/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */

RTL_INT V_NEG(RTL_INT a)
{
	std::string result = "";
	auto bit_a = get_bitstring(a).crbegin();
    for (; bit_a != get_bitstring(a).crend(); ++bit_a)
		PUSH_MSB(result, v_not(*bit_a));
	
	return return_internal_representation(false, result.length(), result);
}

RTL_INT V_PLUS_PLUS(RTL_INT a)
{
	return V_INCREMENT(a, l_xor, l_and);
}

RTL_INT  V_MINUS_MINUS(RTL_INT a)
{
	return V_INCREMENT(a, l_xnor, l_or);
}

RTL_INT V_ADD(RTL_INT a)
{
	return a;
}

RTL_INT V_MINUS(RTL_INT a)
{
	return V_PLUS_PLUS(V_NEG(a));
}

RTL_INT V_REDUCTION_AND(RTL_INT a)
{
	return V_REDUX(a, l_and);
}

RTL_INT V_REDUCTION_OR(RTL_INT a)
{
	return V_REDUX(a, l_or);	
}

RTL_INT V_REDUCTION_XOR(RTL_INT a)
{
	return V_REDUX(a, l_xor);
}

RTL_INT V_REDUCTION_NAND(RTL_INT a)
{
	return V_REDUX(a, l_nand);
}

RTL_INT V_REDUCTION_NOR(RTL_INT a)
{
	return V_REDUX(a, l_nor);
}

RTL_INT V_REDUCTION_XNOR(RTL_INT a)
{
	return V_REDUX(a, l_xnor);
}

RTL_INT V_LOGICAL_NOT(RTL_INT a)
{
	return V_REDUCTION_NOR(a);
}

/***
 *     __               __          __   __   ___  __       ___    __       
 *    |__) | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    |__) | | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                          
 */

RTL_INT V_REDUCTION_AND(RTL_INT a,RTL_INT b)
{
	return V_REDUX(a,b,l_and);
}

RTL_INT V_REDUCTION_OR(RTL_INT a,RTL_INT b)
{
	return V_REDUX(a,b,l_or);
}

RTL_INT V_REDUCTION_XOR(RTL_INT a,RTL_INT b)
{
	return V_REDUX(a,b,l_xor);
}

RTL_INT V_REDUCTION_NAND(RTL_INT a,RTL_INT b)
{
	return V_REDUX(a,b,l_nand);
}

RTL_INT V_REDUCTION_NOR(RTL_INT a,RTL_INT b)
{
	return V_REDUX(a,b,l_nor);
}

RTL_INT V_REDUCTION_XNOR(RTL_INT a,RTL_INT b)
{
	return V_REDUX(a,b,l_xnor);
}

// TODO: fix me!!! this is bit by bit comparison!!!
RTL_INT V_CASE_EQUAL(RTL_INT a,RTL_INT b)
{
	std::size_t std_length = size_max(get_len(a), get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	auto bit_a = get_bitstring(a).crbegin();
	auto bit_b = get_bitstring(b).crbegin();
	for (; bit_a != get_bitstring(a).crend() && bit_b != get_bitstring(b).crend(); ++bit_a, ++bit_b)
		if(*bit_a != *bit_b)	return V_ZERO;
	
	return V_ONE;
}
RTL_INT V_CASE_NOT_EQUAL(RTL_INT a,RTL_INT b)
{
	return V_LOGICAL_NOT(V_CASE_EQUAL(a,b));
}

/**
 * Shift operations
 */
RTL_INT V_SIGNED_SHIFT_LEFT(RTL_INT a, RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(b)))	
		return V_UNK;

	EVAL_RESULT compare_result = return_int_eval(b, 0);
	if(compare_result == EQUAL)					return a;
	else if(compare_result == LESS_THAN)		return V_SIGNED_SHIFT_RIGHT(a,V_MINUS(b));
	else										return V_SIGNED_SHIFT_LEFT(a,v_strtoul(get_bitstring(b),2));

}
RTL_INT V_SIGNED_SHIFT_LEFT(RTL_INT a, std::size_t b)
{
	get_bitstring(a).append(std::string(b,'0'));
	return readjust_size(a); 
}

RTL_INT V_SIGNED_SHIFT_RIGHT(RTL_INT a, RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(b)))	
		return V_UNK;
		
	EVAL_RESULT compare_result = return_int_eval(b, 0);
	if(compare_result == EQUAL)					return a;
	else if(compare_result == LESS_THAN)		return V_SIGNED_SHIFT_LEFT(a,V_MINUS(b));
	else										return V_SIGNED_SHIFT_RIGHT(a,v_strtoul(get_bitstring(b),2));

}
RTL_INT V_SIGNED_SHIFT_RIGHT(RTL_INT a, std::size_t b)
{
	get_bitstring(a) = get_bitstring(a).substr(0,get_len(a)-b);
	return readjust_size(a); 
}

RTL_INT V_SHIFT_LEFT(RTL_INT a, RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(b)))	
		return V_UNK;
		
	EVAL_RESULT compare_result = return_int_eval(b, 0);
	if(compare_result == EQUAL)					return a;
	else if(compare_result == LESS_THAN)		return V_SHIFT_RIGHT(a,V_MINUS(b));
	else										return V_SHIFT_LEFT(a,v_strtoul(get_bitstring(b),2));

}
RTL_INT V_SHIFT_LEFT(RTL_INT a, std::size_t b)
{
	return V_SIGNED_SHIFT_LEFT(set_sign(a, false),b);  
}

RTL_INT V_SHIFT_RIGHT(RTL_INT a, RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(b)))	
		return V_UNK;
		
	EVAL_RESULT compare_result = return_int_eval(b, 0);
	if(compare_result == EQUAL)					return a;
	else if(compare_result == LESS_THAN)		return V_SHIFT_LEFT(a,V_MINUS(b));
	else										return V_SHIFT_RIGHT(a,v_strtoul(get_bitstring(b),2));

}
RTL_INT V_SHIFT_RIGHT(RTL_INT a, std::size_t b)
{
	return V_SIGNED_SHIFT_RIGHT(set_sign(a, false),b); 
}

/**
 * Logical Operations
 */
RTL_INT V_LOGICAL_AND(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return V_REDUCTION_AND(V_REDUCTION_OR(a),V_REDUCTION_OR(b));
}

RTL_INT V_LOGICAL_OR(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return V_REDUCTION_OR(V_REDUCTION_OR(a),V_REDUCTION_OR(b));
}

RTL_INT V_LT(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return	return_internal_representation(false, 1, (return_int_eval(a,b) == LESS_THAN) 	? "1":"0");
}

RTL_INT V_GT(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return	return_internal_representation(false, 1, (return_int_eval(a,b) == GREATHER_THAN) 	? "1":"0");
}

RTL_INT V_LE(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return	return_internal_representation(false, 1, (return_int_eval(a,b) != GREATHER_THAN) 	? "1":"0");
}

RTL_INT V_GE(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return	return_internal_representation(false, 1, (return_int_eval(a,b) != LESS_THAN) 	? "1":"0");
}

RTL_INT V_EQUAL(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return	return_internal_representation(false, 1, (return_int_eval(a,b) == EQUAL) 	? "1":"0");
}

RTL_INT V_NOT_EQUAL(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return	return_internal_representation(false, 1, (return_int_eval(a,b) != EQUAL) 	? "1":"0");
}

RTL_INT V_ADD(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	char previous_carry = '0';
	std::string result = "";
	std::size_t std_length = size_max(get_len(a), get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	auto bit_a = get_bitstring(a).crbegin();
	auto bit_b = get_bitstring(b).crbegin();
	for (; bit_a != get_bitstring(a).crend() && bit_b != get_bitstring(b).crend(); ++bit_a, ++bit_b)
	{
		PUSH_MSB(result,add_func(*bit_a, *bit_b, previous_carry));
		PUSH_MSB(result,carry_func(*bit_a, *bit_b, previous_carry));
	}
	PUSH_MSB(result,previous_carry);

	return	return_internal_representation(is_signed(a) && is_signed(b), std_length+1, result);
}

RTL_INT V_MINUS(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	return V_ADD(a, V_MINUS(b));
}

RTL_INT V_MULTIPLY(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	a = readjust_size(a);
	b = readjust_size(b);
	std::size_t std_length = std::max(get_len(b),get_len(a))*2;
	RTL_INT result = return_internal_representation(is_signed(a) && is_signed(b),std_length,"0");
	
	std::size_t current_shift =0;
	auto bit_a = get_bitstring(a).crbegin();
	for (; bit_a != get_bitstring(a).crend(); ++bit_a, ++current_shift)
		if(*bit_a == '1')
			result = V_ADD(result,V_SHIFT_LEFT(b, current_shift));

	// TODO what size do we go to ??
	return result;
}

RTL_INT V_POWER(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b)))
		return V_UNK;

	RTL_INT result = return_internal_representation(is_signed(a) && is_signed(b),0,"1");
	while(return_int_eval(b,0) == GREATHER_THAN)
	{
		b = V_MINUS_MINUS(b);
		result = V_MULTIPLY( result, a);
	}
	// TODO negative power !!!( is it suported ?)
	return result;
}
RTL_INT V_DIV(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b))
	|| return_int_eval(b,0) == EQUAL)
		return V_UNK;

	RTL_INT result = return_internal_representation(is_signed(a) && is_signed(b),0,"0");
	//TODO signed division!
	while(return_int_eval(b, a) == LESS_THAN)
	{
		RTL_INT count = return_internal_representation(is_signed(a) && is_signed(b),0,"1");
		RTL_INT sub_with = b;
		for( RTL_INT tmp = sub_with; return_int_eval(tmp, a) == LESS_THAN; tmp = V_SHIFT_LEFT(tmp, 1))
		{
			count = V_SHIFT_LEFT(count, 1);
			sub_with = tmp;
		}
		a = V_MINUS(a, sub_with);
		result = V_ADD(result, count);
	}
	return result;
}

RTL_INT V_MOD(RTL_INT a,RTL_INT b)
{
	if(is_dont_care_string(get_bitstring(a)) || is_dont_care_string(get_bitstring(b))
	|| return_int_eval(b, 0) == EQUAL)
		return V_UNK;

	//TODO signed division!
	while(return_int_eval(b, a)  == LESS_THAN)
	{
		RTL_INT sub_with = b;
		for( RTL_INT tmp = sub_with; return_int_eval(tmp, a) == LESS_THAN; tmp = V_SHIFT_LEFT(tmp, 1))
		{
			sub_with = tmp;
		}
		a = V_MINUS(a, sub_with);
	}
	return a;
}

/***
 *    ___  ___  __             __          __   __   ___  __       ___    __       
 *     |  |__  |__) |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *     |  |___ |  \ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                                 
*/
RTL_INT V_TERNARY(RTL_INT a, RTL_INT b, RTL_INT c)
{
	/*	if a evaluates properly	*/
	EVAL_RESULT eval = return_int_eval(V_LOGICAL_NOT(a),V_ZERO);
	if(eval == UNKNOWN)		return V_REDUX(b,c,l_ternary);
	else if(eval == EQUAL)	return b;
	else					return c;
}
