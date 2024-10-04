
#ifndef MEMORYDUMMY_H
#define MEMORYDUMMY_H

#include <inttypes.h>
#include <string.h>

///Class to handle dummy memory
class MemoryDummy
{
	public:
		///Constructor
		///\param _size Size of the created memory
		MemoryDummy(uint32_t _size, uint8_t _voidChar = 0xFF)
		{
			mem = new uint8_t[_size];
			memset(mem, _voidChar, _size);
		}

		///Method to write a byte to an address of the memory
		bool write(uint32_t _dir, uint8_t _data)
		{
			if (_dir < sizeof(mem))
			{
				mem[_dir] = _data;
				return true;
			}
			else
			{
				return false;
			}
		}

		///Method to read a byte from an address of the memory
		bool read(uint32_t _dir, uint8_t& _data)
		{
			if (_dir < sizeof(mem))
			{
				_data = mem[_dir];
				return true;
			}
			else
			{
				return false;
			}
		}

		///Method to write several bytes to an address of the memory
		int writeBytes(uint32_t _dir, uint8_t* _data, uint16_t _length)
		{
			uint32_t i;
			if (_dir >= sizeof(mem)) return -1;
			for (i = 0; (i < _length) && (_dir + i < sizeof(mem)); i++)
				mem[_dir + i] = _data[i];
			return i;
		}

		///Method to read several bytes from an address of the memory
		int readBytes(uint32_t _dir, uint8_t* _data, uint16_t _length)
		{
			uint32_t i;
			if (_dir >= sizeof(mem)) return -1;
			for (i = 0; (i < _length) && (_dir + i < sizeof(mem)); i++)
				_data[i] = mem[_dir + i];
			return i;
		}

	private:
		uint8_t* mem;
};

#endif