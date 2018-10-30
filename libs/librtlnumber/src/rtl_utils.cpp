#include "rtl_utils.h"
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
bool is_string_of_radix(const std::string& input, uint8_t radix)
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

#define assert_not_dc_string(input) _assert_not_dc_string(input, __func__,__LINE__)
void _assert_not_dc_string(const std::string& input, const char *FUNCT, int LINE)
{
	if (is_dont_care_string(input))
	{
		std::cout << "Invalid Number contains dont care values. number: " << input << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

#define assert_string_of_radix(input, radix) _assert_string_of_radix(input, radix, __func__,__LINE__)
void _assert_string_of_radix(const std::string& input, uint8_t radix, const char *FUNCT, int LINE)
{
	if(!is_string_of_radix(input, radix))
	{
		std::cout << "Invalid bitstring of radix input " << std::to_string(radix) << " number: " << input << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

#define assert_bits_len_within_limit(input) _assert_bits_len_within_limit(input, radix, __func__,__LINE__)
void _assert_bits_len_within_limit(const std::string& input, const char *FUNCT, int LINE)
{
    if( input.size() >= DEFAULT_BIT_WIDTH )
	{
		std::cout << "Invalid Number. Too large to be converted. number: " << input << " upper limit: " << std::to_string(DEFAULT_BIT_WIDTH) << " @" << FUNCT << "::" << std::to_string(LINE) << std::endl;
		std::abort();
	}
}

char get_msb(const std::string& input)
{
    if(!input.empty())
        return *input.end();
    else
        return '0';
}

bool is_negative(const std::string& input)
{
	return (get_msb(input) == '1');
}

char get_padding_bit(const std::string& input)
{
	return is_negative(input)? '1':'0';
}

void resize(std::string& bits, std::size_t len)
{

    bool let_me_go = false;
	while(bits.size() > 1 && !let_me_go)
	{
        std::size_t current_bit_width = bits.length();
        if(len <= 0)
        {
            if(*(bits.end()) == *(bits.end()-1))
                bits.pop_back();
            else
                let_me_go = true;
        }
        else if(len > current_bit_width)
        {
            bits.push_back(get_padding_bit(bits));
        }
        else if(len < current_bit_width)
        {
            bits.pop_back();
        }
        else if(len == current_bit_width)
        {
            let_me_go = true;
        }
	}
}

int64_t _v_strtoul(const std::string& input, uint8_t radix, const char *FUNCT, int LINE)
{
    _assert_not_dc_string(input, FUNCT, LINE);
	_assert_string_of_radix(input, radix, FUNCT, LINE);
    _assert_bits_len_within_limit(input,FUNCT, LINE);
	
	return std::strtoul(input.c_str(),NULL,radix);
}

#define radix_digit_to_bits_str(num,radix) _radix_digit_to_bits_str(num,radix,__func__, __LINE__)
inline static std::string _radix_digit_to_bits_str(const char digit, uint8_t radix,  const char *FUNCT, int LINE)
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
inline static std::string _radix_digit_to_bits(const char digit, uint8_t radix,  const char *FUNCT, int LINE)
{
    std::string result = _radix_digit_to_bits_str(digit, radix, FUNCT, LINE);
    return std::string(result.crbegin(), result.crend());
}


/**********************
 * convert from and to internal representation bitstring
 */
std::string string_of_radix_to_bitstring(std::string orig_string, std::size_t radix)
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
                std::string big_endian_str = radix_digit_to_bits(orig_string.back(), radix);
                result += radix_digit_to_bits(orig_string.back(), radix);
                orig_string.pop_back();
                break;
			}
		}
	}
	return result;
}

std::string verilog_string_to_bitstring(std::string input_number)
{	
	//remove underscores
	input_number.erase(std::remove(input_number.begin(), input_number.end(), '_'), input_number.end());

	std::size_t loc = input_number.find("\'");
	if(loc == std::string::npos)
	{
		input_number.insert(0, "\'d");
		loc = 0;
	}

	std::size_t len = 0;
	if(loc != 0)
		len = v_strtoul(input_number.substr(0,loc),10);

	bool sign = false;
	if(std::tolower(input_number[loc+1]) == 's')
		sign = true;

    char base = std::tolower(input_number[loc+1+sign]);
    uint8_t radix = (base == 'b') ? 2   :
                    (base == 'o') ? 8   :
                    (base == 'd') ? 10  :
                    (base == 'h') ? 16  :
                                    -1  ;

	std::string result = string_of_radix_to_bitstring(input_number.substr(loc+2+sign), radix);
    //truncate the excess bits (unecessary padding or outside of defined length)

    // pad an extra zero to assure non negative number
    if(!sign)
        result.push_back('0');

    resize(result, len);

    //increase length by 3 this allows us to store sign properly
    if(!sign)
        result.push_back('0');
    else
        result.push_back(get_msb(result));

    return result;
}

// convert lsb_msb bitstring to verilog
std::string bitstring_to_verilog_string(std::string origstring)
{
    std::string out(origstring.crbegin(), origstring.crend());

	return std::string(
        std::to_string(out.size()) 
        + "\'sb" 
        + out
    );
}
