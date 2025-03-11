#include "Framework.h"

BinaryWriter::BinaryWriter(string filePath)
{
    file = CreateFileA(filePath.c_str(), GENERIC_WRITE,
        0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryWriter::BinaryWriter(wstring filePath)
{
    file = CreateFile(filePath.c_str(), GENERIC_WRITE,
        0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
}

BinaryWriter::~BinaryWriter()
{
    CloseHandle(file);
}

void BinaryWriter::String(string data)
{
    Data<UINT>(data.size());

    const char* str = data.c_str();
    WriteFile(file, str, sizeof(char) * data.size(), &size, nullptr);
}

void BinaryWriter::WString(wstring data)
{
    Data<UINT>(data.size());

    const WCHAR* str = data.c_str();
    WriteFile(file, str, sizeof(WCHAR) * data.size(), &size, nullptr);
}

void BinaryWriter::Byte(void* data, UINT dataSize)
{
    WriteFile(file, data, dataSize, &size, nullptr);
}
