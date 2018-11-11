/* Authors: Aaron Graham (aaron.graham@unb.ca, aarongraham9@gmail.com),
 *           Jean-Philippe Legault (jlegault@unb.ca, jeanphilippe.legault@gmail.com) and
 *            Dr. Kenneth B. Kent (ken@unb.ca)
 *            for the Reconfigurable Computing Research Lab at the
 *             Univerity of New Brunswick in Fredericton, New Brunswick, Canada
 */

#include <algorithm>
#include <string>
#include "rtl_utils.h"
#include "v_number.h"

v_number::v_number(std::string verilog_string)
{
    //remove underscores
	verilog_string.erase(std::remove(verilog_string.begin(), verilog_string.end(), '_'), verilog_string.end());

	INT_TYPE loc = verilog_string.find("\'");
	if(loc == std::string::npos)
	{
		verilog_string.insert(0, "\'d");
		loc = 0;
	}

	this->length = 0;
	if(loc != 0)
		this->length = dec_str_to_int(verilog_string.substr(0,loc));

	this->sign = false;
	if(std::tolower(verilog_string[loc+1]) == 's')
		this->sign = true;

    char base = std::tolower(verilog_string[loc+1+sign]);
    INT_TYPE radix = (base == 'b') ? 2   :
                    (base == 'o') ? 8   :
                    (base == 'd') ? 10  :
                    (base == 'h') ? 16  :
                                    -1  ;

	this->bitstring = string_of_radix_to_bitstring(verilog_string.substr(loc+2+sign), radix);
    //truncate the excess bits (unecessary padding or outside of defined length)

    this->resize(this->length);
}

INT_TYPE v_number::get_value()
{
    return bits_str_to_int(this->bitstring);
}

void value(INT_TYPE)
{

}

char v_number::bits_from_msb(INT_TYPE index)
{
    if(index < this->bitstring.size() )
        return *(this->bitstring.end()-index);

    return 0;
}

char v_number::bits_from_lsb(INT_TYPE index)
{
    if(index < this->bitstring.size() )
        return *(this->bitstring.begin()+index);

    return 0;
}

bool v_number::is_negative()
{
    return (this->bits_from_msb(0) == '1');
}

char v_number::get_padding_bit()
{
    return this->is_negative()? '1':'0';
}

void v_number::resize(INT_TYPE len)
{
    while(this->bitstring.length() > 1)
    {
        if(len <= 0)
        {
            if(this->bits_from_msb(0) == this->bits_from_msb(1))
                this->bitstring.pop_back();
            else
                break;
        }
        else if(len > this->bitstring.length())
        {
            this->bitstring.push_back(this->get_padding_bit());
        }
        else if(len < this->bitstring.length())
        {
            this->bitstring.pop_back();
        }
        else if(len == this->bitstring.length())
        {
            break;
        }
    }
}

// convert lsb_msb bitstring to verilog
std::string v_number::to_bitstring()
{
    //final resize to verify length of output.
    this->resize(0);
    std::string out = "";
    INT_TYPE len = 0;
    //print in hex
    if(false)
    {
        std::string out = "";
        for(int i = 0; i< this->bitstring.size(); i+=4)
        {
            std::string bits_to_hex_str = this->bitstring.substr(i,4);
            if (bits_to_hex_str.size() < 4)
                bits_to_hex_str += std::string(4-bits_to_hex_str.size(), this->get_padding_bit());
            
            out.insert(0,1,bits_to_hex(std::string(bits_to_hex_str.crbegin(), bits_to_hex_str.crend())));
        }
        len = out.size() * 4;
    }
    else
    {
        out = std::string(this->bitstring.crbegin(), this->bitstring.crend());
        len = out.size();

    }

	return std::string(
        std::to_string(len) 
        + "\'sb" 
        + out
    );
}
