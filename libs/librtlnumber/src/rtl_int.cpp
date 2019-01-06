/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include <string>

#include "rtl_int.hpp"
#include "rtl_utils.hpp"
#include "rtl_primitive.hpp"
#include "v_number.hpp"

using namespace BitSpace;

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

inline static EVAL_RESULT eval_op(VNumber a_in, VNumber b_in)
{
	bool neg_a = a_in.is_negative();
	bool neg_b = (b_in.is_negative());

	VNumber a();
	if(neg_a)	a = V_MINUS(a_in);
	else		a = V_ADD(a_in);

	VNumber b();
	if(neg_b)	b = V_MINUS(b_in);
	else		b = V_ADD(b_in);

	bool invert_result = ((!neg_a && neg_b) || (neg_a && !neg_b));

	int std_length = std::max(a.size(), b.size());
	const bit_value_t pad_a = a.get_padding_bit();
	const bit_value_t pad_b = b.get_padding_bit();

	VNumber result = "";

	for(int i=std_length-1; i >= 0; i--)
	{
		bit_value_t bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		bit_value_t bit_b = pad_b;
		if(i < b.size())
			bit_b = b[i];

		if (bit_a == _1 && bit_b == _0)	
			return (invert_result)? LESS_THAN: GREATHER_THAN;

		else if	(bit_a == _0 && bit_b == _1)	
			return (invert_result)? GREATHER_THAN: LESS_THAN;
			
		else if	(bit_a == _x || bit_b == _x)	
			return UNKNOWN;
	}
	return EQUAL;
}

inline static EVAL_RESULT eval_op(long a, VNumber b)
{
	VNumber bits_value = VNumber(std::to_string(a));
	return eval_op(bits_value,b);
}

inline static EVAL_RESULT eval_op(VNumber a,long b)
{
	VNumber bits_value = VNumber(std::to_string(b));
	return eval_op(a, bits_value);
}

/**
 * Not operations
 */
inline static void not_op(VNumber& a)
{
	for(int i=0; i <a.size(); i++)
		a[i] = (v_op(l_not,a[i]));
}

/**
 * Unary Reduction operations
 */
inline static void inline_redux_op(VNumber& a, const bit_value_t lut[4][4])
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
inline static VNumber redux_op(VNumber a, VNumber b, const bit_value_t lut[4][4])
{
	int std_length = std::max(a.size(), b.size());
	const bit_value_t pad_a = a.get_padding_bit();
	const bit_value_t pad_b = b.get_padding_bit();

	VNumber result = "";

	for(int i=0; i < std_length; i++)
	{
		bit_value_t bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		bit_value_t bit_b = pad_b;
		if(i < b.size())
			bit_b = b[i];

		result.push_back(v_op(lut, bit_a, bit_b));
	}

	return result;
}

/**
 * Addition operations
 */
inline static VNumber sum_op(VNumber a, VNumber b, const bit_value_t initial_carry)
{
	int std_length = std::max(a.size(), b.size());
	const bit_value_t pad_a = a.get_padding_bit();
	const bit_value_t pad_b = b.get_padding_bit();

	bit_value_t previous_carry = initial_carry;
	VNumber result = "";

	for(int i=0; i < std_length; i++)
	{
		bit_value_t bit_a = pad_a;
		if(i < a.size())
			bit_a = a[i];

		bit_value_t bit_b = pad_b;
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
 * TODO change this! doing it inplace wil cause some issue since we cant resize the internal bits, fix one or the other
 */
inline static void shift_op(VNumber& bit_string, long long len, signed bit_value_t padding_bit)
{
	VNumber pad(std::abs(len),padding_bit);
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

bool V_TRUE(VNumber a)
{
	if(is_dont_care_string(a))
		return false;

	return	(eval_op(a,1) == EQUAL) 			? true : false;
}

/***
 *                    __          __   __   ___  __       ___    __       
 *    |  | |\ |  /\  |__) \ /    /  \ |__) |__  |__)  /\   |  | /  \ |\ | 
 *    \__/ | \| /~~\ |  \  |     \__/ |    |___ |  \ /~~\  |  | \__/ | \| 
 *                                                                        
 */

VNumber V_BITWISE_NOT(VNumber a)
{
	VNumber result(a);
	not_op(result);
	return result;
}

VNumber V_ADD(VNumber a)
{
	VNumber result(a);
	return result;
}

VNumber V_MINUS(VNumber a)
{
	return V_MINUS(VNumber("0"), a);
}

VNumber V_BITWISE_AND(VNumber a)
{
	VNumber result(a);
	inline_redux_op(result, l_and);
	return result;
}

VNumber V_BITWISE_OR(VNumber a)
{
	VNumber result(a);
	inline_redux_op(result, l_or);	
	return result;
}

VNumber V_BITWISE_XOR(VNumber a)
{
	VNumber result(a);
	inline_redux_op(result, l_xor);
	return result;
}

VNumber V_BITWISE_NAND(VNumber a)
{
	VNumber result(a);
	inline_redux_op(result, l_and);
	not_op(result);
	return result;
}

VNumber V_BITWISE_NOR(VNumber a)
{
	VNumber result(a);
	inline_redux_op(result, l_or);
	not_op(result);
	return result;
}

VNumber V_BITWISE_XNOR(VNumber a)
{
	VNumber result(a);
	inline_redux_op(result, l_xnor);
	not_op(result);
	return result;
}

VNumber V_LOGICAL_NOT(VNumber a)
{
	VNumber result(a);
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

VNumber V_BITWISE_AND(VNumber a,VNumber b)
{
	return redux_op(a,b,l_and);
}

VNumber V_BITWISE_OR(VNumber a,VNumber b)
{
	return redux_op(a,b,l_or);
}

VNumber V_BITWISE_XOR(VNumber a,VNumber b)
{
	return redux_op(a,b,l_xor);
}

VNumber V_BITWISE_NAND(VNumber a,VNumber b)
{
	return redux_op(a,b,l_nand);
}

VNumber V_BITWISE_NOR(VNumber a,VNumber b)
{
	return redux_op(a,b,l_nor);
}

VNumber V_BITWISE_XNOR(VNumber a,VNumber b)
{
	return redux_op(a,b,l_xnor);
}

VNumber V_CASE_EQUAL(VNumber a,VNumber b)
{
	int std_length = std::max(a.size(), b.size());
	bit_value_t last_a = a.get_padding_bit();
	bit_value_t last_b = b.get_padding_bit(); 

	for (int i=0; i<std_length; i++)
	{
		bit_value_t cur_a = last_a;
		if(i < a.size())
			cur_a = a[i];
		
		bit_value_t cur_b = last_b;
		if(i < b.size())
			cur_b = b[i];

		if(cur_a != cur_b)	return VNumber("0");
	}
	
	return VNumber("1");
}
VNumber V_CASE_NOT_EQUAL(VNumber a,VNumber b)
{
	return V_LOGICAL_NOT(V_CASE_EQUAL(a,b));
}

VNumber V_SIGNED_SHIFT_LEFT(VNumber a_in, VNumber b)
{
	if(is_dont_care_string(b))	
		return ;
	
	//TODO this won't work
	VNumber value(b.crbegin(), b.crend());
	VNumber a(a_in);

	shift_op(a, bits_str_to_int(value), _0);
	return a;
}

VNumber V_SHIFT_LEFT(VNumber a, VNumber b)
{
	return V_SIGNED_SHIFT_LEFT(a,b);
}

VNumber V_SIGNED_SHIFT_RIGHT(VNumber a_in, VNumber b)
{
	if(is_dont_care_string(b))	
		return VNumber("x");
	
	VNumber a(a_in);

	shift_op(a, -1 * b.value(), a_in.get_padding_bit());
	return a;
}

VNumber V_SHIFT_RIGHT(VNumber a_in, VNumber b)
{
	if(is_dont_care_string(b))	
		return VNumber("x");
	
	VNumber a(a_in);

	shift_op(a, -1 * b.value(), _0);
	return a;
}

/**
 * Logical Operations
 */
VNumber V_LOGICAL_AND(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return V_BITWISE_AND(V_BITWISE_OR(a),V_BITWISE_OR(b));
}

VNumber V_LOGICAL_OR(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return V_BITWISE_OR(V_BITWISE_OR(a),V_BITWISE_OR(b));
}

VNumber V_LT(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return	(eval_op(a,b) == LESS_THAN) 		? "1":"0";
}

VNumber V_GT(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return	(eval_op(a,b) == GREATHER_THAN) 	? "1":"0";
}

VNumber V_LE(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return	(eval_op(a,b) != GREATHER_THAN) 	? "1":"0";
}

VNumber V_GE(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return	(eval_op(a,b) != LESS_THAN) 		? "1":"0";
}

VNumber V_EQUAL(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return	(eval_op(a,b) == EQUAL) 			? "1":"0";
}

VNumber V_NOT_EQUAL(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return	(eval_op(a,b) != EQUAL) 			? "1":"0";
}

VNumber V_ADD(VNumber a, VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return sum_op(a, b, _0);
}

VNumber V_MINUS(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("x");

	return sum_op(a, V_BITWISE_NOT(b), _1);

}

VNumber V_MULTIPLY(VNumber a_in, VNumber b_in)
{
	if(is_dont_care_string(a_in) || is_dont_care_string(b_in))
		return VNumber("x");

	VNumber a;
	VNumber b;

	bool neg_a = a_in.is_negative();
	bool neg_b = b_in.is_negative();
	
	if(neg_a)	
		a = V_MINUS(a_in);
	else
		a = a_in;

	if(neg_b)	
		b = V_MINUS(b_in);
	else
		b = b_in;
		
	bool invert_result = ((!neg_a && neg_b) || (neg_a && !neg_b));

	//TODO this won't work
	VNumber result("0");
	VNumber b_copy = b;

	for(int i=0; i< a.size(); i++)
	{
		bit_value_t bit_a = a[i];
		if(bit_a == _1)
			result = V_ADD(result,b);
		shift_op(b_copy, 1, _0);
	}

	if(invert_result)	
		result = V_MINUS(result);

	return result;
}

VNumber V_POWER(VNumber a,VNumber b)
{
	if(is_dont_care_string(a) || is_dont_care_string(b))
		return VNumber("xxxx");
	
	EVAL_RESULT res_a = eval_op(a, 0);
	short val_a = 	(res_a == EQUAL) 			? 	0: 
					(res_a == LESS_THAN) 		? 	((eval_op(a,-1) == LESS_THAN))	?	-2: -1:
					/* GREATHER_THAN */  			((eval_op(a,1) == GREATHER_THAN))	?	2: 1;

	EVAL_RESULT res_b = eval_op(b, 0);
	short val_b = 	(res_b == EQUAL) 			? 	0: 
					(res_b == LESS_THAN) 		? 	-1:
					/* GREATHER_THAN */				1;

	/* TODO: Pass in v_number */v_number v_number_a(a);
	// /* TODO: Pass in v_number */v_number v_number_b(b);

	//TODO this won't work

	//compute
	if(val_b == 1 && (val_a < -1 || val_a > 1 ))
	{
		VNumber result("1");
		
		VNumber tmp_b = b;
		while(eval_op(tmp_b,0) == GREATHER_THAN)
		{
			tmp_b = V_MINUS(tmp_b,VNumber("1"));
			result = V_MULTIPLY( result, a);
		}
		return result;
	}
	if (val_b == 0 || val_a == 1)	
	{
		return VNumber("1000");
	}
	else if(val_b == -1 && val_a == 0)
	{
		return VNumber("xxxx");
	}
	else if(val_a == -1)
	{
		if(v_number_a.is_negative()) 	// even
			return VNumber("0111");
		else				//	odd
			return VNumber("1000");
	}
	else	
	{
		return VNumber("0000");
	}
}

//TODO this won't work
/////////////////////////////
VNumber V_DIV(VNumber a,VNumber b)
{
	VNumber a(a_in);
	if(is_dont_care_string(a) || is_dont_care_string(b)
	|| eval_op(b,0) == EQUAL)
		return VNumber("x");

	
	VNumber result("0");
	//TODO signed division!
	while(eval_op(a, b) == GREATHER_THAN )
	{
		VNumber  count("1");
		VNumber  sub_with = b;
		VNumber  tmp = b;
		while(eval_op(tmp, a) == LESS_THAN)
		{
			sub_with = tmp;
			shift_op(count, 1,_0);
			shift_op(tmp, 1,_0);
		}
		a = V_MINUS(a, sub_with);
		result = V_ADD(result, count);
	}
	return result;
}

VNumber V_MOD(VNumber a,VNumber b)
{
	VNumber a(a_in);

	if(is_dont_care_string(a) || is_dont_care_string(b)
	|| eval_op(b, 0) == EQUAL)
		return VNumber("x");

	//TODO signed division!
	while(eval_op(b, a)  == LESS_THAN)
	{
		VNumber  sub_with = b;
		for( VNumber  tmp = b; eval_op(tmp, a) == LESS_THAN; shift_op(tmp, 1,_0))
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
VNumber V_TERNARY(VNumber a, VNumber b, VNumber c)
{
	/*	if a evaluates properly	*/
	EVAL_RESULT eval = eval_op(V_LOGICAL_NOT(a),0);

	if(eval == UNKNOWN)		return redux_op(b,c,l_ternary);
	else if(eval == EQUAL)	return b;
	else					return c;
}
