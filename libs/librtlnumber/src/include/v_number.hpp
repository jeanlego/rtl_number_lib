/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#ifndef VNUMBER_HPP
#define VNUMBER_HPP

#include <algorithm>
#include <string>
#include "rtl_utils.hpp"
#include "verilog_bits.hpp"

using namespace BitSpace;
class VNumber 
{
private:
    bool sign;
    size_t size;
    VerilogBits bitstring;

public:

    VNumber(std::string verilog_string)
    {
        //remove underscores
        verilog_string.erase(std::remove(verilog_string.begin(), verilog_string.end(), '_'), verilog_string.end());

        size_t loc = verilog_string.find("\'");
        if(loc == std::string::npos)
        {
            verilog_string.insert(0, "\'d");
            loc = 0;
        }

        this->size = 0;
        if(loc != 0)
            this->size = dec_str_to_int(verilog_string.substr(0,loc));

        this->sign = false;
        if(std::tolower(verilog_string[loc+1]) == 's')
            this->sign = true;

        char base = std::tolower(verilog_string[loc+1+sign]);
        size_t radix = (base == 'b') ? 2   :
                        (base == 'o') ? 8   :
                        (base == 'd') ? 10  :
                        (base == 'h') ? 16  :
                                        -1  ;

        std::string temp_bitstring = string_of_radix_to_bitstring(verilog_string.substr(loc+2+sign), radix);

        // adjust the size of the bitstring
        while(temp_bitstring.length() > 1)
        {
            if(this->size <= 0)
            {
                if(temp_bitstring[0] == temp_bitstring[1])
                    temp_bitstring.erase(temp_bitstring.begin(),temp_bitstring.begin()+1);
                else
                    break;
            }
            else if(this->size > temp_bitstring.length())
            {
                temp_bitstring.insert(temp_bitstring.begin(),this->get_padding_bit());
            }
            else if(this->size < temp_bitstring.length())
            {
                temp_bitstring.erase(temp_bitstring.begin(),temp_bitstring.begin()+1);
            }
            else if(this->size == temp_bitstring.length())
            {
                break;
            }
        }

        // convert the bits to the internal data struct the bitstring is in little endian and we switch to big
        this->bitstring.init_bits(temp_bitstring.size(), _0);
        size_t counter=temp_bitstring.size();
        for(char in: temp_bitstring)
            this->bitstring.set_bit(--counter,from_char(in));
    }

    long long value()
    {
        return bits_str_to_int(this->bitstring.to_string(false));
    }

    bit_value_t bits_from_msb(size_t index)
    {
        if(index < this->size )
            return this->bitstring.get_bit(this->size-index);

        return 0;
    }

    bit_value_t bits_from_lsb(size_t index)
    {
        if(index < this->size )
            return this->bitstring.get_bit(index);

        return 0;
    }

    bool is_negative()
    {
        return ( this->bits_from_msb(0) == _1 && this->sign );
    }

    bit_value_t get_padding_bit()
    {
        return this->is_negative()? _1:_0;
    }

    // convert lsb_msb bitstring to verilog
    std::string to_string()
    {
        std::string out = this->bitstring.to_string(false);
        size_t len = this->size;

        return std::string(
            std::to_string(len) 
            + "\'sb" 
            + out
        );
    }
};

#endif // VNUMBER_HPP
