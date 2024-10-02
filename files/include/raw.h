#ifndef RAWFILE_H
#define RAWFILE_H

class FileMemory
{
	public:
		FileMemory(string _filename, MemoryDummy _memory) : memory_(_memory) {}
		int write(uint8_t _data);
		int write(uint8_t* _data, uint32_t _size);
		int read(uint8_t& _data);
		int read(uint8_t* _data, uint32_t _size);

	private:
		MemoryDummy memory_;
};

#endif