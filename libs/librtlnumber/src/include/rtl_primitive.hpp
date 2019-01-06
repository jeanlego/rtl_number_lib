/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#ifndef RTL_PRIMITIVE_H
#define RTL_PRIMITIVE_H

#include <array>

#include "rtl_utils.hpp"
#include "internal_bits.hpp"

using namespace BitSpace;

/***                                                              
 * these are taken from the raw verilog truth tables so that the evaluation are correct.
 * only use this to evaluate any expression for the number_t binary digits.
 * reference: http://staff.ustc.edu.cn/~songch/download/IEEE.1364-2005.pdf
 * 
 *******************************************************/

static const bit_value_t l_buf[4] = {
	/*	 0   1   x   z  <- a*/
		_0,_1,_x,_x
};

static const bit_value_t l_not[4] = {
	/*   0   1   x   z 	<- a */
		_1,_0,_x,_x
};

static const bit_value_t l_and[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{_0,_0,_0,_0},	
	/* 1 */	{_0,_1,_x,_x},	
	/* x */	{_0,_x,_x,_x},	
	/* z */	{_0,_x,_x,_x}
};

static const bit_value_t l_or[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{_0,_1,_x,_x},	
	/* 1 */	{_1,_1,_1,_1},	
	/* x */	{_x,_1,_x,_x},	
	/* z */	{_x,_1,_x,_x}
};

static const bit_value_t l_xor[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{_0,_1,_x,_x},	
	/* 1 */	{_1,_0,_x,_x},	
	/* x */	{_x,_x,_x,_x},	
	/* z */	{_x,_x,_x,_x}
};

static const bit_value_t l_nand[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{_1,_1,_1,_1},	
	/* 1 */	{_1,_0,_x,_x},	
	/* x */	{_1,_x,_x,_x},	
	/* z */	{_1,_x,_x,_x}
};

static const bit_value_t l_nor[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{_1,_0,_x,_x},	
	/* 1 */	{_0,_0,_0,_0},	
	/* x */	{_x,_0,_x,_x},	
	/* z */	{_x,_0,_x,_x}
};

static const bit_value_t l_xnor[4][4] = {
	/* a  /	 0   1   x   z 	<-b */	
	/* 0 */	{_1,_0,_x,_x},	
	/* 1 */	{_0,_1,_x,_x},	
	/* x */	{_x,_x,_x,_x},	
	/* z */	{_x,_x,_x,_x}
};

static const bit_value_t l_notif1[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_z,_1,_x,_x},
	/* 1 */	{_z,_0,_x,_x},
	/* x */	{_z,_x,_x,_x},
	/* z */	{_z,_x,_x,_x}
};

static const bit_value_t l_notif0[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_1,_z,_x,_x},
	/* 1 */	{_0,_z,_x,_x},
	/* x */	{_x,_z,_x,_x},
	/* z */	{_x,_z,_x,_x}
};

static const bit_value_t l_bufif1[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_z,_0,_x,_x},
	/* 1 */	{_z,_1,_x,_x},
	/* x */	{_z,_x,_x,_x},
	/* z */	{_z,_x,_x,_x}
};

static const bit_value_t l_bufif0[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_0,_z,_x,_x},
	/* 1 */	{_1,_z,_x,_x},
	/* x */	{_x,_z,_x,_x},
	/* z */	{_x,_z,_x,_x}
};

static const bit_value_t l_rpmos[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_0,_z,_x,_x},
	/* 1 */	{_1,_z,_x,_x},
	/* x */	{_x,_z,_x,_x},
	/* z */	{_z,_z,_z,_z}
};

static const bit_value_t l_rnmos[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_z,_0,_x,_x},
	/* 1 */	{_z,_1,_x,_x},
	/* x */	{_z,_x,_x,_x},
	/* z */	{_z,_z,_z,_z}
};

static const bit_value_t l_nmos[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_z,_0,_x,_x},
	/* 1 */	{_z,_1,_x,_x},
	/* x */	{_z,_x,_x,_x},
	/* z */	{_z,_z,_z,_z}
};

// see table 5-21 p:54 IEEE 1364-2005
static const bit_value_t l_ternary[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_0,_x,_x,_x},
	/* 1 */	{_x,_1,_x,_x},
	/* x */	{_x,_x,_x,_x},
	/* z */	{_x,_x,_x,_x}
};

static const bit_value_t l_unk[4][4] = {
	/* in /	 0   1   x   z 	<-control */	
	/* 0 */	{_x,_x,_x,_x},
	/* 1 */	{_x,_x,_x,_x},
	/* x */	{_x,_x,_x,_x},
	/* z */	{_x,_x,_x,_x}
};

#define unroll_1d(lut) { lut[0], lut[1], lut[2], lut[3] }
#define unroll_2d(lut) { unroll_1d(lut[0]), unroll_1d(lut[1]), unroll_1d(lut[2]), unroll_1d(lut[3]) }

static const bit_value_t l_sum[4][4][4] = {
	unroll_2d(l_xor),
	unroll_2d(l_xnor),
	unroll_2d(l_unk),
	unroll_2d(l_unk)
};

static const bit_value_t l_carry[4][4][4] = {
	unroll_2d(l_and),
	unroll_2d(l_or),
	unroll_2d(l_ternary),
	unroll_2d(l_ternary)
};

inline bit_value_t v_op( const bit_value_t lut[4], std::array<bit_value_t,1> args) 
{
	return lut[args[0]];
}

inline bit_value_t v_op(const bit_value_t lut[4][4], std::array<bit_value_t,2> args) 
{
	return v_op(lut[args[0]], {args[1]});
}

inline bit_value_t v_op(const bit_value_t lut[4][4][4], std::array<bit_value_t,3> args) 
{
	return v_op(lut[args[0]], {args[1], args[2]});
}

#endif
