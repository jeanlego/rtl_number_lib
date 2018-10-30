#include <cstdarg>
#include <stdio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <assert.h>

#include "rtl_int.h"
#include "rtl_utils.h"

/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *           Dr. Kenneth B. Kent (ken@unb.ca)
 *           for the Reconfigurable Computing Research Lab at the
 *           Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

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

static const char l_unk[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{'x','x','x','x'},
	/* 1 */	{'x','x','x','x'},
	/* x */	{'x','x','x','x'},
	/* z */	{'x','x','x','x'}
};

#define unroll_1d(lut) { lut[0], lut[1], lut[2], lut[3] }
#define unroll_2d(lut) { unroll_1d(lut[0]), unroll_1d(lut[1]), unroll_1d(lut[2]), unroll_1d(lut[3]) }

static const char l_sum[4][4][4] = {
	unroll_2d(l_xor),
	unroll_2d(l_xnor),
	unroll_2d(l_unk),
	unroll_2d(l_unk)
};

static const char l_carry[4][4][4] = {
	unroll_2d(l_and),
	unroll_2d(l_or),
	unroll_2d(l_ternary),
	unroll_2d(l_ternary)
};

#define	_0 0
#define	_1 1
#define	_x 2
#define	_z 3

inline static uint8_t _chr2id(const char in, const char *FUNCT, int LINE)
{
	switch(std::tolower(in))
	{
		case '0':	return _0;
		case '1':	return _1;
		case 'x':	return _x;
		case 'z':	return _z;
		default:
			_bad_value(in, FUNCT, LINE);
			std::abort();
	}
}

#define v_op(op, ... ) _v_op(op, {__VA_ARGS__} , __func__, __LINE__)
inline static char _v_op( const char lut[4], std::vector<char> args, const char *FUNCT, int LINE) 
{
	assert(args.size() == 1);
	const char my_arg = args[0];
	args.erase(args.begin());

	return lut[_chr2id(my_arg, FUNCT, LINE)];
}

inline static char _v_op(const char lut[4][4], std::vector<char> args, const char *FUNCT, int LINE) 
{
	assert(args.size() == 2);
	const char my_arg = args[0];
	args.erase(args.begin());

	return _v_op(lut[_chr2id(my_arg, FUNCT, LINE)], args, FUNCT, LINE);
}

inline static char _v_op(const char lut[4][4][4], std::vector<char> args, const char *FUNCT, int LINE) 
{
	assert(args.size() == 3);
	const char my_arg = args[0];
	args.erase(args.begin());

	return _v_op(lut[_chr2id(my_arg, FUNCT, LINE)], args, FUNCT, LINE);
}


typedef enum
{
    LESS_THAN,
    GREATHER_THAN,
    EQUAL,
    UNKNOWN
}EVAL_RESULT;

inline static EVAL_RESULT return_int_eval(const std::string& a_in, const std::string& b_in)
{
	bool neg_a = (is_negative(a_in));
	bool neg_b = (is_negative(b_in));
	
	std::string a;
	if(neg_a)	a = V_MINUS(a_in);
	else		a = V_ADD(a_in);

	std::string b;
	if(neg_b)	b = V_MINUS(b_in);
	else		b = V_ADD(b_in);


	bool invert_result = ((!neg_a && neg_b) || (neg_a && !neg_b));

	std::size_t std_length = std::max(a.size(), b.size());
	const char pad_a = get_padding_bit(a);
	const char pad_b = get_padding_bit(b);

	std::string result = "";

	for(int i=std_length-1; i >= 0; i--)
	{
		char bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		char bit_b = pad_b;
		if(i < b.size())
			bit_b = b[i];

		if (bit_a == '1' && bit_b == '0')	
			return (invert_result)? LESS_THAN: GREATHER_THAN;

		else if	(bit_a == '0' && bit_b == '1')	
			return (invert_result)? GREATHER_THAN: LESS_THAN;
			
		else if	(bit_a == 'x' || bit_b == 'x')	
			return UNKNOWN;
	}
	return EQUAL;
}

inline static EVAL_RESULT return_int_eval(long a, const std::string& b)
{
	std::string bits_value = string_of_radix_to_bitstring(std::to_string(a), 2);
	std::string BEbits(bits_value.crbegin(),bits_value.crend());
	return return_int_eval(BEbits,b);
}
inline static EVAL_RESULT return_int_eval(const std::string& a,long b)
{
	std::string bits_value = string_of_radix_to_bitstring(std::to_string(b), 2);
	std::string BEbits(bits_value.crbegin(),bits_value.crend());
	return return_int_eval(a, BEbits);
}

inline static std::string V_REDUX(const std::string& a, const char lut[4])
{
	std::string result = "";
	for(int i=0; i <a.size(); i++)
		result.push_back(v_op(lut,a[i]));

	return result;
}

inline static std::string V_REDUX(const std::string& a, const char lut[4][4])
{
	char result = 0;
	for(int i=0; i < a.size(); i++)
	{
		if(!result)
			result = a[i];
		else
			result = v_op(lut, a[i], result);
	}

	return std::string(1,result);
}

inline static std::string V_REDUX(const std::string& a, const std::string& b, const char lut[4][4])
{
	std::size_t std_length = std::max(a.size(), b.size());
	const char pad_a = get_padding_bit(a);
	const char pad_b = get_padding_bit(b);

	std::string result = "";

	for(int i=0; i < std_length; i++)
	{
		char bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		char bit_b = pad_b;
		if(i < b.size())
			bit_b = b[i];

		result.push_back(v_op(lut, bit_a, bit_b));
	}

	return result;
}

inline static std::string V_INCREMENT(const std::string& a, const char lut_adder[4][4], const char lut_carry[4][4], signed const char initial)
{
	std::string result = "";
	char tmp_carry  = initial;
	for(int i=0; i<a.size(); i++)
	{
		result.push_back(v_op(lut_adder, a[i],tmp_carry));
		tmp_carry = v_op(lut_carry, a[i], tmp_carry);
	}
	result.push_back(tmp_carry);

	return result;
}

inline static std::string V_SUM(const std::string& a, const std::string& b, const char lut_adder[4][4][4], const char lut_carry[4][4][4], signed const char initial)
{
	std::size_t std_length = std::max(a.size(), b.size());
	const char pad_a = get_padding_bit(a);
	const char pad_b = get_padding_bit(b);

	char previous_carry = initial;
	std::string result = "";

	for(int i=0; i < std_length; i++)
	{
		char bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		char bit_b = pad_b;
		if(i < b.size())
			bit_b = b[i];

		result.push_back(v_op(lut_adder, bit_a, bit_b, previous_carry));
		previous_carry = v_op(lut_carry, bit_a, bit_b, previous_carry);
	}
	result.push_back(previous_carry);

	return result;
}

/**
 * Shift operations
 */

inline static void shift_op(std::string& bit_string, int len, signed char padding_bit)
{
	std::string pad(std::abs(len),padding_bit);
	//shift left , let it grow, let it grow ...
	if(len > 0)	
	{
		bit_string.insert(0,pad);
	}
	//shift right, because it's the rightest thing to do
	else if(len < 0)
	{
		bit_string.erase(0, len);
		bit_string.append(pad);
	}
}

/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */

std::string V_NEG(const std::string& a)
{
	return V_REDUX(a,l_not);
}

std::string V_PLUS_PLUS(const std::string& a)
{
	return V_INCREMENT(a, l_xor, l_and, '1');
}

std::string  V_MINUS_MINUS(const std::string& a)
{
	return V_INCREMENT(a, l_xnor, l_or, '0');
}

std::string V_ADD(const std::string& a)
{
	return std::string(a);
}

std::string V_MINUS(const std::string& a)
{
	return V_PLUS_PLUS(V_NEG(a));
}

std::string V_AND(const std::string& a)
{
	return V_REDUX(a, l_and);
}

std::string V_OR(const std::string& a)
{
	return V_REDUX(a, l_or);	
}

std::string V_XOR(const std::string& a)
{
	return V_REDUX(a, l_xor);
}

std::string V_NAND(const std::string& a)
{
	return V_NEG(V_AND(a));
}

std::string V_NOR(const std::string& a)
{
	return V_NEG(V_OR(a));
}

std::string V_XNOR(const std::string& a)
{
	return V_NEG(V_XOR(a));
}

std::string V_LOGICAL_NOT(const std::string& a)
{
	return V_NEG(V_OR(a));
}

/***
 *     __               __          __   __   ___  __       ___    __       
 *    |__) | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    |__) | | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                          
 */

std::string V_AND(const std::string& a,const std::string& b)
{
	return V_REDUX(a,b,l_and);
}

std::string V_OR(const std::string& a,const std::string& b)
{
	return V_REDUX(a,b,l_or);
}

std::string V_XOR(const std::string& a,const std::string& b)
{
	return V_REDUX(a,b,l_xor);
}

std::string V_NAND(const std::string& a,const std::string& b)
{
	return V_REDUX(a,b,l_nand);
}

std::string V_NOR(const std::string& a,const std::string& b)
{
	return V_REDUX(a,b,l_nor);
}

std::string V_XNOR(const std::string& a,const std::string& b)
{
	return V_REDUX(a,b,l_xnor);
}

std::string V_CASE_EQUAL(const std::string& a,const std::string& b)
{
	std::size_t std_length = std::max(a.size(), b.size());

	char last_a = get_padding_bit(a);
	char last_b = get_padding_bit(b); 

	for (int i=0; i<std_length; i++)
	{
		char cur_a = last_a;
		if(i < a.size())
			cur_a = a[i];
		
		char cur_b = last_b;
		if(i < b.size())
			cur_b = b[i];

		if(cur_a != cur_b)	return "0";
	}
	
	return "1";
}
std::string V_CASE_NOT_EQUAL(const std::string& a,const std::string& b)
{
	return V_LOGICAL_NOT(V_CASE_EQUAL(a,b));
}

std::string V_SIGNED_SHIFT_LEFT(const std::string& a_in, const std::string& b)
{
	if(is_dont_care_string(b))	
		return "x";
	
	std::string value(b.crbegin(), b.crend());
	std::string a(a_in);

	shift_op(a, v_strtoul(value,2), '0');
	return a;
}

std::string V_SHIFT_LEFT(const std::string& a, const std::string& b)
{
	return V_SIGNED_SHIFT_LEFT(a,b);
}

std::string V_SIGNED_SHIFT_RIGHT(const std::string& a_in, const std::string& b)
{
	if(is_dont_care_string(b))	
		return "x";
	
	std::string value(b.crbegin(), b.crend());
	std::string a(a_in);

	shift_op(a, -1 * v_strtoul(value,2), get_padding_bit(a));
	return a;
}

std::string V_SHIFT_RIGHT(const std::string& a_in, const std::string& b)
{
	if(is_dont_care_string(b))	
		return "x";

	std::string value(b.crbegin(), b.crend());
	std::string a(a_in);

	shift_op(a, -1 * v_strtoul(value,2), '0');
	return a;

}

/**
 * Logical Operations
 */
std::string V_LOGICAL_AND(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return V_AND(V_OR(a),V_OR(b));
}

std::string V_LOGICAL_OR(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return V_OR(V_OR(a),V_OR(b));
}

std::string V_LT(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(return_int_eval(a,b) == LESS_THAN) 		? "1":"0";
}

std::string V_GT(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(return_int_eval(a,b) == GREATHER_THAN) 	? "1":"0";
}

std::string V_LE(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(return_int_eval(a,b) != GREATHER_THAN) 	? "1":"0";
}

std::string V_GE(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(return_int_eval(a,b) != LESS_THAN) 		? "1":"0";
}

std::string V_EQUAL(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(return_int_eval(a,b) == EQUAL) 			? "1":"0";
}

std::string V_NOT_EQUAL(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(return_int_eval(a,b) != EQUAL) 			? "1":"0";
}

std::string V_ADD(const std::string& a, const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return V_SUM(a, b, l_sum, l_carry, '0');
}

std::string V_MINUS(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return V_SUM(a, V_NEG(b), l_sum, l_carry, '1');

}

std::string V_MULTIPLY(const std::string& a_in,const std::string& b_in)
{
	if(is_dont_care_string(a_in) || is_dont_care_string(b_in))
		return "x";

	std::string a;
	std::string b;

	bool neg_a = is_negative(a_in);
	bool neg_b = is_negative(b_in);
	
	if(neg_a)	
		a = V_MINUS(a_in);
	else
		a = a_in;

	if(neg_b)	
		b = V_MINUS(b_in);
	else
		b = b_in;
		
	bool invert_result = ((!neg_a && neg_b) || (neg_a && !neg_b));


	std::size_t std_length = std::max(b.size(),a.size())*2;
	std::string result = "0";

	for (auto bit_a = a.crbegin(); bit_a != a.crend(); ++bit_a)
	{
		if(*bit_a == '1')
			result = V_ADD(result,b);
		shift_op(b, 1, '0');
	}

	if(invert_result)	
		result = V_MINUS(result);

	return result;
}

std::string V_POWER(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "xxxx";
	
	EVAL_RESULT res_a = return_int_eval(a, 0);
	short val_a = 	(res_a == EQUAL) 			? 	0: 
					(res_a == LESS_THAN) 		? 	((return_int_eval(a,-1) == LESS_THAN))	?	-2: -1:
					/* GREATHER_THAN */  			((return_int_eval(a,1) == GREATHER_THAN))	?	2: 1;

	EVAL_RESULT res_b = return_int_eval(b, 0);
	short val_b = 	(res_b == EQUAL) 			? 	0: 
					(res_b == LESS_THAN) 		? 	-1:
					/* GREATHER_THAN */				1;

	//compute
	if(val_b == 1 && (val_a < -1 || val_a > 1 ))
	{
		std::string result = "1";
		
		std::string tmp_b = b;
		while(return_int_eval(tmp_b,0) == GREATHER_THAN)
		{
			tmp_b = V_MINUS_MINUS(tmp_b);
			result = V_MULTIPLY( result, a);
		}
		return result;
	}
	if (val_b == 0 || val_a == 1)	
	{
		return "0001";
	}
	else if(val_b == -1 && val_a == 0)
	{
		return "xxxx";
	}
	else if(val_a == -1)
	{
		if(is_negative(a)) 	// even
			return "1110";
		else				//	odd
			return "0001";
	}
	else	
	{
		return "0000";
	}
}

std::string V_DIV(const std::string& a_in,const std::string& b)
{
	std::string a(a_in);
	if(is_dont_care_string(a) || is_dont_care_string(b)
	|| return_int_eval(b,0) == EQUAL)
		return "x";

	
	std::string result = "0";
	//TODO signed division!
	while(return_int_eval(a, b) == GREATHER_THAN )
	{
		std::string  count = "1";
		std::string  sub_with = b;
		std::string  tmp = b;
		while(return_int_eval(tmp, a) == LESS_THAN)
		{
			sub_with = tmp;
			shift_op(count, 1,'0');
			shift_op(tmp, 1,'0');
		}
		a = V_MINUS(a, sub_with);
		result = V_ADD(result, count);
	}
	return result;
}

std::string V_MOD(const std::string& a_in,const std::string& b)
{
	std::string a(a_in);

	if(is_dont_care_string(a) || is_dont_care_string(b)
	|| return_int_eval(b, 0) == EQUAL)
		return "x";

	//TODO signed division!
	while(return_int_eval(b, a)  == LESS_THAN)
	{
		std::string  sub_with = b;
		for( std::string  tmp = b; return_int_eval(tmp, a) == LESS_THAN; shift_op(tmp, 1,'0'))
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
std::string V_TERNARY(const std::string& a, const std::string& b, const std::string& c)
{
	/*	if a evaluates properly	*/
	EVAL_RESULT eval = return_int_eval(V_LOGICAL_NOT(a),0);

	if(eval == UNKNOWN)		return V_REDUX(b,c,l_ternary);
	else if(eval == EQUAL)	return b;
	else					return c;
}
