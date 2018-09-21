#include "rtl_int.h"
#include <cstdarg>
#include "stdio.h"
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>

// this sets the default bit width
#define DEFAULT_BIT_WIDTH 64

/***
 *     ___  __   __   __   __      __   ___  __   __   __  ___         __  
 *    |__  |__) |__) /  \ |__)    |__) |__  |__) /  \ |__)  |  | |\ | / _` 
 *    |___ |  \ |  \ \__/ |  \    |  \ |___ |    \__/ |  \  |  | | \| \__> 
 *                                                                         
 */
inline static std::vector<std::string> return_internal_representation(bool sign, std::size_t length, std::string bitstring)
{
	std::vector<std::string> to_return;
	to_return.push_back((sign)?"1":"0");
	to_return.push_back(std::to_string(length));
	to_return.push_back(bitstring);
	return to_return;
}

#define bad_value(test) _bad_value(static_cast<char>(std::tolower(test)), __func__, __LINE__); std::abort()
inline static char _bad_value(char test, const char *FUNCT, int LINE)	
{	
	std::clog << "INVALID BIT INPUT: (" << std::string(1,test) << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
	return 'x'; 
}

#define bad_ops(test) _bad_ops(test, __func__, __LINE__)
inline static std::vector<std::string> _bad_ops(std::string test, const char *FUNCT, int LINE)	
{	
	std::clog << "INVALID INPUT OPS: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;		
	return return_internal_representation(false,0,"xxxxxxxxxxxxxxx"); 
}

#define bad_bit_string(test) _bad_bit_string(test, __func__, __LINE__); std::abort()
inline static char _bad_bit_string(std::string test, const char *FUNCT, int LINE)	
{	
	std::clog << "INVALID INPUT BITSTRING: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;		
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

inline static bool cont(std::string input, std::string list, bool not_one_of)
{
	for(std::size_t i=0; i < input.length(); i ++)
		for(std::size_t j =0; j <= list.length(); j ++)
			if(j == list.length())
			{
				if(not_one_of) 	return false; 
				else 			break;
			}else if(std::tolower(input[i]) == std::tolower(list[i])){			
				if(not_one_of) 	break; 
				else 			return true;
			}

	return (not_one_of)? true: false;
}


inline static bool is_dont_care_string(std::string input)
{
	return cont(input, " xz",false);
}

inline static bool is_string_of_radix(std::string input, std::size_t radix)
{
	switch(radix)
	{
		case 2:		return cont(input, " xz01", true);
		case 8:		return cont(input, " xz01234567", true);
		case 10:	return cont(input, " 0123456789", true);
		case 16:	return cont(input, " xz0123456789abcdef", true);
		default:	return false;
	}
}

#define v_strtoul(num,radix) _v_strtoul(num,radix,__func__, __LINE__)
inline static std::size_t _v_strtoul(std::string orig_number, std::size_t radix, const char *FUNCT, int LINE)
{
	std::size_t result = 0;
	if (is_string_of_radix(orig_number, radix))
	{
		if (!is_dont_care_string(orig_number))
		{
			if( orig_number.length() < (sizeof(long)*8) )
			{
				for(std::size_t i=MSB; i < orig_number.length(); i++) {
					std::size_t bit = static_cast<std::size_t>(orig_number[i]);
					if(bit >= '0' && bit <= '9' )		result = (result * radix) +(bit - static_cast<std::size_t>('0'));
					else if(bit >= 'a' && bit <= 'f')	result = (result * radix) +(bit - static_cast<std::size_t>('a') + 10);
					else if(bit >= 'A' && bit <= 'F')	result = (result * radix) +(bit - static_cast<std::size_t>('A') + 10);
				}
			}
			else
			{
				std::cout << "Invalid Number. Too large to be converted. number: " << orig_number << " upper limit: " << std::to_string((sizeof(long)*8)) << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
				std::abort();
			}
		}
		else
		{
			std::cout << "Invalid Number contains dont care values. number: " << orig_number << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
			std::abort();
		}
	}	
	else
	{
		std::cout << "Invalid base " << std::to_string(radix) << " number: " << orig_number << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
	return result;
}

#define get_len(internal_bit_struct) _get_len(internal_bit_struct, __func__, __LINE__)
inline static std::size_t _get_len(std::vector<std::string> internal_bit_struct, const char *FUNCT, int LINE)
{
	std::size_t defined_size = _v_strtoul(internal_bit_struct[1],10, FUNCT, LINE);
	std::size_t current_bit_width = internal_bit_struct[2].length();

	if(defined_size == 0)
		return current_bit_width;
	else
		return defined_size;
}

#define get_bit(int_bits, location) _get_bit(int_bits, location, __func__, __LINE__)
inline static char _get_bit(std::vector<std::string> internal_bit_struct, std::size_t location, const char *FUNCT, int LINE)
{
	if(location >= get_len(internal_bit_struct))
	{
		std::clog << "INVALID INPUT BITSTRING INDEX: (" << std::to_string(location) << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
		std::abort();	
	}
	return static_cast<char>(internal_bit_struct[2][location]);
}

#define is_signed(bits)	(bits[0] == "0")? 0 : 1
inline static std::vector<std::string> set_sign(std::vector<std::string> internal_binaries, bool new_sign)
{
	return return_internal_representation(new_sign && is_signed(internal_binaries), v_strtoul(internal_binaries[1],10), internal_binaries[2]);
}

inline static std::vector<std::string> resize(std::vector<std::string> internal_bit_struct, std::size_t len)
{
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
inline static std::vector<std::string> readjust_size(std::vector<std::string> internal_bit_struct)
{
	return resize(internal_bit_struct, get_len(internal_bit_struct));
}

static std::string to_bitstring(std::string orig_string, std::size_t radix)
{
	std::string result = "";	
	if(!is_string_of_radix(orig_string,radix))
		bad_bit_string(orig_string);

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

//resize bitstring to length


inline static std::vector<std::string> standardize_input(std::string input_number)
{	
	//remove underscores
	std::cout << "pre-processing input: " << input_number << std::endl;
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());
	std::vector<std::string> to_return = V_BAD;

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
inline static std::string v_bin_string(std::vector<std::string> internal_binary_number)
{
	//final resize
	readjust_size(internal_binary_number);

	std::string output = "b\'";

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

inline static int internal_quick_eval(std::vector<std::string> a,std::vector<std::string> b)
{
	// -1 less, 0 equal, 1 greater
	short condition_code = -1; 

	std::size_t std_length = size_max(get_len(a), get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	for(std::size_t i=std_length-1;  i < std_length && condition_code == 0; i--)
	{
		condition_code = 	(get_bit(a, i) == get_bit(b, i))	?	0	:
							(get_bit(a, i) == '0')				?	-1	:
																	1	;
	}
	return condition_code;
}

inline static std::vector<std::string> V_REDUX(std::vector<std::string> a, const char lut[4][4])
{
	std::size_t end = get_len(a)-1;
	char result = get_bit(a, end);
	for(std::size_t i=end-1;  i < end; i--)
		result = v_binary_op(get_bit(a, i), result, lut);

	return return_internal_representation(false, 1, std::string(1,result));
}

inline static std::vector<std::string> V_REDUX(std::vector<std::string> a, std::vector<std::string> b, const char lut[4][4])
{
	std::string result = "";
	std::size_t std_length = size_max(get_len(a), get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	for(std::size_t i=std_length-1;  i < std_length; i--)
		PUSH_MSB(result, v_binary_op(get_bit(a, i), get_bit(b, i),lut));

	return return_internal_representation(false, std_length, result);
}

inline static std::vector<std::string> V_INCREMENT(std::vector<std::string> a, const char lut_adder[4][4], const char lut_carry[4][4])
{
	std::string result = "";
	char tmp_carry  = '0';
	std::size_t std_length = get_len(a);
	for(std::size_t i=std_length-1; i < std_length; i--)
	{
		PUSH_MSB(result, v_binary_op(get_bit(a, i), tmp_carry, lut_adder));
		tmp_carry = v_binary_op(get_bit(a, i), tmp_carry, lut_carry);
	}
	return return_internal_representation(is_signed(a), get_len(a), result);
}

/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */

inline static std::vector<std::string> V_NEG(std::vector<std::string> a)
{
	std::string result = "";
	std::size_t std_length = get_len(a);
	for(std::size_t i=std_length-1;  i < std_length; i--)
		PUSH_MSB(result, v_not(get_bit(a, i)));
	
	return return_internal_representation(false, 1, result);
}

inline static std::vector<std::string> V_PLUS_PLUS(std::vector<std::string> a)
{
	return V_INCREMENT(a, l_xor, l_and);
}

inline static std::vector<std::string>  V_MINUS_MINUS(std::vector<std::string> a)
{
	return V_INCREMENT(a, l_xnor, l_or);
}

inline static std::vector<std::string> V_ADD(std::vector<std::string> a)
{
	return a;
}

inline static std::vector<std::string> V_MINUS(std::vector<std::string> a)
{
	return V_PLUS_PLUS(V_NEG(a));
}

inline static std::vector<std::string> V_REDUCTION_AND(std::vector<std::string> a)
{
	return V_REDUX(a, l_and);
}

inline static std::vector<std::string> V_REDUCTION_OR(std::vector<std::string> a)
{
	return V_REDUX(a, l_or);	
}

inline static std::vector<std::string> V_REDUCTION_XOR(std::vector<std::string> a)
{
	return V_REDUX(a, l_xor);
}

inline static std::vector<std::string> V_REDUCTION_NAND(std::vector<std::string> a)
{
	return V_REDUX(a, l_nand);
}

inline static std::vector<std::string> V_REDUCTION_NOR(std::vector<std::string> a)
{
	return V_REDUX(a, l_nor);
}

inline static std::vector<std::string> V_REDUCTION_XNOR(std::vector<std::string> a)
{
	return V_REDUX(a, l_xnor);
}

inline static std::vector<std::string> V_LOGICAL_NOT(std::vector<std::string> a)
{
	return V_REDUCTION_NOR(a);
}

/***
 *     __               __          __   __   ___  __       ___    __       
 *    |__) | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    |__) | | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                          
 */

inline static std::vector<std::string> V_REDUCTION_AND(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUX(a,b,l_and);
}

inline static std::vector<std::string> V_REDUCTION_OR(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUX(a,b,l_or);
}

inline static std::vector<std::string> V_REDUCTION_XOR(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUX(a,b,l_xor);
}

inline static std::vector<std::string> V_REDUCTION_NAND(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUX(a,b,l_nand);
}

inline static std::vector<std::string> V_REDUCTION_NOR(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUX(a,b,l_nor);
}

inline static std::vector<std::string> V_REDUCTION_XNOR(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUX(a,b,l_xnor);
}

inline static std::vector<std::string> V_CASE_EQUAL(std::vector<std::string> a,std::vector<std::string> b)
{
	return return_internal_representation(false, 1, (internal_quick_eval(a,b) == 0)? "1": "0");
}
inline static std::vector<std::string> V_CASE_NOT_EQUAL(std::vector<std::string> a,std::vector<std::string> b)
{
	return return_internal_representation(false, 1, (internal_quick_eval(a,b) != 0)? "1": "0");
}

inline static std::vector<std::string> V_SIGNED_SHIFT_LEFT(std::vector<std::string> a, std::size_t b)
{
	get_bitstring(a).append(std::string(b,'0'));
	return readjust_size(a); 
}

inline static std::vector<std::string> V_SIGNED_SHIFT_RIGHT(std::vector<std::string> a, std::size_t b)
{
	get_bitstring(a) = get_bitstring(a).substr(0,get_len(a)-b);
	return readjust_size(a); 
}

inline static std::vector<std::string> V_SHIFT_LEFT(std::vector<std::string> a, std::size_t b)
{
	return V_SIGNED_SHIFT_LEFT(set_sign(a, false),b);  
}

inline static std::vector<std::string> V_SHIFT_RIGHT(std::vector<std::string> a, std::size_t b)
{
	return V_SIGNED_SHIFT_RIGHT(set_sign(a, false),b); 
}

inline static std::vector<std::string> V_LOGICAL_AND(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUCTION_AND(V_REDUCTION_OR(a),V_REDUCTION_OR(b));
}

inline static std::vector<std::string> V_LOGICAL_OR(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_REDUCTION_OR(V_REDUCTION_OR(a),V_REDUCTION_OR(b));
}

inline static std::vector<std::string> V_LT(std::vector<std::string> a,std::vector<std::string> b)
{
	return	return_internal_representation(false, 1, (internal_quick_eval(a,b) == -1) 	? "1":"0");
}

inline static std::vector<std::string> V_GT(std::vector<std::string> a,std::vector<std::string> b)
{
	return	return_internal_representation(false, 1, (internal_quick_eval(a,b) == 1) 	? "1":"0");
}

inline static std::vector<std::string> V_LE(std::vector<std::string> a,std::vector<std::string> b)
{
	return	return_internal_representation(false, 1, (internal_quick_eval(a,b) != 1) 	? "1":"0");
}

inline static std::vector<std::string> V_GE(std::vector<std::string> a,std::vector<std::string> b)
{
	return	return_internal_representation(false, 1, (internal_quick_eval(a,b) != -1) 	? "1":"0");
}

inline static std::vector<std::string> V_EQUAL(std::vector<std::string> a,std::vector<std::string> b)
{
	return	return_internal_representation(false, 1, (internal_quick_eval(a,b) == 0) 	? "1":"0");
}

inline static std::vector<std::string> V_NOT_EQUAL(std::vector<std::string> a,std::vector<std::string> b)
{
	return	return_internal_representation(false, 1, (internal_quick_eval(a,b) != 0) 	? "1":"0");
}

inline static std::vector<std::string> V_ADD(std::vector<std::string> a,std::vector<std::string> b)
{
	char previous_carry = '0';
	std::string result = "";
	std::size_t std_length = size_max(get_len(a), get_len(b));
	a = resize(a, std_length);
	b = resize(b, std_length);

	for(std::size_t i=std_length-1; i < std_length; i--)
	{
		PUSH_MSB(result,add_func(get_bit(a, i), get_bit(b, i), previous_carry));
		PUSH_MSB(result,carry_func(get_bit(a, i), get_bit(b, i), previous_carry));
	}
	PUSH_MSB(result,previous_carry);

	return	return_internal_representation(is_signed(a) && is_signed(b), std_length+1, result);
}

inline static std::vector<std::string> V_MINUS(std::vector<std::string> a,std::vector<std::string> b)
{
	return V_ADD(a, V_MINUS(b));
}

inline static std::vector<std::string> V_MULTIPLY(std::vector<std::string> a,std::vector<std::string> b)
{
	std::vector<std::string> result = return_internal_representation(is_signed(a) && is_signed(b),0,"0");
	a = readjust_size(a);
	b = readjust_size(b);
	std::size_t std_length = get_len(a);
	for(std::size_t i=std_length-1; i < std_length; i--)
		if(get_bit(a, i) == '1')
			result = V_ADD(result,V_SHIFT_LEFT(b, std_length-1-i));

	// TODO what size do we go to ??
	return result;
}

inline static std::vector<std::string> V_POWER(std::vector<std::string> a,std::vector<std::string> b)
{
	std::vector<std::string> result = return_internal_representation(is_signed(a) && is_signed(b),0,"1");
	while(internal_quick_eval(b,V_ZERO) > 0)
	{
		b = V_MINUS_MINUS(b);
		result = V_MULTIPLY( result, a);
	}
	// TODO negative power !!!( is it suported ?)
	return result;
}
inline static std::vector<std::string> V_DIV(std::vector<std::string> a,std::vector<std::string> b)
{
	std::vector<std::string> result = return_internal_representation(is_signed(a) && is_signed(b),0,"0");
	//TODO signed division!
	while(internal_quick_eval(b, a) < 0)
	{
		std::vector<std::string> count = return_internal_representation(is_signed(a) && is_signed(b),0,"1");
		std::vector<std::string> sub_with = b;
		for( std::vector<std::string> tmp = sub_with; internal_quick_eval(tmp, a) < 0; tmp = V_SHIFT_LEFT(tmp, 1))
		{
			count = V_SHIFT_LEFT(count, 1);
			sub_with = tmp;
		}
		a = V_MINUS(a, sub_with);
		result = V_ADD(result, count);
	}
	return result;
}

inline static std::vector<std::string> V_MOD(std::vector<std::string> a,std::vector<std::string> b)
{
	//TODO signed division!
	while(internal_quick_eval(b, a) < 0)
	{
		std::vector<std::string> sub_with = b;
		for( std::vector<std::string> tmp = sub_with; internal_quick_eval(tmp, a) < 0; tmp = V_SHIFT_LEFT(tmp, 1))
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
	The  evaluation  of  a  conditional  operator  shall  begin  with  a  logical  equality  comparison
	of expression1 with zero, termed the condition. If the condition evaluates to false (0), then expression3 shall be
	evaluated  and  used  as  the  result  of  the  conditional  expression.  If  the  condition  evaluates  to  true  (1),  then
	expression2 is evaluated and used as the result. If the condition evaluates to an ambiguous value (x or z),
	then  both  expression2  and  expression3  shall  be  evaluated;  and  their  results  shall  be  combined,  bit  by  bit,
	using Table 5-21 to  calculate  the  final  result  unless  expression2  or  expression3  is  real,  in  which  case  the
	result  shall  be  0.  If  the  lengths  of  expression2  and  expression3  are  different,  the  shorter  operand  shall  be
	lengthened to match the longer and zero-filled from the left (the high-order end)
*/
static std::vector<std::string> V_TERNARY(std::vector<std::string> a, std::vector<std::string> b, std::vector<std::string> c)
{
	/*	if a evaluates properly	*/
	std::vector<std::string> eval = V_REDUCTION_OR(a);
	if(internal_quick_eval(eval, V_ONE) == 0)		return b;
	else if(internal_quick_eval(eval, V_ZERO) == 0)	return c; 

	std::string result = "";
	std::size_t std_len = size_max(get_len(b), get_len(c));
	b = resize(b,std_len);
	c = resize(c,std_len);

	for(std::size_t i=std_len-1; i>=MSB; i--)
		PUSH_MSB(result,v_ternary(get_bit(b, i), get_bit(c, i)));

	return return_internal_representation(false,std_len,result);
}


/***
 *     __   __       ___  __   __           ___       __       
 *    /  ` /  \ |\ |  |  |__) /  \ |       |__  |    /  \ |  | 
 *    \__, \__/ | \|  |  |  \ \__/ |___    |    |___ \__/ |/\| 
 *                                                             
 */
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
							(op == "<<")	?		(is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
													(internal_quick_eval(b, V_ZERO)==0)			?	a:
													(internal_quick_eval(b, V_ZERO)==-1)		?	V_SHIFT_RIGHT(a,v_strtoul(get_bitstring(V_MINUS(b)),2)):
																									V_SHIFT_LEFT(a,v_strtoul(get_bitstring(b),2)):
							(op == "<<<")	?		(is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
													(internal_quick_eval(b, V_ZERO)==0)			?	a:
													(internal_quick_eval(b, V_ZERO)==-1)		?	V_SIGNED_SHIFT_RIGHT(a,v_strtoul(get_bitstring(V_MINUS(b)),2)):
																									V_SIGNED_SHIFT_LEFT(a,v_strtoul(get_bitstring(b),2)):
							(op == ">>")	?		(is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
													(internal_quick_eval(b, V_ZERO)==0)			?	a:
													(internal_quick_eval(b, V_ZERO)==-1)		?	V_SHIFT_LEFT(a,v_strtoul(get_bitstring(V_MINUS(b)),2)):
																									V_SHIFT_RIGHT(a,v_strtoul(get_bitstring(b),2)):
							(op == ">>>")	?		(is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
													(internal_quick_eval(b, V_ZERO)==0)			?	a:
													(internal_quick_eval(b, V_ZERO)==-1)		?	V_SIGNED_SHIFT_LEFT(a,v_strtoul(get_bitstring(V_MINUS(b)),2)):
																									V_SIGNED_SHIFT_RIGHT(a,v_strtoul(get_bitstring(b),2)):
							/* Logical Operators */
							(op == "&&")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_LOGICAL_AND(a,b):
							(op == "||")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_LOGICAL_OR(a,b):
							(op == "<")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_LT(a,b):																																													
							(op == ">")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_GT(a,b):
							(op == "<=")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_LE(a,b):
							(op == ">=")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_GE(a,b):
							(op == "==")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_EQUAL(a,b):
							(op == "!=")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_NOT_EQUAL(a,b):
							/* arithmetic Operators */																
							(op == "+")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_ADD(a,b):
							(op == "-")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_MINUS(a,b):
							(op == "*")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_MULTIPLY(a,b):
							(op == "**")	?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))) 	? 	V_UNK:
																									V_POWER(a,b):
							/* cannot div by 0 */
							(op == "/")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))
													|| get_bitstring(V_LE(b, V_ZERO)) == "1")	?	V_UNK:
																									V_DIV(a,b):
							(op == "%")		?		(is_dont_care_string(get_bitstring(a))
													|| is_dont_care_string(get_bitstring(b))
													|| get_bitstring(V_LE(b, V_ZERO)) == "1")	?	V_UNK:
																									V_MOD(a,b):																																																																																																																																																																																																															
													bad_ops(op));
}

std::string arithmetic(std::string a_in, std::string op1 ,std::string b_in, std::string op2, std::string c_in)
{
	std::vector<std::string> a = standardize_input(a_in);
	std::vector<std::string> b = standardize_input(b_in);
	std::vector<std::string> c = standardize_input(c_in);
	
	 /* return Process Operator via ternary */
	return	v_bin_string(	(op1 != "?")	?	bad_ops(op1):
							(op2 == ":")	?	bad_ops(op2):
												V_TERNARY(a,b,c));
}

int main()
{
	//read input file space separated operation 1 / line
	while(1)
	{
		char numb[3][512];
		scanf("%s %s %s", numb[0], numb[1], numb[2]);
		std::cout << arithmetic(numb[0],numb[1],numb[2]);
	}

	if(is_string_of_radix(std::string("0"),10) == false)
		std::cout << "BROKEN";
	else	
		std::cout << "WORKING";

	return 0;
}
