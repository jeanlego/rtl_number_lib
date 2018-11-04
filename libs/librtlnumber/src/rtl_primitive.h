#ifndef RTL_PRIMITIVE_H
#define RTL_PRIMITIVE_H

#include <array>

#include "rtl_utils.h"

/***                                                              
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

inline short _chr2id(const char in, const char *FUNCT, int LINE)
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
inline char _v_op( const char lut[4], std::array<char,1> args, const char *FUNCT, int LINE) 
{
	return lut[_chr2id(args[0], FUNCT, LINE)];
}

inline char _v_op(const char lut[4][4], std::array<char,2> args, const char *FUNCT, int LINE) 
{
	return _v_op(lut[_chr2id(args[0], FUNCT, LINE)], {args[1]}, FUNCT, LINE);
}

inline char _v_op(const char lut[4][4][4], std::array<char,3> args, const char *FUNCT, int LINE) 
{
	return _v_op(lut[_chr2id(args[0], FUNCT, LINE)], {args[1], args[2]}, FUNCT, LINE);
}

#endif