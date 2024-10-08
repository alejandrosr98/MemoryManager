#include <iostream>
#include <MemoryDummy.h>
#include <string>

using namespace std;
void info(void);

int main(int argc, char** argv)
{
	if (argc < 5)
	{
		info();
		return 0;
	}

	bool eepromInfo = false;
	bool fileInfo = false;
	bool clearByteInfo = false;

	unsigned int memSize = 0;

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
					else if (i >= argc - 1)
					{
						cout << "Not enough arguments for EEPROM information." << endl;
						info();
						return 0;
					}
					else if ('0' <= argv[i + 1][0] && argv[i + 1][0] <= '9')
					{
						memSize = atoi(argv[i + 1]);
						eepromInfo = true;
						i++;
						if (memSize == 0)
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
					else if (i >= argc - 1)
					{
						cout << "Not enough arguments for file names." << endl;
						info();
						return 0;
					}
					else if (argv[i + 1][0] == '-')
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
					if (clearByteInfo)
					{
						cout << "Argument -" << argv[i][1] << " already entered." << endl;
						info();
						return 0;
					}
					else if (i >= argc - 1)
					{
						cout << "Not enough arguments for not flashed bytes info." << endl;
						info();
						return 0;
					}
					else if (argv[i + 1][0] == '-')
					{
						cout << "Not enough arguments for not flashed bytes info." << endl;
						info();
						return 0;
					}
					else
					{
						i++;
						unsigned long voidByte = stoul(argv[i], nullptr, 16);
						if (voidByte > 0xFF)
						{
							cout << "WARNING: Not flashed bytes representation should not be longer than one byte. Default value 0xFF will be used." << endl;
						}
						else
						{
							clearedBytes = (uint8_t)voidByte;
						}
						clearByteInfo = true;
					}
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

	fileList = new char* [fileNumber];

	cout << "Memory Size: " << memSize << " kb." << endl;
	cout << "File number: " << fileNumber << endl;
	cout << "File list:" << endl;

	for (int i = 0; i < fileNumber; i++)
	{
		fileList[i] = argv[fileIndex + i];
		cout << "\t-> " << fileList[i] << endl;
	}

	cout << "Void bytes: ";
	printf("0x%02X\n\n", clearedBytes);

	auto memoryDummy = new MemoryDummy(memSize * 1024, clearedBytes);
	uint32_t initDir = 0;

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

		if (initDir + file.size() > memSize * 1024)
		{
			cout << "WARNING: Files entered bigger than the memory capacity. The first ones will be used until the memory capacity" << endl;
			break;
		}
		memoryDummy->writeBytes(initDir,(uint8_t*)file.data(),file.size());
		initDir += (uint32_t)(file.size() + 1);
	}

	char* data = new char[1024 * memSize];
	memoryDummy->readBytes(0, (uint8_t*)data, 1024 * memSize);

	FILE* out = fopen("eeprom.hex", "w");
	for (uint16_t i = 0, j = 0; i < 1024 * memSize; i += j)
	{
		if ((uint8_t)data[i] != clearedBytes)
		{
			fprintf(out, ":10%04X00", i);
			uint8_t checksum = 16 + (uint8_t)((i & 0xFF00) >> 8) + (uint8_t)(i & 0xFF);
			for (j = 0; j < 16; j++)
			{
				if ((uint8_t)data[i + j] != clearedBytes)
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
	cout << "Mandatory:" << endl;
	cout << "\t-e\tParams for EEPROM: <size in kb>." << endl;
	cout << "\t-f\tFile names: <file1> <file2> ... At least one." << endl;
	cout << "Options:" << endl;
	cout << "\t-b\tNot flashed bytes representation in hex format. Only one byte long. Default 0xFF." << endl;
}