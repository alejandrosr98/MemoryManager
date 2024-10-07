#include <iostream>
#include <MemoryDummy.h>
#include <raw.h>

using namespace std;
void info(void);

int main(int argc, char** argv)
{
	if (argc < 6)
	{
		info();
		return 0;
	}

	bool eepromInfo = false;
	bool fileInfo = false;

	unsigned int memSize = 0;
	unsigned int fileSize = 0;

	char** fileList;
	int fileIndex;
	int fileNumber;

	uint8_t clearedBytes = 0xFF;

	for (int i = 1; i < argc; i++)
	{
		if (argv[i][0] == '-')
		{
			switch (argv[i][1])
			{
				case 'e':
					if (eepromInfo)
					{
						cout << "Argument -" << argv[i][1] << " already entered." << endl;
						info();
						return 0;
					}
					else if (i >= argc - 2)
					{
						cout << "Not enough arguments for EEPROM information." << endl;
						info();
						return 0;
					}
					else if ('0' <= argv[i + 1][0] && argv[i + 1][0] <= '9' && '0' <= argv[i + 2][0] && argv[i + 2][0] <= '9')
					{
						memSize = atoi(argv[i + 1]);
						fileSize = atoi(argv[i + 2]);
						eepromInfo = true;
						i += 2;
						if (memSize == 0 || fileSize == 0)
						{
							info();
							return 0;
						}
					}
					else
					{
						info();
						return 0;
					}
					break;
				case 'f':
					if (fileInfo)
					{
						cout << "Argument -" << argv[i][1] << " already entered." << endl;
						info();
						return 0;
					}
					else if ((i >= argc - 1) || (argv[i + 1][0] == '-'))
					{
						cout << "Not enough arguments for file names." << endl;
						info();
						return 0;
					}
					else
					{
						fileNumber = 0;
						while ((i + 1 + fileNumber) < argc && argv[i + 1 + fileNumber][0] != '-')
						{
							fileNumber++;
						}
						fileIndex = i + 1;
						i += fileNumber;
						fileInfo = true;
					}
					break;
				case 'b':
					break;
				default:
					cout << "Argument -" << argv[i][1] << " not supported." << endl;
					info();
					return 0;
			}
		}
		else
		{
			info();
			return 0;
		}
	}

	if (!eepromInfo || !fileInfo)
	{
		info();
		return 0;
	}

	unsigned int filesPerMem = memSize / fileSize;
	if (filesPerMem < fileNumber)
	{
		cout << "WARNING: More files entered than the memory supports. The first ones will be used until the memory capacity" << endl;
		fileNumber = filesPerMem;
	}

	fileList = new char* [fileNumber];

	cout << "Memory Size: " << memSize << " kb. Max File Size: " << fileSize << " kb." << endl;
	cout << "File number: " << fileNumber << endl;
	cout << "File list:" << endl;

	for (int i = 0; i < fileNumber; i++)
	{
		fileList[i] = argv[fileIndex + i];
		cout << "\t-" << fileList[i] << endl;
	}

	auto memoryDummy = new MemoryDummy(memSize * 1024, clearedBytes);

	for (int i = 0; i < fileNumber; i++)
	{
		char dataRead[100];
		string file = "";
		FILE* f = fopen(fileList[i], "r");
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
		cout << file << endl << "Fin del archivo " << i + 1 << endl << endl;

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
	for (uint16_t i = 0, j = 0; i < 1024 * memSize; i += j)
	{
		if ((uint8_t)data[i] != 0xFF)
		{
			fprintf(out, ":10%04X00", i);
			uint8_t checksum = 16 + (uint8_t)((i & 0xFF00) >> 8) + (uint8_t)(i & 0xFF);
			for (j = 0; j < 16; j++)
			{
				if ((uint8_t)data[i + j] != 0xFF)
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
		else
		{
			j = 1;
		}
	}
	fprintf(out, ":00000001FF");
	fclose(out);

	return 0;
}

void info(void)
{
	cout << "Incorrect arguments. The correct format is:" << endl;
	cout << "Mandatory" << endl;
	cout << "\t-e\tParams for EEPROM: <size in kb> <max size per file in kb>. Both." << endl;
	cout << "\t-f\tFile names: <file1> <file2> ... At least one." << endl;
	cout << "Options:" << endl;
	cout << "\t-b\tNot flashed bytes representation in hex format. Default 0xFF" << endl;
}