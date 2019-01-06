#ifndef INTERNAL_BITS_HPP
#define INTERNAL_BITS_HPP

#include <cstdint>
#include <string>
#include <algorithm>

namespace BitSpace {
    typedef uint8_t bit_value_t;

    const bit_value_t _0 = 0x0;
    const bit_value_t _1 = 0x1;
    const bit_value_t _z = 0x2;
    const bit_value_t _x = 0x3;

    bit_value_t from_char(char val_in);
    bit_value_t from_char(char val_in)
    {
        switch(val_in)
        {
            case '0':    return _0;
            case '1':    return _1;
            case 'z':    return _z;
            default:     return _x;
        }            
    }

    char to_char(bit_value_t val_in);
    char to_char(bit_value_t val_in)
    {
        switch(val_in)
        {
            case _0:    return '0';
            case _1:    return '1';
            case _z:    return 'z';
            default:    return 'x';
        } 
    }

    const uint64_t mask = 0x3;

    template<typename T>
    class BitFields
    {
    private :

        T bits;

        size_t get_bit_location(size_t address)
        {
            size_t real_address = ((address%this->size())<<1);
            return real_address;
        }


    public :

        void init_values(bit_value_t init)
        {
            for(size_t i=0; i < this->size() ; i++)
            {
                this->bits |= init;
                init <<= 2;
            }
        }

        BitFields()
        {
            this->init_values(_x);
        }

        BitFields(bit_value_t initial_value)
        {
            this->init_values(initial_value);
        }        
        
        bit_value_t get_bit(size_t address)
        {
            size_t real_address = this->get_bit_location(address);
            return (this->bits >> real_address) & mask;
        }

        void set_bit(size_t address, bit_value_t value)
        {	
            size_t real_address = this->get_bit_location(address);

            T long_value = value;

            T set_value = long_value << real_address;
            T zero_out_location = ~(mask << real_address);

            this->bits &= zero_out_location;
            this->bits |= set_value;
        }

        size_t size()
        {
            return (sizeof(bits)<<2);
        }
    };
}

#endif
