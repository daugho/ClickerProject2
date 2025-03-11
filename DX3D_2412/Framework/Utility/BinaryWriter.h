#pragma once

class BinaryWriter
{
public:
    BinaryWriter(string filePath);
    BinaryWriter(wstring filePath);
    ~BinaryWriter();

    template <typename T>
    void Data(T data);

    void String(string data);
    void WString(wstring data);    

    void Byte(void* data, UINT dataSize);

private:
    HANDLE file;
    DWORD size = 0;
};

template<typename T>
inline void BinaryWriter::Data(T data)
{
    WriteFile(file, (void*)&data, sizeof(T), &size, nullptr);
}
