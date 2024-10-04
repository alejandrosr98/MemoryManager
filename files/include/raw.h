#ifndef RAWFILE_H
#define RAWFILE_H

#include <MemoryDummy.h>
#include <string>

class FileMemoryBase
{
	public:
		FileMemoryBase(MemoryDummy* _memory) : memory_(_memory) {}
		virtual int write(const char _data) = 0;
		virtual int write(const char* _data, uint32_t _size) = 0;
		virtual int read(const char& _data) = 0;
		virtual int read(const char* _data, uint32_t _size) = 0;

	protected:
		MemoryDummy* memory_;
};

class FileMemory : public FileMemoryBase
{
	public:
		FileMemory(std::string _filename, MemoryDummy* _memory) : FileMemoryBase(_memory) {}
		int write(const char _data);
		int write(const char* _data, uint32_t _size);
		int read(const char& _data);
		int read(const char* _data, uint32_t _size);

	private:
		uint32_t writeIndex = 0;
		uint32_t readIndex = 0;
};

#endif