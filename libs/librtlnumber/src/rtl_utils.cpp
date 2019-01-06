/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include "rtl_utils.hpp"
#include <algorithm>
#include <iostream>

// this sets the default bit width

/***
 *     __    ___     __  ___  __          __                     __        ___  __   __  
 *    |__) |  |     /__`  |  |__) | |\ | / _`    |__|  /\  |\ | |  \ |    |__  |__) /__` 
 *    |__) |  |     .__/  |  |  \ | | \| \__>    |  | /~~\ | \| |__/ |___ |___ |  \ .__/ 
 *    
 *  These utils read input string written in litle endian                                                                                   
 */

#define DC_CHR "xXzZ"
#define DEC_CHR "0123456789"
#define HEX_CHR "xZzZ0123456789aAbBcCdDeEfF"
#define BIN_CHR "xXzZ01"
#define OCT_CHR "xXzZ01234567"

bool is_dont_care_string(const std::string& input)
{
    return (input.find(DC_CHR) != std::string::npos);
}
bool is_string_of_radix(const std::string& input, short radix)
{
	switch(radix)
	{
		case 2:		return (!input.empty() && input.find_first_not_of(BIN_CHR) == std::string::npos);
		case 8:	    return (!input.empty() && input.find_first_not_of(DEC_CHR) == std::string::npos);
		case 10:	return (!input.empty() && input.find_first_not_of(DEC_CHR) == std::string::npos);
		case 16:	return (!input.empty() && input.find_first_not_of(HEX_CHR) == std::string::npos);
		default:	return false;
	}
}

char _bad_value(const char test, const char *FUNCT, int LINE)
{	
	std::cout << "INVALID BIT INPUT: (" << std::string(1,test) << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
	return test; 
}

std::string _bad_value(const std::string& test, const char *FUNCT, int LINE)
{	
	std::cout << "INVALID BIT INPUT: (" << test << ")@" << FUNCT << "::" << std::to_string(LINE) << std::endl;	
	return test; 
}

void _assert_not_dc_string(const std::string& input, const char *FUNCT, int LINE)
{
	if (is_dont_care_string(input))
	{
		std::cout << "Invalid Number contains dont care values. number: " << input << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

void _assert_string_of_radix(const std::string& input, short radix, const char *FUNCT, int LINE)
{
	if(!is_string_of_radix(input, radix))
	{
		std::cout << "Invalid bitstring of radix input " << std::to_string(radix) << " number: " << input << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

void _assert_bits_len_within_limit(const std::string& input, const char *FUNCT, int LINE)
{
    if( input.size() >= DEFAULT_BIT_WIDTH )
	{
		std::cout << "Invalid Number. Too large to be converted. number: " << input << " upper limit: " << std::to_string(DEFAULT_BIT_WIDTH) << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

long long _str_to_int(const std::string& input, short radix, const char *FUNCT, int LINE)
{
    _assert_not_dc_string(input, FUNCT, LINE);
	_assert_string_of_radix(input, radix, FUNCT, LINE);
    _assert_bits_len_within_limit(input,FUNCT, LINE);
	
	return std::strtoll(input.c_str(),NULL,radix);
}

char _bits_to_hex(std::string revers, const char *FUNCT, int LINE)
{
    if     ( revers == "0000" )              return '0';
    else if( revers == "0001" )              return '1';
    else if( revers == "0010" )              return '2';
    else if( revers == "0011" )              return '3';
    else if( revers == "0100" )              return '4';
    else if( revers == "0101" )              return '5';
    else if( revers == "0110" )              return '6';
    else if( revers == "0111" )              return '7';
    else if( revers == "1000" )              return '8';
    else if( revers == "1001" )              return '9';
    else if( revers == "1010" )              return 'a';
    else if( revers == "1011" )              return 'b';
    else if( revers == "1100" )              return 'c';
    else if( revers == "1101" )              return 'd';
    else if( revers == "1110" )              return 'e';
    else if( revers == "1111" )              return 'f';
    else if( revers == "xxxx" )              return 'x';
    else if( revers == "zzzz" )              return 'z';
    else{    _bad_value(revers, FUNCT, LINE);return 'x';}
}

#define radix_digit_to_bits_str(num,radix) _radix_digit_to_bits_str(num,radix,__func__, __LINE__)
inline static std::string _radix_digit_to_bits_str(const char digit, short radix,  const char *FUNCT, int LINE)
{
    switch(radix)
    {
        case 2:
        {
            switch(std::tolower(digit))
            {
                case '0': return "0";
                case '1': return "1";
                case 'x': return "x";
                case 'z': return "z";
                default:  
                    _bad_value(digit, FUNCT, LINE);
                    break;
            }
            break;
        }
        case 8:
        {
            switch(std::tolower(digit))
            {
                case '0': return "000";
                case '1': return "001";
                case '2': return "010";
                case '3': return "011";
                case '4': return "100";
                case '5': return "101";
                case '6': return "110";
                case '7': return "111";
                case 'x': return "xxx";
                case 'z': return "zzz";
                default:  
                    _bad_value(digit, FUNCT, LINE);
                    break;
            }
            break;
        }
        case 16:
        {
            switch(std::tolower(digit))
            {
                case '0': return "0000";
                case '1': return "0001";
                case '2': return "0010";
                case '3': return "0011";
                case '4': return "0100";
                case '5': return "0101";
                case '6': return "0110";
                case '7': return "0111";
                case '8': return "1000";
                case '9': return "1001";
                case 'a': return "1010";
                case 'b': return "1011";
                case 'c': return "1100";
                case 'd': return "1101";
                case 'e': return "1110";
                case 'f': return "1111";
                case 'x': return "xxxx";
                case 'z': return "zzzz";
                default:  
                    _bad_value(digit, FUNCT, LINE);
                    break;
            }
            break;
        }
        default:
        {
            std::cout << "Invalid base " << std::to_string(radix) << " for digit: " << std::string(1,digit) << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
            break;
        }
    }
    std::abort();
    return "";
}

#define radix_digit_to_bits(num,radix) _radix_digit_to_bits(num,radix,__func__, __LINE__)
inline static std::string _radix_digit_to_bits(const char digit, short radix,  const char *FUNCT, int LINE)
{
    std::string result = _radix_digit_to_bits_str(digit, radix, FUNCT, LINE);
    return std::string(result.crbegin(), result.crend());
}

/**********************
 * convert from and to internal representation bitstring
 */
std::string string_of_radix_to_bitstring(std::string orig_string, short radix)
{
	
	std::string result = "";	
	assert_string_of_radix(orig_string,radix);
	while(!orig_string.empty())
	{
		switch(radix)
		{
			case 10:
			{
				std::string new_number = "";

				char rem_digit = '0';
				for(char& current_digit : orig_string)
				{
					short new_pair = ((rem_digit - '0')*10) + (current_digit-'0');
					new_number.push_back((new_pair/2) + '0');
                    rem_digit =         ((new_pair%2) + '0');
				}

                result.push_back(rem_digit);
                if(new_number == "0")
                    orig_string = "";
                else
                    orig_string = new_number;

				break;
			}
			default:
			{
                result += radix_digit_to_bits(orig_string.back(), radix);
                orig_string.pop_back();
                break;
			}
		}
	}
	return result;
}
