#include <iostream>
#include <MemoryDummy.h>
#include <raw.h>

using namespace std;
void info(void);

int main(int argc, char** argv)
{
	if (argc < 3)
	{
		info();
		return 0;
	}

	unsigned int memSize = atoi(argv[1]);
	unsigned int fileSize = atoi(argv[2]);
	unsigned int filesPerMem = memSize / fileSize;

	auto memoryDummy = new MemoryDummy(memSize * 1024);

	for (int i = 3; i < argc && (i - 3 < filesPerMem); i++)
	{
		char dataRead[100];
		string file = "";
		FILE* f = fopen(argv[i], "r");
		if (f == NULL)
		{
			cout << "No se abre el archivo" << endl;
			return 0;
		}
		while (fgets(dataRead, 100, f) != NULL)
		{
			file += dataRead;
		}

		fclose(f);
		cout << file << endl << "Fin del archivo " << i - 2 << endl << endl;

		if (file.size() > fileSize * 1024)
		{
			cout << "File bigger than " << fileSize << "kB in memory" << endl;
			break;
		}
		auto f1 = new FileMemory(argv[i], memoryDummy);
		f1->write(file.c_str(),file.size());
	}

	char* data = new char[1024 * memSize];
	memoryDummy->readBytes(0, (uint8_t*)data, 1024 * memSize);

	FILE* out = fopen("eeprom.hex", "w");
	for (uint16_t i = 0, int j = 0; i < 1024 * memSize; i += j)
	{
		fprintf(out, ":10%04X00", i);
		uint8_t checksum = 16 + (uint8_t)((i & 0xFF00) >> 8) + (uint8_t)(i & 0xFF);
		for (j = 0; j < 16; j++)
		{
			if (data[i + j] != 0xFF)
			{
				fprintf(out, "%02X", (uint8_t)data[i + j]);
				checksum += data[i + j];
			}
			else
			{
				j++;
				break;
			}
		}
		checksum = 0xFF - checksum + 1;
		fprintf(out, "%02X\n", checksum);
	}
	fprintf(out, ":00000001FF");
	fclose(out);

	return 0;
}

void info(void)
{
	cout << "Incorrect arguments. The correct format is:" << endl;
	cout << "Params for EEPROM: <size in kb> <max size per file in kb>. Both" << endl;
	cout << "File names: <file1> <file2> ..." << endl;
}