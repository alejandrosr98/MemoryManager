#include <raw.h>

int FileMemory::write(const char _data)
{
	bool ret = memory_->write(writeIndex, (uint8_t)_data);
	if (ret)
	{
		writeIndex++;
		return 1;
	}
	else return 0;
}
int FileMemory::write(const char* _data, uint32_t _size)
{
	int ret = memory_->writeBytes(writeIndex, (uint8_t*)_data, _size);
	writeIndex += ret;
	return ret;
}
int FileMemory::read(const char& _data)
{
	bool ret = memory_->read(readIndex, (uint8_t&)_data);
	if (ret)
	{
		readIndex++;
		return 1;
	}
	else return 0;
}

int FileMemory::read(const char* _data, uint32_t _size)
{
	int ret = memory_->readBytes(readIndex, (uint8_t*)_data, _size);
	readIndex += ret;
	return ret;
}