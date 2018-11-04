/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include <string>

#include "rtl_int.h"
#include "rtl_utils.h"
#include "rtl_primitive.h"

/**
 * Compare operations
 */
typedef enum
{
    LESS_THAN,
    GREATHER_THAN,
    EQUAL,
    UNKNOWN
}EVAL_RESULT;

inline static EVAL_RESULT eval_op(const std::string& a_in, const std::string& b_in)
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

	int std_length = std::max(a.size(), b.size());
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

inline static EVAL_RESULT eval_op(long a, const std::string& b)
{
	std::string bits_value = string_of_radix_to_bitstring(std::to_string(a), 10);
	std::string BEbits(bits_value.crbegin(),bits_value.crend());
	return eval_op(BEbits,b);
}
inline static EVAL_RESULT eval_op(const std::string& a,long b)
{
	std::string bits_value = string_of_radix_to_bitstring(std::to_string(b), 10);
	std::string BEbits(bits_value.crbegin(),bits_value.crend());
	return eval_op(a, BEbits);
}

/**
 * Not operations
 */
inline static void not_op(std::string& a)
{
	for(int i=0; i <a.size(); i++)
		a[i] = (v_op(l_not,a[i]));
}

/**
 * Unary Reduction operations
 */
inline static void inline_redux_op(std::string& a, const char lut[4][4])
{
	if(a.empty())
		return;

	while(a.size() > 1)
	{
		a[0] = v_op(lut, a[0], a[1]);
		a.erase(a.begin()+1);
	}

}

/**
 * Binary Reduction operations
 */
inline static std::string redux_op(const std::string& a, const std::string& b, const char lut[4][4])
{
	int std_length = std::max(a.size(), b.size());
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

/**
 * Addition operations
 */
inline static std::string sum_op(const std::string& a, const std::string& b, const char initial_carry)
{
	int std_length = std::max(a.size(), b.size());
	const char pad_a = get_padding_bit(a);
	const char pad_b = get_padding_bit(b);

	char previous_carry = initial_carry;
	std::string result = "";

	for(int i=0; i < std_length; i++)
	{
		char bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		char bit_b = pad_b;
		if(i < b.size())
			bit_b = b[i];

		result.push_back(v_op(l_sum, bit_a, bit_b, previous_carry));
		previous_carry = v_op(l_carry, bit_a, bit_b, previous_carry);
	}
	result.push_back(previous_carry);

	return result;
}

/**
 * Shift operations
 */
inline static void shift_op(std::string& bit_string, INT_TYPE len, signed char padding_bit)
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

bool V_TRUE(const std::string& a)
{
	if(is_dont_care_string(a))
		return "x";

	return	(eval_op(a,"1") == EQUAL) 			? true : false;
}

/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */

std::string V_BITWISE_NOT(const std::string& a)
{
	std::string result(a);
	not_op(result);
	return result;
}

std::string V_ADD(const std::string& a)
{
	std::string result(a);
	return result;
}

std::string V_MINUS(const std::string& a)
{
	return V_MINUS("0", a);
}

std::string V_BITWISE_AND(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_and);
	return result;
}

std::string V_BITWISE_OR(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_or);	
	return result;
}

std::string V_BITWISE_XOR(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_xor);
	return result;
}

std::string V_BITWISE_NAND(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_and);
	not_op(result);
	return result;
}

std::string V_BITWISE_NOR(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_or);
	not_op(result);
	return result;
}

std::string V_BITWISE_XNOR(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_xnor);
	not_op(result);
	return result;
}

std::string V_LOGICAL_NOT(const std::string& a)
{
	std::string result(a);
	inline_redux_op(result, l_or);
	not_op(result);
	return result;
}

/***
 *     __               __          __   __   ___  __       ___    __       
 *    |__) | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    |__) | | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                          
 */

std::string V_BITWISE_AND(const std::string& a,const std::string& b)
{
	return redux_op(a,b,l_and);
}

std::string V_BITWISE_OR(const std::string& a,const std::string& b)
{
	return redux_op(a,b,l_or);
}

std::string V_BITWISE_XOR(const std::string& a,const std::string& b)
{
	return redux_op(a,b,l_xor);
}

std::string V_BITWISE_NAND(const std::string& a,const std::string& b)
{
	return redux_op(a,b,l_nand);
}

std::string V_BITWISE_NOR(const std::string& a,const std::string& b)
{
	return redux_op(a,b,l_nor);
}

std::string V_BITWISE_XNOR(const std::string& a,const std::string& b)
{
	return redux_op(a,b,l_xnor);
}

std::string V_CASE_EQUAL(const std::string& a,const std::string& b)
{
	int std_length = std::max(a.size(), b.size());

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

	shift_op(a, bits_str_to_int(value), '0');
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

	shift_op(a, -1 * bits_str_to_int(value), get_padding_bit(a));
	return a;
}

std::string V_SHIFT_RIGHT(const std::string& a_in, const std::string& b)
{
	if(is_dont_care_string(b))	
		return "x";

	std::string value(b.crbegin(), b.crend());
	std::string a(a_in);

	shift_op(a, -1 * bits_str_to_int(value), '0');
	return a;

}

/**
 * Logical Operations
 */
std::string V_LOGICAL_AND(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return V_BITWISE_AND(V_BITWISE_OR(a),V_BITWISE_OR(b));
}

std::string V_LOGICAL_OR(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return V_BITWISE_OR(V_BITWISE_OR(a),V_BITWISE_OR(b));
}

std::string V_LT(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(eval_op(a,b) == LESS_THAN) 		? "1":"0";
}

std::string V_GT(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(eval_op(a,b) == GREATHER_THAN) 	? "1":"0";
}

std::string V_LE(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(eval_op(a,b) != GREATHER_THAN) 	? "1":"0";
}

std::string V_GE(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(eval_op(a,b) != LESS_THAN) 		? "1":"0";
}

std::string V_EQUAL(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(eval_op(a,b) == EQUAL) 			? "1":"0";
}

std::string V_NOT_EQUAL(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return	(eval_op(a,b) != EQUAL) 			? "1":"0";
}

std::string V_ADD(const std::string& a, const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return sum_op(a, b, '0');
}

std::string V_MINUS(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "x";

	return sum_op(a, V_BITWISE_NOT(b), '1');

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

	std::string result = "0";
	std::string b_copy = b;

	for(int i=0; i< a.size(); i++)
	{
		char bit_a = a[i];
		if(bit_a == '1')
			result = V_ADD(result,b);
		shift_op(b_copy, 1, '0');
	}

	if(invert_result)	
		result = V_MINUS(result);

	return result;
}

std::string V_POWER(const std::string& a,const std::string& b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return "xxxx";
	
	EVAL_RESULT res_a = eval_op(a, 0);
	short val_a = 	(res_a == EQUAL) 			? 	0: 
					(res_a == LESS_THAN) 		? 	((eval_op(a,-1) == LESS_THAN))	?	-2: -1:
					/* GREATHER_THAN */  			((eval_op(a,1) == GREATHER_THAN))	?	2: 1;

	EVAL_RESULT res_b = eval_op(b, 0);
	short val_b = 	(res_b == EQUAL) 			? 	0: 
					(res_b == LESS_THAN) 		? 	-1:
					/* GREATHER_THAN */				1;

	//compute
	if(val_b == 1 && (val_a < -1 || val_a > 1 ))
	{
		std::string result = "1";
		
		std::string tmp_b = b;
		while(eval_op(tmp_b,0) == GREATHER_THAN)
		{
			tmp_b = V_MINUS(tmp_b,"1");
			result = V_MULTIPLY( result, a);
		}
		return result;
	}
	if (val_b == 0 || val_a == 1)	
	{
		return "1000";
	}
	else if(val_b == -1 && val_a == 0)
	{
		return "xxxx";
	}
	else if(val_a == -1)
	{
		if(is_negative(a)) 	// even
			return "0111";
		else				//	odd
			return "1000";
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
	|| eval_op(b,0) == EQUAL)
		return "x";

	
	std::string result = "0";
	//TODO signed division!
	while(eval_op(a, b) == GREATHER_THAN )
	{
		std::string  count = "1";
		std::string  sub_with = b;
		std::string  tmp = b;
		while(eval_op(tmp, a) == LESS_THAN)
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
	|| eval_op(b, 0) == EQUAL)
		return "x";

	//TODO signed division!
	while(eval_op(b, a)  == LESS_THAN)
	{
		std::string  sub_with = b;
		for( std::string  tmp = b; eval_op(tmp, a) == LESS_THAN; shift_op(tmp, 1,'0'))
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
	EVAL_RESULT eval = eval_op(V_LOGICAL_NOT(a),0);

	if(eval == UNKNOWN)		return redux_op(b,c,l_ternary);
	else if(eval == EQUAL)	return b;
	else					return c;
}
