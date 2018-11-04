/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#ifndef RTL_INT_H
#define RTL_INT_H

#include <string>

/**
 * Unary Operator
 */

bool V_TRUE(const std::string& a);

std::string  V_ADD(const std::string& a);
std::string  V_MINUS(const std::string& a);

std::string  V_BITWISE_NOT(const std::string& a);
std::string  V_BITWISE_AND(const std::string& a);
std::string  V_BITWISE_OR(const std::string& a);
std::string  V_BITWISE_XOR(const std::string& a);
std::string  V_BITWISE_NAND(const std::string& a);
std::string  V_BITWISE_NOR(const std::string& a);
std::string  V_BITWISE_XNOR(const std::string& a);

std::string  V_LOGICAL_NOT(const std::string& a);

/**
 * Binary Operator
 */
std::string  V_BITWISE_AND(const std::string& a,const std::string& b);
std::string  V_BITWISE_OR(const std::string& a,const std::string& b);
std::string  V_BITWISE_XOR(const std::string& a,const std::string& b);
std::string  V_BITWISE_NAND(const std::string& a,const std::string& b);
std::string  V_BITWISE_NOR(const std::string& a,const std::string& b);
std::string  V_BITWISE_XNOR(const std::string& a,const std::string& b);

std::string  V_SIGNED_SHIFT_LEFT(const std::string& a, const std::string& b);
std::string  V_SIGNED_SHIFT_RIGHT(const std::string& a, const std::string& b);
std::string  V_SHIFT_LEFT(const std::string& a, const std::string& b);
std::string  V_SHIFT_RIGHT(const std::string& a, const std::string& b);

std::string  V_LOGICAL_AND(const std::string& a,const std::string& b);
std::string  V_LOGICAL_OR(const std::string& a,const std::string& b);

std::string  V_LT(const std::string& a,const std::string& b);
std::string  V_GT(const std::string& a,const std::string& b);
std::string  V_LE(const std::string& a,const std::string& b);
std::string  V_GE(const std::string& a,const std::string& b);
std::string  V_EQUAL(const std::string& a,const std::string& b);
std::string  V_NOT_EQUAL(const std::string& a,const std::string& b);
std::string  V_CASE_EQUAL(const std::string& a,const std::string& b);
std::string  V_CASE_NOT_EQUAL(const std::string& a,const std::string& b);

std::string  V_ADD(const std::string& a,const std::string& b);
std::string  V_MINUS(const std::string& a,const std::string& b);
std::string  V_MULTIPLY(const std::string& a,const std::string& b);
std::string  V_POWER(const std::string& a,const std::string& b);
std::string  V_DIV(const std::string& a,const std::string& b);
std::string  V_MOD(const std::string& a,const std::string& b);

/**
 * Ternary Operator
 */
std::string  V_TERNARY(const std::string& a, const std::string& b, const std::string& c);

#endif //const std::string&_H
