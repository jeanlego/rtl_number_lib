/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#ifndef RTL_INT_H
#define RTL_INT_H

#include <vector>
#include <string>

typedef std::vector<std::string> RTL_INT;

/**
 * convert between internal representation to string
 */
RTL_INT standardize_input(std::string input_number);

std::string v_bin_string(RTL_INT internal_binary_number);

/**
 * return an integer after evaluation
 */
typedef enum
{
    LESS_THAN,
    GREATHER_THAN,
    EQUAL,
    UNKNOWN
}EVAL_RESULT;
EVAL_RESULT return_int_eval(RTL_INT a, RTL_INT b);
EVAL_RESULT return_int_eval(long a, RTL_INT b);
EVAL_RESULT return_int_eval(RTL_INT a, long b);

/**
 * Unary Operator
 */
RTL_INT V_NEG(RTL_INT a);
RTL_INT V_PLUS_PLUS(RTL_INT a);
RTL_INT  V_MINUS_MINUS(RTL_INT a);
RTL_INT V_ADD(RTL_INT a);
RTL_INT V_MINUS(RTL_INT a);

RTL_INT V_AND(RTL_INT a);
RTL_INT V_OR(RTL_INT a);
RTL_INT V_XOR(RTL_INT a);
RTL_INT V_NAND(RTL_INT a);
RTL_INT V_NOR(RTL_INT a);
RTL_INT V_XNOR(RTL_INT a);

RTL_INT V_LOGICAL_NOT(RTL_INT a);

/**
 * Binary Operator
 */
RTL_INT V_AND(RTL_INT a,RTL_INT b);
RTL_INT V_OR(RTL_INT a,RTL_INT b);
RTL_INT V_XOR(RTL_INT a,RTL_INT b);
RTL_INT V_NAND(RTL_INT a,RTL_INT b);
RTL_INT V_NOR(RTL_INT a,RTL_INT b);
RTL_INT V_XNOR(RTL_INT a,RTL_INT b);

RTL_INT V_CASE_EQUAL(RTL_INT a,RTL_INT b);
RTL_INT V_CASE_NOT_EQUAL(RTL_INT a,RTL_INT b);

RTL_INT V_SIGNED_SHIFT_LEFT(RTL_INT a, std::size_t b);
RTL_INT V_SIGNED_SHIFT_LEFT(RTL_INT a, RTL_INT b);
RTL_INT V_SIGNED_SHIFT_RIGHT(RTL_INT a, std::size_t b);
RTL_INT V_SIGNED_SHIFT_RIGHT(RTL_INT a, RTL_INT b);
RTL_INT V_SHIFT_LEFT(RTL_INT a, std::size_t b);
RTL_INT V_SHIFT_LEFT(RTL_INT a, RTL_INT b);
RTL_INT V_SHIFT_RIGHT(RTL_INT a, std::size_t b);
RTL_INT V_SHIFT_RIGHT(RTL_INT a, RTL_INT b);


RTL_INT V_LOGICAL_AND(RTL_INT a,RTL_INT b);
RTL_INT V_LOGICAL_OR(RTL_INT a,RTL_INT b);

RTL_INT V_LT(RTL_INT a,RTL_INT b);
RTL_INT V_GT(RTL_INT a,RTL_INT b);
RTL_INT V_LE(RTL_INT a,RTL_INT b);
RTL_INT V_GE(RTL_INT a,RTL_INT b);
RTL_INT V_EQUAL(RTL_INT a,RTL_INT b);
RTL_INT V_NOT_EQUAL(RTL_INT a,RTL_INT b);

RTL_INT V_ADD(RTL_INT a,RTL_INT b);
RTL_INT V_MINUS(RTL_INT a,RTL_INT b);
RTL_INT V_MULTIPLY(RTL_INT a,RTL_INT b);
RTL_INT V_POWER(RTL_INT a,RTL_INT b);
RTL_INT V_DIV(RTL_INT a,RTL_INT b);
RTL_INT V_MOD(RTL_INT a,RTL_INT b);

/**
 * Ternary Operator
 */
RTL_INT V_TERNARY(RTL_INT a, RTL_INT b, RTL_INT c);

#endif //RTL_INT_H
