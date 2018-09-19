// #include <rtl_int.h>
#include <cstdarg>
#include "stdio.h"
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

#define bad_value(test) _bad_value(test, __FILE__, __LINE__)
inline static const char _bad_value(char test, char *FILE, int LINE)	
{	
	std::clog << "INVALID BIT INPUT: (" << test << ")@" << __FILE__<< "::" << __LINE__ << std::endl;	
	std::abort;		
	return 0; 
}

#define bad_ops(test) _bad_ops(test, __FILE__, __LINE__)
inline static std::string _bad_ops(std::string test, char *FILE, int LINE)	
{	
	std::clog << "INVALID INPUT OPS: (" << test << ")@" << __FILE__<< "::" << __LINE__ << std::endl;	
	std::abort;		
	return "'bDEADBEEF"; 
}

#define bad_bit_string(test) _bad_bit_string(test, __FILE__, __LINE__)
inline static const char _bad_bit_string(std::string test, char *FILE, int LINE)	
{	
	std::clog << "INVALID INPUT BITSTRING: (" << test << ")@" << __FILE__<< "::" << __LINE__ << std::endl;	
	std::abort;		
	return 0; 
}

/***
 *     __    ___     __  ___  __          __                     __        ___  __   __  
 *    |__) |  |     /__`  |  |__) | |\ | / _`    |__|  /\  |\ | |  \ |    |__  |__) /__` 
 *    |__) |  |     .__/  |  |  \ | | \| \__>    |  | /~~\ | \| |__/ |___ |___ |  \ .__/ 
 *                                                                                       
 */

#define is_signed(bits)	(bits[0] == "0")? 0 : 1
#define get_len(bits)	v_strtoull(bits[1],10)
#define get_bits(bits)	bits[2]

inline static std::string set_len(std::vector<std::string> internal_binaries, long new_len)
{
	if(get_len(internal_binaries) == 0)
		return "0";
	else if(get_len(internal_binaries) > 0)
		return internal_binaries[1];
	else if(new_len >= 0)
		return std::to_string(new_len);
	else 
		error_message(-1, -1, "Invalid length %d number.\n", new_len);
}

inline static std::string set_sign(std::vector<std::string> internal_binaries, bool new_sign)
{
	if(new_sign && is_signed(internal_binaries))
		return "1";
	else
		return "0";
}

#define containing_only_character_from_list(input, char_list) 	( input.find_first_not_of(char_list) == std::string::npos )
#define containing_character_from_list(input, char_list)	( input.find(char_list) == std::string::npos )
#define is_dont_care_string(input)								( containing_character_from_list(input,"XxZz") )
#define is_string_of_radix(input, radix) 		(radix == 16)?	( containing_only_character_from_list(input,"0123456789abcdefABCDEFXxZz") ) \
												:(radix == 10)?	( containing_only_character_from_list(input,"0123456789") ) \
												:(radix == 8)?	( containing_only_character_from_list(input,"01234567xXzZ") ) \
												:(radix == 2)?	( containing_only_character_from_list(input,"01xXzZ") ) \
												:false

#define MSB 0

#define PUSH_MS_BITSTRING(str,chr)	str.insert(MSB,chr)
#define PUSH_MSB(str,chr)			str.insert(MSB,std::string(1,chr))
#define SET_MSB(str,chr)			str[MSB] = std::tolower(chr)
#define GET_MSB(str)				std::tolower(str[MSB])

#define POP_LSB(str)				std::tolower(str[str.length()-1]); str.pop_back()
#define PUSH_LSB(str)				str.append(std::string(1,chr))
#define PUSH_LS_BITSTRING(str)		str.append(chr)

static unsigned long long v_strtoull(std::string orig_number, int radix)
{
	if (!is_string_of_radix(orig_number, radix))
		error_message(-1, -1, "Invalid base %d number: %s.\n", radix, orig_number);

	if (is_dont_care_string(orig_number))
		error_message(-1, -1, "Invalid number for conversion, contains don't care values: %s.\n", orig_number.c_str());

	if( orig_number.length() >= (sizeof(unsigned long long)*8) )
		error_message(-1, -1, "This base %d number (%s) is too long for Odin\n", radix, orig_number.c_str());

	return std::strtoull(orig_number.c_str(), NULL, radix);
}


static std::string to_bitstring(std::string orig_string, int radix)
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
				char prev = '0';
				short flag = 0;
				for(int i=orig_string.length()-1; i>=0; i--)
				{
					short new_pair = ((prev-'0')%2)*10 + (orig_string[i]-'0');
					std::string div(std::to_string(new_pair/2));
					std::string rem(std::to_string(new_pair%2));

					prev = orig_string[i];
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
			case 2:
			case 8:
			case 16:
				switch(radix)
				{
					char lsb = POP_LSB(orig_string);
					case 2:
					{
						switch(lsb)
						{
							case '0': PUSH_MS_BITSTRING(result, "0");	break;
							case '1': PUSH_MS_BITSTRING(result, "1");	break;
							case 'x': PUSH_MS_BITSTRING(result, "x");	break;
							case 'z': PUSH_MS_BITSTRING(result, "z");	break;
							default:  bad_value(lsb);		break;
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
							default:  bad_value(lsb);		break;
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
							default:  bad_value(lsb);			break;
						}
						break;
					}
					default:
					{
						error_message(-1, -1, "Invalid base %d number: %s.\n", radix, orig_string);
						break;
					}
				}
			default:
			{
				error_message(-1, -1, "Invalid base %d number: %s.\n", radix, orig_string);
				break;
			}
		}
	}
	return result;
}

std::vector<std::string> standardize_input(std::string input_number)
{	
	//remove underscores
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());
	
	auto loc = input_number.find("\'");
	if(loc == std::string::npos)
	{
		input_number.insert(0, "\'sd");
		loc = 0;
	}

	std::string bin_length = (loc == 0)? DEFAULT_BIT_WIDTH: input_number.substr(0,loc);

	std::string is_signed = "0";
	if(std::tolower(input_number[loc+1]) == 's')
	{
		input_number.erase (input_number.begin()+loc+1);
		is_signed = "1";
	}
	std::string binary = "DEADBEEF";

	switch(std::tolower(input_number[loc+1]))
	{
		case 'b':	binary = to_bitstring(input_number.substr(loc+2), 2); break;
		case 'o':	binary = to_bitstring(input_number.substr(loc+2), 8); break;
		case 'd':	binary = to_bitstring(input_number.substr(loc+2), 10); break;
		case 'h':	binary = to_bitstring(input_number.substr(loc+2), 16); break;
		default:	error_message(-1, -1, "Invalid base (%c) number: %s.\n", input_number[loc+1], input_number); break;
	}

	return {is_signed, bin_length, binary};
}


//resize bitstring to its own length
inline static std::vector<std::string> resize(std::vector<std::string> internal_binary_number)
{
	//expand to length
	long long size_dif = get_len(internal_binary_number)-get_bits(internal_binary_number).length();
	if(size_dif > 0)
	{
		std::string padding_bits(size_dif, (!is_signed(internal_binary_number) || GET_MSB(get_bits(internal_binary_number)) =='x')? '0': GET_MSB(get_bits(internal_binary_number)));
		PUSH_MS_BITSTRING(get_bits(internal_binary_number), padding_bits);
	}

	//truncate to length
	else if (size_dif < 0)
		get_bits(internal_binary_number) = get_bits(internal_binary_number).substr(-size_dif);

	return internal_binary_number;
}

// convert internal format to verilog
std::string v_bin_string(std::vector<std::string> internal_binary_number)
{
	std::string output = "b\'";

	if(get_len(internal_binary_number)>0)
		output += internal_binary_number[1];

	output += "\'";

	if(is_signed(internal_binary_number)) 
		output += "s";

	output += "b";

	//final resize
	resize(internal_binary_number);

	output += get_bits(internal_binary_number);

	return output;
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

inline static const char v_switch(	const char a, const char lut[4], char *FILE, int LINE) 
{
	return 	((a) == '0')				?	lut[REF_0] :
			((a) == '1')				?	lut[REF_1] :
			(std::tolower(a) == 'x')	?	lut[REF_x] : 
			(std::tolower(a) == 'z')	?	lut[REF_z] :
											_bad_value(a, FILE,LINE)
			;
}

inline static const char v_switch(	const char a, const char b, const char lut[4][4], char *FILE, int LINE) 
{
	return 	((a) == '0')				? 	v_switch(b, lut[REF_0], FILE, LINE)	:
			((a) == '1')				? 	v_switch(b, lut[REF_1], FILE, LINE) :
			(std::tolower(a) == 'x')	?	v_switch(b, lut[REF_x], FILE, LINE)	: 
			(std::tolower(a) == 'z')	?	v_switch(b, lut[REF_z], FILE, LINE) :
											_bad_value(a, FILE, LINE)
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

#define v_buf(a)				v_switch(a, 			l_buf,		__FILE__,__LINE__)
#define v_not(a)				v_switch(a, 			l_not,		__FILE__,__LINE__)
#define v_and(a,b)				v_switch(a,b, 			l_and,		__FILE__,__LINE__)
#define v_or(a,b)				v_switch(a,b, 			l_or,		__FILE__,__LINE__)
#define v_xor(a,b)				v_switch(a,b, 			l_xor,		__FILE__,__LINE__)
#define v_nand(a,b)				v_switch(a,b, 			l_nand,		__FILE__,__LINE__)
#define v_nor(a,b)				v_switch(a,b, 			l_nor,		__FILE__,__LINE__)
#define v_xnor(a,b)				v_switch(a,b, 			l_xnor,		__FILE__,__LINE__)
#define v_notif1(in,control)	v_switch(in,control, 	l_notif1,	__FILE__,__LINE__)
#define v_notif0(in,control)	v_switch(in,control, 	l_notif0,	__FILE__,__LINE__)
#define v_bufif1(in,control)	v_switch(in,control, 	l_bufif1,	__FILE__,__LINE__)
#define v_bufif0(in,control)	v_switch(in,control, 	l_bufif0,	__FILE__,__LINE__)
#define v_rpmos(in, control)	v_switch(in,control, 	l_rpmos,	__FILE__,__LINE__)
#define v_pmos(in,control)		v_switch(in,control, 	l_pmos,		__FILE__,__LINE__)
#define v_rnmos(in, control)	v_switch(in,control, 	l_rnmos,	__FILE__,__LINE__)
#define v_nmos(in,control)		v_switch(in,control, 	l_nmos,		__FILE__,__LINE__)
#define v_ternary(in,control)	v_switch(in,control,	l_ternary,	__FILE__,__LINE__)

/***
 * these are extended defines to simplify our lives
 */
#define add_func(a, b, carry)	v_xor(v_xor(a,b),carry)
#define carry_func(a, b, carry)	v_or(v_and(a,b),v_and(v_xor(a,b),carry))



/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */

inline static std::vector<std::string> V_NEG(std::vector<std::string> a)
{
	std::string result = "";
	for(int i=get_bits(a).length()-1; i >= MSB; i--)
		PUSH_MSB(result, v_not(get_bits(a)[i]));
	
	return {set_sign(a, false), set_len(a, 0), result};
}

inline static std::vector<std::string> V_ADD(std::vector<std::string> a)
{
	return a;
}

inline static std::vector<std::string> V_MINUS(std::vector<std::string> a)
{
	return V_PLUS_PLUS(V_NEG(a));
}

inline static std::vector<std::string>  V_MINUS_MINUS(std::vector<std::string> a)
{
	std::string result = "1";
	for(int i=get_bits(a).length()-1; i >= 0; i--)
	{
		const char tmp_add = v_xnor(get_bits(a)[i], GET_MSB(result));
		const char tmp_carry = v_or(get_bits(a)[i], GET_MSB(result));

		SET_MSB(result, tmp_add);
		PUSH_MSB(result, tmp_carry);
	}
	return {set_sign(a, true), set_len(a, 0), result};
}

inline static std::vector<std::string> V_PLUS_PLUS(std::vector<std::string> a)
{
	std::string result = "1";
	for(int i=get_bits(a).length()-1; i >= 0; i--)
	{
		const char tmp_add = v_xor(get_bits(a)[i], GET_MSB(result));
		const char tmp_carry = v_and(get_bits(a)[i], GET_MSB(result));

		SET_MSB(result, tmp_add);
		PUSH_MSB(result, tmp_carry);
	}
	return {set_sign(a, true), set_len(a, 0), result};
}

inline static std::vector<std::string> V_REDUCTION_AND(std::vector<std::string> a)
{
	int end = get_bits(a).length()-1;
	std::string result = std::string(get_bits(a)[end--],1);
	for(int i=end; i>=MSB; i--)
		SET_MSB(result,v_and(get_bits(a)[i],GET_MSB(result)));

	return {set_sign(a, false), set_len(a, 1), result};
}

inline static std::vector<std::string> V_REDUCTION_OR(std::vector<std::string> a)
{
	int end = get_bits(a).length()-1;
	std::string result = std::string(get_bits(a)[end--],1);
	for(int i=end; i>=MSB; i--)
		SET_MSB(result,v_or(get_bits(a)[i],GET_MSB(result)));

	return {set_sign(a, false), set_len(a, 1), result};
}

inline static std::vector<std::string> V_REDUCTION_XOR(std::vector<std::string> a)
{
	int end = get_bits(a).length()-1;
	std::string result = std::string(get_bits(a)[end--],1);
	for(int i=end; i>=MSB; i--)
		SET_MSB(result,v_xor(get_bits(a)[i],GET_MSB(result)));

	return {set_sign(a, false), set_len(a, 1), result};
}

inline static std::vector<std::string> V_REDUCTION_NAND(std::vector<std::string> a)
{
	int end = get_bits(a).length()-1;
	std::string result = std::string(get_bits(a)[end--],1);
	for(int i=end; i>=MSB; i--)
		SET_MSB(result,v_nand(get_bits(a)[i],GET_MSB(result)));

	return {set_sign(a, false), set_len(a, 1), result};
}

inline static std::vector<std::string> V_REDUCTION_NOR(std::vector<std::string> a)
{
	int end = get_bits(a).length()-1;
	std::string result = std::string(get_bits(a)[end--],1);
	for(int i=end; i>=MSB; i--)
		SET_MSB(result,v_nor(get_bits(a)[i],GET_MSB(result)));

	return {set_sign(a, false), set_len(a, 1), result};
}

inline static std::vector<std::string> V_REDUCTION_XNOR(std::vector<std::string> a)
{
	int end = get_bits(a).length()-1;
	std::string result = std::string(get_bits(a)[end--],1);
	for(int i=end; i>=MSB; i--)
		SET_MSB(result,v_xnor(get_bits(a)[i],GET_MSB(result)));

	return {set_sign(a, false), set_len(a, 1), result};
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
static std::string binops(std::string a_in, std::string op, std::string b_in)
{

	/**********************************
	 * Gather input 
	 */

	/* get A */
	int offset = 0;
	auto loc = a_in.find("\'");
	long a_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(a_in.substr(0,loc),2);
	bool a_signed = false;

	offset++;// skip " ' "
	if(std::tolower(a_in[loc+offset]) == 's')
	{
		a_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string a = a_in.substr(loc+offset);

	/* get B */
	offset = 0;
	loc = b_in.find("\'");
	long b_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(b_in.substr(0,loc), 2);
	bool b_signed = false;

	offset++;// skip " ' "
	if(std::tolower(b_in[loc+offset]) == 's')
	{
		b_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string b = b_in.substr(loc+offset);
	
	/**********************************
	 * Process Operator
	 */
	// bitwise Ops
	if(op == "^" || op == "~^" || op == "^~" || op == "&" || op == "~&" || op == "|" || op == "~|")
	{
		std::string result = "";

		long std_length = std::max(a_length, b_length);
		std::string a = resize(a, std_length, a_signed);
		std::string b = resize(b, std_length, b_signed);

		for(int i=std_length-1; i>=MSB; i--)
		{
			PUSH_MSB(result, 
				/*reduction AND*/	(op == "&")		?	v_and(a[i], b[i])	:
				/*reduction OR*/	(op == "|")		?	v_or(a[i], b[i])	:
				/*reduction XOR*/	(op == "^")		?	v_xor(a[i], b[i])	:
				/*reduction NAND*/	(op == "~&")	?	v_nand(a[i], b[i])	:
				/*reduction NOR*/	(op == "~|")	?	v_nor(a[i], b[i])	:
				/*reduction XNOR*/	(op == "~^")	?	v_xnor(a[i], b[i])	:
				/*reduction XNOR*/	(op == "^~")	?	v_xnor(a[i], b[i])	:
														bad_ops(op)
			);
		}
		return v_bin_string(result, std_length, false);
	}
	// case equals are not picky with don't care
	else if(op == "===" || op == "!==")
	{
		char passing_value = ( op == "===" )? '1': '0';
		std::string result(1,passing_value);

		long std_length = std::max(a_length, b_length);
		std::string a = resize(a, std_length, a_signed);
		std::string b = resize(b, std_length, b_signed);

		for(int i=std_length-1; i >= MSB && GET_MSB(result) == passing_value; i--)
			SET_MSB(result, (a[i] == b[i])? '1': '0');

		if( op == "!==" )
			SET_MSB(result, (GET_MSB(result) == '0')? '1': '0');
	}
	// Shifts
	else if( op == "<<" ||  op == "<<<" || op == ">>" || op == ">>>" )
	{
		if(!is_dont_care_string(b))
		{
			// flip the shift if signed integer
			if(b_signed && b[b.length()-1] == '1')
			{
				std::string two_comp = arithmetic("-", v_bin_string(b, 0, true));
				op =	(op == "<<<")	?	">>>"	:
						(op == "<<")	?	">>"	:
						(op == ">>>")	?	"<<<"	:
						/*(op == ">>")	?*/	"<<"	;
			}

			//these operation are limited to shift by the amount dictated by a long long 2^64 (doubt we will ever reach this!) 
			unsigned long long shift_by = v_strtoull(b,2);
			if( op == "<<" || op == "<<<")
				return v_bin_string(a + std::string('0',shift_by), shift_by+a.length(), a_signed); 
			else if(op == ">>")
				return v_bin_string(std::string('0', shift_by) + a, shift_by+a.length(), a_signed); 
			else /*if(op == ">>>" )*/
				return v_bin_string(std::string(a[0], shift_by) + a, shift_by+a.length(), a_signed); 
			}
		}
	}
	else if (( op == "&&" || op == "||")
	||( op == "<"  || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" )		
	||( op == "+"  || op == "-" || op == "*"  || op == "**" )
	||( op == "/"  || op == "%"))
	{
		/* dont care sensitive ops */
		if(!is_dont_care_string(a) && !is_dont_care_string(b))
		{
			if(op == "&&" )
			{
				result = arithmetic(arithmetic("|", a_in), "&", arithmetic("|", b_in));
			}
			else if(op == "||" )
			{
				result = arithmetic(arithmetic("|", a_in), "|", arithmetic("|", b_in));
			}
			// todo signed int!!
			else if( op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" )
			{
				// -1 less, 0 equal, 1 greater
				short condition_code = -1; 

				long std_length = std::max(a_length, b_length);
				std::string a = resize(a, std_length, a_signed);
				std::string b = resize(b, std_length, b_signed);

				for(int i=std_length-1; i>=MSB && condition_code == 0; i--)
				{
					condition_code = 	(a[i] == b[i])	?	0	:
										(a[i] == '0')		?	-1	:
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
				long std_length = std::max(a_length, b_length);
				std::string a = resize(a, std_length, a_signed);
				std::string b = resize(b, std_length, b_signed);

				for(int i=std_length-1; i>=MSB; i--)
				{
					result[i+1] = add_func(a[i], b[i], previous_carry);
					previous_carry = carry_func(a[i], b[i], previous_carry);
				}
				result[0] = previous_carry;
			
				result_length = length+1;
				result_sign = false; //TODO
			}
			else if( op == "-" )
			{
				result = arithmetic(a_in, "+", arithmetic("-", b_in));
			}
			else if( op == "*" )
			{
				result = "\'sb0";
				for(int i=a_in.length()-1; i >= MSB; i--)
					if(a_in[i] == '1')
						result = arithmetic(result, "+", arithmetic(b_in , "<<<", v_bin_string(to_bitstring(std::to_string(a_in.length()-1-i),10,0,false), 0, false)));
			}
			else if( op == "**" )
			{
				std::string result = "\'sb1";
				while((b_in = arithmetic("|", b_in)) != "\'sb0" )
				{
					std::string eval_ge = arithmetic("|", b_in)
					b_in = arithmetic("--", b_in);
					result = arithmetic( result, length, true, "*", a, length, true);
				}
			}
			else if( op == "/" || op == "%" )
			{
				//sensitive to division by zero
				if( '0' != arithmetic("|", b, length, b_signed))
				{
					result = "0";
					//TODO signed division!
					while(arithmetic(b, length, b_signed, "<", a, length, a_signed) ) )
					{
						std::string count = "1";
						std::string sub_with = b;
						for( std::string tmp = sub_with; arithmetic(tmp, length, true, "<", a, length, a_signed); arithmetic(tmp, length, true, "<<", "1", 2, true))
						{
							count = arithmetic( count, length, true, "<<", "1", 2, true);
							sub_with = tmp;
						}
						a = arithmetic( a, length, a_signed, "-", sub_with, length, true);
						result = arithmetic ( result, length, true, "+", count, length, true);
					}
					if( op == "%" )
					{
						result = a;
					}
				}
			}
		}
	}
	else
	{
		error_message(-1,-1,"Operator %s is invalid for binary operation", op);
	}
	return v_bin_string(result, result_length, result_sign);
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

static std::string V_TERNARY(std::string a_bits, bool a_sign, long a_len, std::string b_bits, bool b_sign, long b_len, std::string c_bits, bool c_sign, long c_len)
{
	/*	if a evaluates properly	*/
	if(a_bits == '1')
		return b;
	else if(a_bits == '0')
		return c; 

	/* get B */
	offset = 0
	loc = b.find("\'");
	long b_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(b.substr(0,loc),2);

	offset++;// skip " ' "
	if(std::tolower(b[loc+offset]) == 's')
		offset ++; //skip " s "

	offset++; // skip  " [ b, o, d, h ] "
	std::string a = b.substr(loc+offset);

	/* get C */
	offset = 0
	loc = c.find("\'");
	long c_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(c.substr(0,loc),2);

	offset++;// skip " ' "
	if(std::tolower(b[loc+offset]) == 's')
		offset ++; //skip " s "

	offset++; // skip  " [ b, o, d, h ] "
	std::string c = c.substr(loc+offset);


	std::string result = "x";
	long std_length = std::max(std::max(b_length, c_length);
	bool result_sign = false;

	std::string b = resize(b, std_length, result_sign);
	std::string c = resize(c, std_length, result_sign);

	for(int i=std_length-1; i>=MSB; i--)
		result[i] = v_ternary(b[i], c[i]);

	return v_bin_string(result, result_length, result_sign);
}


/***
 *     __   __       ___  __   __           ___       __       
 *    /  ` /  \ |\ |  |  |__) /  \ |       |__  |    /  \ |  | 
 *    \__, \__/ | \|  |  |  \ \__/ |___    |    |___ \__/ |/\| 
 *                                                             
 */

static std::string arithmetic(std::string op, std::string a_in)
{
	/**********************************
	 * Gather input 
	 */
	int offset = 0;
	auto loc = a_in.find("\'");
	long a_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(a_in.substr(0,loc), 2);
	bool a_signed = false;

	offset++;// skip " ' "
	if(std::tolower(a[loc+offset]) == 's')
	{
		a_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string a = a_in.substr(loc+offset);

	/**********************************
	 * return Process Operator via ternary
	 */
	return	v_bin_string(	(op == "~")		?		V_NEG(a, a_signed, a_length):
							(op == "-")		?		V_MINUS(a, a_signed, a_length):
							(op == "+")		?		V_ADD(a, a_signed, a_length):
							(op == "++")	?		V_PLUS_PLUS(a, a_signed, a_length):
							(op == "--")	?		V_MINUS_MINUS(a, a_signed, a_length):
							(op == "&")		?		V_REDUCTION_AND(a, a_signed, a_length):
							(op == "|")		?		V_REDUCTION_OR(a, a_signed, a_length):
							(op == "^")		?		V_REDUCTION_XOR(a, a_signed, a_length):
							(op == "~&")	?		V_REDUCTION_NAND(a, a_signed, a_length):
							(op == "~|")	?		V_REDUCTION_NOR(a, a_signed, a_length):
							(op == "~^"	
							|| op == "~^")	?		V_REDUCTION_XNOR(a, a_signed, a_length):
							(op == "!")		?		V_LOGICAL_NOT(a, a_signed, a_length):
													bad_ops(op));

}

static std::string arithmetic(std::string a_in, std::string op, std::string b_in)
{

	/**********************************
	 * Gather input 
	 */

	/* get A */
	int offset = 0;
	auto loc = a_in.find("\'");
	long a_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(a_in.substr(0,loc),2);
	bool a_signed = false;

	offset++;// skip " ' "
	if(std::tolower(a_in[loc+offset]) == 's')
	{
		a_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string a = a_in.substr(loc+offset);

	/* get B */
	offset = 0;
	loc = b_in.find("\'");
	long b_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(b_in.substr(0,loc), 2);
	bool b_signed = false;

	offset++;// skip " ' "
	if(std::tolower(b_in[loc+offset]) == 's')
	{
		b_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string b = b_in.substr(loc+offset);
	
	/**********************************
	 * return Process Operator via ternary
	 */
	return	(op == "~")		?		V_NEG(a, a_signed, a_length):
			(op == "-")		?		V_MINUS(a, a_signed, a_length):
			(op == "+")		?		V_ADD(a, a_signed, a_length):
			(op == "++")	?		V_PLUS_PLUS(a, a_signed, a_length):
			(op == "--")	?		V_MINUS_MINUS(a, a_signed, a_length):
			(op == "&")		?		V_REDUCTION_AND(a, a_signed, a_length):
			(op == "|")		?		V_REDUCTION_OR(a, a_signed, a_length):
			(op == "^")		?		V_REDUCTION_XOR(a, a_signed, a_length):
			(op == "~&")	?		V_REDUCTION_NAND(a, a_signed, a_length):
			(op == "~|")	?		V_REDUCTION_NOR(a, a_signed, a_length):
			(op == "~^"	
			|| op == "~^")	?		V_REDUCTION_XNOR(a, a_signed, a_length):
			(op == "!")		?		V_LOGICAL_NOT(a, a_signed, a_length):
									bad_ops(op);








	// bitwise Ops
	if(op == "^" || op == "~^" || op == "^~" || op == "&" || op == "~&" || op == "|" || op == "~|")
	{

	}
	// case equals are not picky with don't care
	else if(op == "===" || op == "!==")
	{

	}
	// Shifts
	else if( op == "<<" ||  op == "<<<" || op == ">>" || op == ">>>" )
	{
		if(!is_dont_care_string(b))
		{
			// flip the shift if signed integer
			if(b_signed && b[b.length()-1] == '1')
			{
				std::string two_comp = arithmetic("-", v_bin_string(b, 0, true));
				op =	(op == "<<<")	?	">>>"	:
						(op == "<<")	?	">>"	:
						(op == ">>>")	?	"<<<"	:
						/*(op == ">>")	?*/	"<<"	;
			}

			unsigned long long shift_by = v_strtoull(b,2);
			if( op == "<<" || op == "<<<")
			else if(op == ">>")
			else /*if(op == ">>>" )*/
			}
		}
	}
	else if (( op == "&&" || op == "||")
	||( op == "<"  || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" )		
	||( op == "+"  || op == "-" || op == "*"  || op == "**" )
	||( op == "/"  || op == "%"))
	{
		/* dont care sensitive ops */
		if(!is_dont_care_string(a) && !is_dont_care_string(b))
		{
			if(op == "&&" )
			{
			}
			else if(op == "||" )
			{
			}
			// todo signed int!!
			else if( op == "<" || op == ">" || op == "<=" || op == ">=" || op == "==" || op == "!=" )
			{

			}

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
			else if( op == "/" || op == "%" )
			{
				//sensitive to division by zero
				if( '0' != arithmetic("|", b, length, b_signed))
				{

				}
			}
		}
	}
	else
	{
		error_message(-1,-1,"Operator %s is invalid for binary operation", op);
	}







}

static std::string arithmetic(std::string a_in, std::string op1 ,std::string b_in, std::string op2, std::string c_in)
{

	/**********************************
	 * Gather input 
	 */

	/* get A */
	int offset = 0;
	auto loc = a_in.find("\'");
	long a_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(a_in.substr(0,loc),2);
	bool a_signed = false;

	offset++;// skip " ' "
	if(std::tolower(a_in[loc+offset]) == 's')
	{
		a_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string a = a_in.substr(loc+offset);

	/* get B */
	offset = 0;
	loc = b_in.find("\'");
	long b_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(b_in.substr(0,loc), 2);
	bool b_signed = false;

	offset++;// skip " ' "
	if(std::tolower(b_in[loc+offset]) == 's')
	{
		b_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string b = b_in.substr(loc+offset);

	/* get C */
	offset = 0;
	loc = c_in.find("\'");
	long c_length = (loc == 0)? DEFAULT_BIT_WIDTH: v_strtoull(c_in.substr(0,loc), 2);
	bool c_signed = false;

	offset++;// skip " ' "
	if(std::tolower(c_in[loc+offset]) == 's')
	{
		c_signed = true;
		offset ++; //skip " s "
	}
	offset++; // skip  " [ b, o, d, h ] "
	std::string c = c_in.substr(loc+offset);
	
	/**********************************
	 * return Process Operator via ternary
	 */
	return	(op1 == "?"
			&& op2 == ":")		?	V_TERNARY(a, a_signed, a_length, b, b_signed, b_length, c, c_signed, c_length,):
									bad_ops(op);
}