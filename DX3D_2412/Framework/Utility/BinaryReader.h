#pragma once

class BinaryReader
{
public:
    BinaryReader(string filePath);
    BinaryReader(wstring filePath);
    ~BinaryReader();

    template <typename T>
    T Data();

    string String();
    wstring WString();

    void Byte(void** data, UINT dataSize);

    bool IsFailed() { return file == INVALID_HANDLE_VALUE; }

private:
    HANDLE file;
    DWORD size = 0;
};

template<typename T>
inline T BinaryReader::Data()
{
    T temp;
    ReadFile(file, &temp, sizeof(T), &size, nullptr);

    return temp;
}
