#include "Framework.h"

BinaryReader::BinaryReader(string filePath)
{
    file = CreateFileA(filePath.c_str(), GENERIC_READ,
        FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryReader::BinaryReader(wstring filePath)
{
    file = CreateFile(filePath.c_str(), GENERIC_READ,
        FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryReader::~BinaryReader()
{
    CloseHandle(file);
}

string BinaryReader::String()
{
    UINT strSize = Data<UINT>();

    char* temp = new char[strSize + 1];
    ReadFile(file, temp, sizeof(char) * strSize, &size, nullptr);
    temp[strSize] = '\0';

    return temp;
}

wstring BinaryReader::WString()
{
    UINT strSize = Data<UINT>();

    WCHAR* temp = new WCHAR[strSize + 1];
    ReadFile(file, temp, sizeof(WCHAR) * strSize, &size, nullptr);
    temp[strSize] = '\0';

    return temp;
}

void BinaryReader::Byte(void** data, UINT dataSize)
{
    ReadFile(file, *data, dataSize, &size, nullptr);
}
