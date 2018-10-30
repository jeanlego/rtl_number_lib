#ifndef RTL_UTILS_H
#define RTL_UTILS_H

#include <inttypes.h>
#include <string>

#define DEFAULT_BIT_WIDTH (sizeof(uint64_t)*8)

/**
 * convert between internal bitstring to verilog string and back (to conversion is LSB to MSB)
 */
std::string verilog_string_to_bitstring(std::string orig_string);
std::string bitstring_to_verilog_string(std::string origstring);

std::string string_of_radix_to_bitstring(std::string orig_string, std::size_t radix);

bool is_dont_care_string(const std::string& input);
bool is_string_of_radix(const std::string& input, uint8_t radix);

#define bad_value(test) _bad_value(test,  __func__, __LINE__)

char _bad_value(const char test, const char *FUNCT, int LINE);

#define assert_bits_len_within_limit(input) _assert_bits_len_within_limit(input, radix, __func__,__LINE__)
#define assert_not_dc_string(input) _assert_not_dc_string(input, __func__,__LINE__)
#define assert_string_of_radix(input, radix) _assert_string_of_radix(input, radix, __func__,__LINE__)

void _assert_not_dc_string(const std::string& input, const char *FUNCT, int LINE);
void _assert_string_of_radix(const std::string& input, uint8_t radix, const char *FUNCT, int LINE);
void _assert_bits_len_within_limit(const std::string& input, const char *FUNCT, int LINE);

char get_msb(const std::string& input);
bool is_negative(const std::string& input);
char get_padding_bit(const std::string& input);
void resize(std::string& bits, std::size_t len);

#define v_strtoul(num,radix) _v_strtoul(num,radix,__func__, __LINE__)
int64_t _v_strtoul(const std::string& input, uint8_t radix, const char *FUNCT, int LINE);

#endif