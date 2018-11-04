/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#ifndef RTL_UTILS_H
#define RTL_UTILS_H

#include <string>

typedef long long INT_TYPE;
#define DEFAULT_BIT_WIDTH (sizeof(INT_TYPE)*8)

#define bits_str_to_int(num) _str_to_int(num,2,__func__, __LINE__)
#define dec_str_to_int(num) _str_to_int(num,10,__func__, __LINE__)

INT_TYPE _str_to_int(const std::string& input, short radix, const char *FUNCT, int LINE);

/**
 * convert between internal bitstring to verilog string and back (to conversion is LSB to MSB)
 */
std::string verilog_string_to_bitstring(std::string orig_string);
std::string bitstring_to_verilog_string(std::string origstring);

std::string string_of_radix_to_bitstring(std::string orig_string, short radix);

bool is_dont_care_string(const std::string& input);
bool is_string_of_radix(const std::string& input, short radix);

#define bad_value(test) _bad_value(test,  __func__, __LINE__)
char _bad_value(const char test, const char *FUNCT, int LINE);

#define assert_bits_len_within_limit(input) _assert_bits_len_within_limit(input, radix, __func__,__LINE__)
#define assert_not_dc_string(input) _assert_not_dc_string(input, __func__,__LINE__)
#define assert_string_of_radix(input, radix) _assert_string_of_radix(input, radix, __func__,__LINE__)

void _assert_not_dc_string(const std::string& input, const char *FUNCT, int LINE);
void _assert_string_of_radix(const std::string& input, short radix, const char *FUNCT, int LINE);
void _assert_bits_len_within_limit(const std::string& input, const char *FUNCT, int LINE);

#endif