#include "v_number.h"

v_number::v_number(std::string verilog_string)
{

}

INT_TYPE v_number::value()
{
    return bits_str_to_int(this->bitstring);
}

void value(INT_TYPE)
{

}

char v_number::bits_from_msb(int index)
{
    if(index < this->bitstring.size() )
        return *(this->bitstring.end()-index);

    return 0;
}

char v_number::bits_from_lsb(int index)
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

void v_number::resize(int len)
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