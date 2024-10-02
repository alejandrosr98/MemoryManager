
#ifndef MEMORYDUMMY_H
#define MEMORYDUMMY_H

#include <inttypes.h>

///Class to handle dummy memory
class MemoryDummy
{
	public:
		///Constructor
		///\param _size Size of the created memory
		MemoryDummy(uint32_t _size)
		{
			mem = new uint8_t[_size];
			memset(mem,0,_size);
		}

		///Method to write a byte to an address of the memory
		bool write(uint32_t _dir, uint8_t _data) { mem[_dir] = _data; return true; }

		///Method to read a byte from an address of the memory
		bool read(uint32_t _dir, uint8_t& _data) { _data = mem[_dir]; return true; }

		///Method to write several bytes to an address of the memory
		int writeBytes(uint32_t _dir, uint8_t* _data, uint16_t _length)
		{
			for (int i = 0; i < _length; i++)
				mem[_dir + i] = _data[i];
			return _length;
		}

		///Method to read several bytes from an address of the memory
		int readBytes(uint32_t _dir, uint8_t* _data, uint16_t _length)
		{
			for (int i = 0; i < _length; i++)
				_data[i] = mem[_dir + i];
			return _length;
		}

	private:
		uint8_t* mem;
};

#endif