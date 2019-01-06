#ifndef VERILOG_BITS_HPP
#define VERILOG_BITS_HPP

#include <algorithm>
#include <vector>
#include <assert.h>
#include <string>
#include <memory>
#include <atomic>

#include "stdio.h"
#include "internal_bits.hpp"

class VerilogBits
{
private:

    BitSpace::BitFields<uint8_t> *bits;
    uint16_t bit_size;

    inline int16_t to_index(int16_t address)
    {
        return (address >> 2);
    }

public:

    VerilogBits()
    {
    }

    VerilogBits(size_t data_size, BitSpace::bit_value_t value_in)
    {
        
        this->init_bits(data_size, value_in);
    }

    void init_bits(size_t data_size, BitSpace::bit_value_t value_in)
    {
        this->bit_size = static_cast<uint16_t>(data_size);
        this->bits = new BitSpace::BitFields<uint8_t>[to_index(this->bit_size)];
        for(size_t i=0; i < to_index(this->bit_size); i++)
        {
            bits[i].init_values(value_in);
        }
    }

    size_t size()
    {
        return bit_size;
    }

    BitSpace::bit_value_t get_bit(size_t address)
    {
        assert(address < this->size()
            && "accessing out of bound location for verilog bits");

        return this->bits[to_index(address)].get_bit(address);
    }

    void set_bit(size_t address, BitSpace::bit_value_t value)
    {
        assert(address < this->size()
            && "accessing out of bound location for verilog bits");

        this->bits[to_index(address)].set_bit(address, value);
    }

    std::string to_string(bool big_endian)
    {
        int start =      (big_endian)? 0x0: this->size()-1;
        int end =        (big_endian)? this->size()-1: 0x0;
        int increment =  (big_endian)? 1: -1;

        std::string to_return = "";
        for(int address=start; (big_endian)? (address <= static_cast<int>(end)): (address >= static_cast<int>(end)); address += increment)
        {
            to_return.push_back(BitSpace::to_char(this->bits[to_index(address)].get_bit(address)));
        }
        return to_return;
    }

};

#endif // VERILOG_BITS_HPP
